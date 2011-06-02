/*
            This file is part of MechCAM.

    MechCAM is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    MechCAM is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MechCAM.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "caddrawing.h"
#include <QtCore/QTimer>
#include <QtOpenGL>
#include <QKeyEvent>
#include <stdio.h>
#include <qgl.h>
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library

CADDrawing::CADDrawing( int timerInterval, QWidget* parent, char* name ): QGLWidget(  parent )
{
  QTimer* t = new QTimer( this );
  setFocusPolicy(Qt::StrongFocus);
  points = 0;
  lines  = 0;
  t->start( timerInterval );
  connect( t, SIGNAL( timeout(  ) ),
                   this, SLOT( timeOut()) );
}

void CADDrawing::initializeGL()
{
        QSettings settings;

        transX = 0;
        transY = 0;
        transZ = 0;
        zoomlevel = 1.0;
        curr_height = this->height();
        curr_width = this->width();
//	currentClearColor = new QColor(0,0,127);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glViewport(0, 0, this->width(), this->height());
//        glClearColor(0.0, 0.0, 0.5, 1.0);
        QColor bgcolor = settings.value("drawingarea/bgcolor").value<QColor>();
        qglClearColor( bgcolor );
        glDisable(GL_NORMALIZE);
        //glEnable(GL_DEPTH_TEST);
        glShadeModel(GL_SMOOTH);
        glDepthRange(0.0f,1.0f);
        draw_Axes();
        addPoint(1,1,0);
        addPoint(2,2,0);
        addPoint(1,2,0);
        addPoint(2,1,0);
        addPoint(1,0,0);
        addPoint(2,0,0);
        addPoint(0,1,0);
        addPoint(0,2,0);
        addLine(4,0,0,4,8,0);
        addLine(8,12,0,12,12,0);
        addLine(14,5,0,14,10,0);
        addLine(0,2,0,11,2,0);
        addLine(4,8,0,8,12,0);
        glTranslated(-2.5, -2.5, 0);
}


void CADDrawing::resizeGL(int w, int h)
{
  double tmp,sf;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glViewport(0, 0, w, h);

        tmp = (double)h/(double)w * 1.0;
        sf = ((double) curr_height/(double) curr_width)/tmp;
        //zoomlevel = zoomlevel * sf;
       // glScalef(tmp * zoomlevel, tmp*zoomlevel, 1* zoomlevel);
        glMatrixMode(GL_MODELVIEW);
  /* adjust viewport to new size */
//  glViewport(0, 0, (GLint)w, (GLint)h);
}

void CADDrawing::timeOut()
{
  updateGL(  );
}

void CADDrawing::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslated(transX,transY,transZ);
  glScalef(((float)this->physicalDpiX()/(float)this->width())*zoomlevel,((float)this->physicalDpiY()/(float)this->height())*zoomlevel, 1* zoomlevel);
  glRotatef(asin(tan(30)),0.0,0.0f, 1.0f);
  draw_Axes();
  draw_stock();
  draw_points();
  draw_lines();
  draw_circles(14,2,0,3);
  draw_circles(12,10,0,2);
  draw_scale_marker();
  renderText(50,50,QString::number(zoomlevel,'f',-1));
  glMatrixMode(GL_MODELVIEW);
}

void CADDrawing::draw_Axes()
{
        glBegin(GL_LINES);
          glColor3f(1.0, 1.0, 0.0); // yellow
          glVertex3f(0.0, 0.0, 0.0);
          glVertex3f(0.0, 0.1, 0.0);
          glVertex3f(-0.025, 0.25,  0.0);
          glVertex3f(0.0, 0.2, 0.0);
          glVertex3f(0.025, 0.25,  0.0);
          glVertex3f(0.0, 0.2, 0.0);
          glVertex3f(0.0, 0.15,  0.0);
          glVertex3f(0.0, 0.2, 0.0);
          glColor3f(1.0, 0.0, 0.0);  // red
          glVertex3f(0.0, 0.0,  0.0);
          glVertex3f(0.0, 0.0, 0.1);
          glColor3f(0.0, 1.0, 0.0);  // green
          glVertex3f(0.0, 0.0, 0.0);
          glVertex3f(0.1, 0.0, 0.0);
          glVertex3f(0.25, -0.025,  0.0);
          glVertex3f(0.15, 0.025, 0.0);
          glVertex3f(0.15, -0.025,  0.0);
          glVertex3f(0.25, 0.025, 0.0);
          glTranslated(transX,transY,transZ);
        glEnd();
}

