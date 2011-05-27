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


#ifndef LINE_H
#define LINE_H

#include "point.h"

class Line
{

  public:
    Line(double xi, double yi, double zi, double xe, double ye, double ze);
    Line(Line* parent, double xi, double yi, double zi, double xe, double ye, double ze);
    virtual ~Line();
    Point unit_vector;
    Point start_point;
    double length;
    Line* next;
    Line* prev;
};

#endif // LINE_H
