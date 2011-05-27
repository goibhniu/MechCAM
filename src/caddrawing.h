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


class CADDrawing : public QGLWidget
{
  Q_OBJECT

public:
  CADDrawing( int timerInterval=0, QWidget *parent=0, char *name=0 );
  int ClearRed;
  int ClearBlue;
  int ClearGreen;
  void addPoint(float x, float y, float z);
  void addLine(double xi, double yi, double zi, double xe, double ye, double ze);
protected:
  void initializeGL();
  void resizeGL( int w, int h );
  void paintGL();

  void keyPressEvent( QKeyEvent *event );


protected slots:
  void timeOut();

private:
  QTimer *m_timer;
  void draw_Axes();
  void draw_points();
  void draw_lines();
  void draw_circles(float i, float j, float k, float r);
  double transX;
  double transY;
  double transZ;
  double zoomlevel;
  int curr_width;
  int curr_height;
  Point* points;
  Line*	 lines;
};


#endif // CADDRAWING_H