void CADDrawing::draw_points()
{
    Point* tmp;
    if (points != 0 ){
      tmp = points;
      glBegin(GL_POINTS);
        glVertex3f(tmp->X,tmp->Y,tmp->Z);
        while (tmp->next !=0) {
          tmp = tmp->next;
          glVertex3f(tmp->X,tmp->Y,tmp->Z);
        }
      glEnd();
    }
}

void CADDrawing::draw_lines()
{
    Line* tmp;
    if (lines != 0 ){
      tmp = lines;
      glBegin(GL_LINES);
        glVertex3f(tmp->start_point.X,tmp->start_point.Y,tmp->start_point.Z);
        glVertex3f(tmp->start_point.X + tmp->unit_vector.X*tmp->length,
                   tmp->start_point.Y + tmp->unit_vector.Y*tmp->length,
                   tmp->start_point.Z + tmp->unit_vector.Z*tmp->length);
        while (tmp->next !=0) {
          tmp = tmp->next;
          glVertex3f(tmp->start_point.X,tmp->start_point.Y,tmp->start_point.Z);
          glVertex3f(tmp->start_point.X + tmp->unit_vector.X*tmp->length,
                     tmp->start_point.Y + tmp->unit_vector.Y*tmp->length,
                     tmp->start_point.Z + tmp->unit_vector.Z*tmp->length);
        }
      glEnd();
    }
}


void CADDrawing::draw_circles(float i, float j, float k, float r)
{
  float circle_x,circle_y;
    glBegin(GL_LINES);
    for (int foo = 0; foo < 180; foo++)
    {
    circle_x = r * cos(foo);// - h;
    circle_y = r * sin(foo);// + k;
    glVertex3f(circle_x + i,j - circle_y,0);

    circle_x = r * cos(foo + 0.1);// - h;
    circle_y = r * sin(foo + 0.1);// + k;
    glVertex3f(circle_x + i,j - circle_y,0);
    }
    glEnd();
}

void CADDrawing::draw_stock()
{
    glBegin(GL_LINES);
        glColor3f(1.0,0,0);
// Draw Lower Stock Surface
        glVertex3f(0-stock_origin_x,0-stock_origin_y,0-stock_origin_z);
        glVertex3f(stock_xdim-stock_origin_x,0-stock_origin_y,0-stock_origin_z);
        glVertex3f(stock_xdim-stock_origin_x,0-stock_origin_y,0-stock_origin_z);
        glVertex3f(stock_xdim-stock_origin_x,stock_ydim-stock_origin_y,0-stock_origin_z);
        glVertex3f(stock_xdim-stock_origin_x,stock_ydim-stock_origin_y,0-stock_origin_z);
        glVertex3f(0-stock_origin_x,stock_ydim-stock_origin_y,0-stock_origin_z);
        glVertex3f(0-stock_origin_x,stock_ydim-stock_origin_y,0-stock_origin_z);
        glVertex3f(0-stock_origin_x,0-stock_origin_y,0-stock_origin_z);
// Draw Upper Stock Surface
        glVertex3f(0-stock_origin_x,0-stock_origin_y,-stock_zdim-stock_origin_z);
        glVertex3f(stock_xdim-stock_origin_x,0-stock_origin_y,-stock_zdim-stock_origin_z);
        glVertex3f(stock_xdim-stock_origin_x,0-stock_origin_y,-stock_zdim-stock_origin_z);
        glVertex3f(stock_xdim-stock_origin_x,stock_ydim-stock_origin_y,-stock_zdim-stock_origin_z);
        glVertex3f(stock_xdim-stock_origin_x,stock_ydim-stock_origin_y,-stock_zdim-stock_origin_z);
        glVertex3f(0-stock_origin_x,stock_ydim-stock_origin_y,-stock_zdim-stock_origin_z);
        glVertex3f(0-stock_origin_x,stock_ydim-stock_origin_y,-stock_zdim-stock_origin_z);
        glVertex3f(0-stock_origin_x,0-stock_origin_y,-stock_zdim-stock_origin_z);
// Draw Stock Corners
        glVertex3f(0-stock_origin_x,0-stock_origin_y,0-stock_origin_z);
        glVertex3f(0-stock_origin_x,0-stock_origin_y,-stock_zdim-stock_origin_z);
        glVertex3f(stock_xdim-stock_origin_x,0-stock_origin_y,0-stock_origin_z);
        glVertex3f(stock_xdim-stock_origin_x,0-stock_origin_y,-stock_zdim-stock_origin_z);
        glVertex3f(stock_xdim-stock_origin_x,stock_ydim-stock_origin_y,0-stock_origin_z);
        glVertex3f(stock_xdim-stock_origin_x,stock_ydim-stock_origin_y,-stock_zdim-stock_origin_z);
        glVertex3f(0-stock_origin_x,stock_ydim-stock_origin_y,0-stock_origin_z);
        glVertex3f(0-stock_origin_x,stock_ydim-stock_origin_y,-stock_zdim-stock_origin_z);
    glEnd();
}

