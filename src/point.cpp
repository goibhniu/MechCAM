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

#include "point.h"

Point::Point()
{
  prev = 0;
  next = 0;
  X = 0;
  Y = 0;
  Z = 0;
}

Point::Point(double x, double y, double z)
{
  prev = 0;
  next = 0;
  X = x;
  Y = y;
  Z = z;
}


Point::Point(Point* parent, double x, double y, double z)
{
  parent->next = this;
  prev = parent;
  next = 0;
  X = x;
  Y = y;
  Z = z;

}

Point::~Point()
{
  if (next != 0) {
    if ( prev !=0 ){
      next->prev = prev;
    } else {
      next->prev = 0;
    }
  }
  if (prev != 0) {
    if ( next != 0 ) {
      prev->next = next;
    } else {
      prev->next = 0;
    }
  }
//  free(this);
}



//#include "point.moc"
