/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Larry W Housner <gobhain@gsarmail.com>

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


#ifndef CADDRAWING_H
#define CADDRAWING_H

#include <QGLWidget>
#include "point.h"
#include "line.h"
#include "circle.h"


class CADDrawing : public QGLWidget
{
  Q_OBJECT

public:
  CADDrawing( int timerInterval=0, QWidget *parent=0, char *name=0 );
  int ClearRed;
  int ClearBlue;
  int ClearGreen;
  void setStockOrigin(double x, double y, double z);
  void setStockSize(double width, double length, double thickness);
  void addPoint(double x, double y, double z);
  void addLine(double xi, double yi, double zi, double xe, double ye, double ze);
  void addCircle(double x, double y, double z, double r, double i, double j, double k);
  int stockVisible;
protected:
  void initializeGL();
  void resizeGL( int w, int h );
  void paintGL();

  void keyPressEvent( QKeyEvent *event );
  void mouseMoveEvent( QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

protected slots:
  void timeOut();
  void changeStock(double x_dim, double y_dim, double z_dim, double x_off, double y_off, double z_off);
signals:
  void mouseMoved(double x, double y, double z);
private:
  QTimer *m_timer;
  void draw_Axes();
  void draw_points();
  void draw_lines();
  void draw_circles();
  void draw_stock();
  void draw_scale_marker();
  double transX;
  double transY;
  double transZ;
  double zoomlevel;
  double stock_origin_x;
  double stock_origin_y;
  double stock_origin_z;
  double stock_xdim;
  double stock_ydim;
  double stock_zdim;
  double position[3];
  int curr_width;
  int curr_height;
  Point* points;
  Line*	 lines;
  Circle* circles;
};


#endif // CADDRAWING_H