void CADDrawing::draw_scale_marker()
{
    int xscale,yscale;

    xscale = this->width()/this->physicalDpiX();
    yscale = this->height()/this->physicalDpiY();
    renderText(this->width()-75,this->height()-100,QString::number(xscale,10));
    renderText(this->width()-75,this->height()-50,QString::number(yscale,10));
    renderText(25,this->height()-100,QString::number(transX,'f',-1));
    renderText(25,this->height()-50,QString::number(transY,'f',-1));
    glBegin(GL_LINES);
        glVertex3d((xscale-1)-(transX*xscale),(0.5*yscale)-(1*yscale)-(transY*yscale),0);
        glVertex3d(xscale-(transX*xscale),(0.5*yscale)-(1*yscale)-(transY*yscale),0);
    glEnd();
}


void CADDrawing::keyPressEvent(QKeyEvent* event)
{
   if (event->key() == Qt::Key_Left) {
       transX -= 0.01;
       updateGL();
   }
   if (event->key() == Qt::Key_Right) {
       transX += 0.01;
       updateGL();
   }
   if (event->key() == Qt::Key_Minus) {
      zoomlevel -= 0.01;

      updateGL();
   }
   if (event->key() == Qt::Key_Plus) {
      zoomlevel += 0.01;
      updateGL();
   }
   if (event->key() == Qt::Key_Up) {
      transY += 0.01;
      updateGL();
   }
   if (event->key() == Qt::Key_Down) {
      transY -= 0.01;
      updateGL();
   }
}

void CADDrawing::addPoint(float x, float y, float z)
{
  Point* tmp;
  if (points != 0) {
    if (points->next == 0) {
      points->next= new Point(points,x,y,z);
    } else {
      tmp = points->next;
      while (tmp->next != 0 ) {
        tmp = tmp->next;
      }
      tmp->next = new Point(tmp,x,y,z);
    }
  } else {
    points = new Point(x,y,z);
  }
}

void CADDrawing::addLine(double xi, double yi, double zi, double xe, double ye, double ze)
{
  Line* tmp;
  if (lines != 0) {
    if (lines->next == 0) {
      lines->next= new Line(lines,xi,yi,zi,xe,ye,ze);
    } else {
      tmp = lines->next;
      while (tmp->next != 0 ) {
        tmp = tmp->next;
      }
      tmp->next = new Line(tmp,xi,yi,zi,xe,ye,ze);
    }
  } else {
    lines = new Line(xi,yi,zi,xe,ye,ze);
  }
}

void CADDrawing::changeStock(double x_dim, double y_dim, double z_dim, double x_off, double y_off, double z_off)
{
    stock_origin_x = x_off;
    stock_origin_y = y_off;
    stock_origin_z = z_off;
    stock_xdim = x_dim;
    stock_ydim = y_dim;
    stock_zdim = z_dim;
}

#include "caddrawing.moc"
