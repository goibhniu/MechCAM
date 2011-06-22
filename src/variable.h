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

#ifndef VARIABLE_H
#define VARIABLE_H


class Variable
{

  public:
    Variable();
    Variable(char *name, char *value);
    Variable(Variable* parent, char *name, char *value);
    virtual ~Variable();
    double X;
    double Y;
    double Z;
    Variable* next;
    Variable* prev;
};

#endif // VARIABLE_H
