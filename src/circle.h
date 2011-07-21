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

#ifndef CIRCLE_H
#define CIRCLE_H

#include "types.h"
#include "point.h"

class Circle: public CADObject {
public:
	Circle();
	Circle(const Circle &circle);
			Circle(double x, double y, double z, double r, double i, double j,
					double k);
	Circle(double x, double y, double z, double r, double i, double j,
			double k, double begin, double end);
	Circle(Circle* parent, double x, double y, double z, double r, double i,
			double j, double k);
	Circle(Circle* parent, double x, double y, double z, double r, double i,
			double j, double k, double begin, double end);
	Point *center;
	virtual ~Circle();
	int GetType() const {
		return CircleType;
	}
	void getDialog(QWidget *parent);
	void glCommands(bool, bool, bool);
	double R;
	double I;
	double J;
	double K;
	CADObject *MakeACopy() const;
};

#endif // CIRCLE_H
