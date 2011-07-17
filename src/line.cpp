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

#include "line.h"
#include <math.h>
#include "point.h"

Line::Line(double xi, double yi, double zi,double xe, double ye, double ze)
{
  double xd,yd,zd;
  xd = xe-xi;
  yd = ye-yi;
  zd = ze-zi;
  x1 = xi;
  y1 = yi;
  z1 = zi;
  x2 = xd;
  y2 = yd;
  z2 = zd;
  prev = 0;
  next = 0;
}

Line::Line(Line* parent, double xi, double yi, double zi, double xe, double ye, double ze)
{
  double xd,yd,zd;
  xd = xe-xi;
  yd = ye-yi;
  zd = ze-zi;
  length = sqrt(xd*xd + yd*yd + zd*zd);
  x1 = xi;
  y1 = yi;
  z1 = zi;
  x2 = xd;
  y2 = yd;
  z2 = zd;
  prev = parent;
  parent->next = this;
  next = 0;
}

Line::~Line()
{

}






//#include "line.moc"
