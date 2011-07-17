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
#include <gp_Pnt.hxx>
#include <algorithm>

CADDrawing::CADDrawing( int timerInterval, QWidget* parent, char* name ): QGLWidget(  parent )
{
  QTimer* t = new QTimer( this );
  setFocusPolicy(Qt::StrongFocus);
  points = 0;
  lines  = 0;
  circles = 0;
  t->start( timerInterval );
  connect( t, SIGNAL( timeout(  ) ),
                   this, SLOT( timeOut()) );
  this->setMouseTracking(true);
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
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
 // gluPerspective(65, (float)this->curr_width/(float)this->curr_height, 1, 1000);
  glMatrixMode(GL_MODELVIEW);

  glScalef(((float)this->physicalDpiX()/this->curr_width)*zoomlevel,((float)this->physicalDpiY()/this->curr_height)*zoomlevel, 1* zoomlevel);
  //glRotatef(asin(tan(30)),0.0,0.0f, 1.0f);
  draw_Axes();
  draw_stock();
  draw_points();
  draw_lines();
  draw_circles();
  draw_scale_marker();
  for(std::list<CADObject*>::iterator It=objects.begin(); It!=objects.end() ;It++)
  	{
  		CADObject* object = *It;
  		//if(object->OnVisibleLayer() && object->visible)
  		//{
  			object->glCommands(false, false, false);
  		//}
  	}



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
		glColor3f(0,0,1);
        glVertex3f(tmp->X,tmp->Y,tmp->Z);
        while (tmp->GetFirstChild() !=0) {
          tmp = (Point *)tmp->GetFirstChild();
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
        glVertex3f(tmp->x1 ,tmp->y1 ,tmp->z1);
        glVertex3f(tmp->x2 ,tmp->y2 ,tmp->z2);
        while (tmp->next !=0) {
          tmp = tmp->next;
          glVertex3f(tmp->x1 ,tmp->y1 ,tmp->z1);
          glVertex3f(tmp->x2 ,tmp->y2, tmp->z2);
        }
      glEnd();
    }
}


void CADDrawing::draw_circles()
{
  Circle *tmp;
  float circle_x,circle_y;
  if (circles != 0){
  tmp = circles;
  do {
  glBegin(GL_LINES);
    for (int foo = (int)(tmp->Begin/2); foo < (int)(tmp->End/2); foo++)
    {
    circle_x = tmp->R * cos(foo);// - h;
    circle_y = tmp->R * sin(foo);// + k;
    glVertex3f(circle_x + tmp->X,tmp->Y - circle_y,0);

    circle_x = tmp->R * cos(foo + 0.1);// - h;
    circle_y = tmp->R * sin(foo + 0.1);// + k;
    glVertex3f(circle_x + tmp->X,tmp->Y - circle_y,0);
    }
    glEnd();
    tmp = tmp->next;
}while(tmp !=0);
}
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

bool CADDrawing::add(CADObject *obj, CADObject *prev_obj){
	if (obj==NULL) return false;
//	if (!CanAdd(obj)) return false;
	if (std::find(objects.begin(), objects.end(), obj) != objects.end()) return true; // It's already here.

	if(obj->GetType() == PointType){
		printf("\nAdding Point\n");
	}
	if (objects.size()==0 || prev_obj==NULL)
	{
		objects.push_back(obj);
		LoopIt = objects.end();
		LoopIt--;
	}
	else
	{
		for(LoopIt = objects.begin(); LoopIt != objects.end(); LoopIt++) { if (*LoopIt==prev_obj) break; }
		objects.insert(LoopIt, obj);
	}
	index_list_valid = false;
	//objects->add(obj, prev_obj);

/*
	if(((!heeksCAD->InOpenFile() || !heeksCAD->FileOpenTypeHeeks() || heeksCAD->InPaste()) && object->UsesID() && (object->m_id == 0 || (heeksCAD->FileOpenTypeHeeks() && heeksCAD->InOpenFile()))))
	{
		object->SetID(heeksCAD->GetNextID(object->GetIDGroupType()));
	}
*/
	return true;
}

void CADDrawing::addPoint(double x, double y, double z)
{

  Point *tmp;
  gp_Pnt *pnt;

  tmp = new Point(x,y,z);

  pnt = new gp_Pnt(x,y,z);
  add(tmp,NULL);
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

void CADDrawing::addCircle(double x, double y, double z, double r, double i, double j, double k)
{
  Circle* tmp;
  if (circles != 0) {
    if (circles->next == 0) {
      circles->next= new Circle(circles,x,y,z,r,i,j,k);
    } else {
      tmp = circles->next;
      while (tmp->next != 0 ) {
        tmp = tmp->next;
      }
      tmp->next = new Circle(tmp,x,y,z,r,i,j,k);
    }
  } else {
    circles = new Circle(x,y,z,r,i,j,k);
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

void CADDrawing::mouseMoveEvent(QMouseEvent *event)
{


       glLoadIdentity();
       //only if left mouse button is down
       // (set in the mouse press event for example)
          //****
          //project window coords to gl coord
          //*****

          GLdouble modelMatrix[16];
          glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
          GLdouble projMatrix[16];
          glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
          int viewport[4];
          glGetIntegerv(GL_VIEWPORT,viewport);
          gluUnProject(
             event->x(),
             event->y(),
             0,
             modelMatrix,
             projMatrix,
             viewport,
             //the next 3 parameters are the pointers to the final object
             //coordinates. Notice that these MUST be double's
             &position[0], //-&gt; pointer to your own position (optional)
             &position[1], // id
             &position[2] // id
          );
          //coords of the object are now saved in position.
          double xscale,yscale;

         xscale = (double)this->width()/(double)this->physicalDpiX();
         yscale = (double)this->height()/(double)this->physicalDpiY();
        emit mouseMoved((position[0]-transX)*(xscale/zoomlevel),(position[1]-transY)*(yscale/zoomlevel),position[2]);

}

void CADDrawing::wheelEvent(QWheelEvent *event)
 {
     int numDegrees = event->delta() / 8;
     int numSteps = numDegrees / 15;

     if (event->delta() < 0 ) {
        zoomlevel -= 0.01;

        updateGL();
     }
     if (event->delta() >= 0 ) {
        zoomlevel += 0.01;
        updateGL();
     }
     event->accept();
 }



#include "caddrawing.moc"
