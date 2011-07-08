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

#include "circle.h"

Circle::Circle()
{
  prev = 0;
  next = 0;
  X = 0;
  Y = 0;
  Z = 0;
  R = 0;
  I = 0;
  J = 0;
  K = 0;
  Begin = 0;
  End = 0;
}

Circle::Circle(double x, double y, double z, double r, double i, double j, double k)
{
  prev = 0;
  next = 0;
  X = x;
  Y = y;
  Z = z;
  R = r;
  I = i;
  J = j;
  K = k;
  Begin = 0;
  End = 360;

}

Circle::Circle(double x, double y, double z, double r, double i, double j, double k, double begin, double end)
{
    prev = 0;
    next = 0;
    X = x;
    Y = y;
    Z = z;
    R = r;
    I = i;
    J = j;
    K = k;
    Begin = begin;
    End = end;
}

Circle::Circle(Circle *parent, double x, double y, double z, double r, double i, double j, double k)
{
  parent->next = this;
  prev = parent;
  next = 0;
  X = x;
  Y = y;
  Z = z;
  R = r;
  I = i;
  J = j;
  K = k;
  Begin = 0;
  End = 360;

}

Circle::Circle(Circle *parent, double x, double y, double z, double r, double i, double j, double k, double begin, double end)
{
    parent->next = this;
    prev = parent;
    next = 0;
    X = x;
    Y = y;
    Z = z;
    R = r;
    I = i;
    J = j;
    K = k;
    Begin = begin;
    End = end;
}

Circle::~Circle()
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
