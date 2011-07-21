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
#include <GL/gl.h>
#include <math.h>
#include "circledialog.h"

Circle::Circle() {
	center = new Point(0,0,0);
	R = 0;
	I = 0;
	J = 0;
	K = 0;

}

Circle::Circle(const Circle &circle){
	operator=(circle);
}

Circle::Circle(double x, double y, double z, double r, double i, double j,
		double k) {
	center = new Point(x, y, z);
	R = r;
	I = i;
	J = j;
	K = k;

}

Circle::Circle(Circle *parent, double x, double y, double z, double r,
		double i, double j, double k) {
	center = new Point(x, y, z);
	R = r;
	I = i;
	J = j;
	K = k;

}

Circle::~Circle() {

}

void Circle::getDialog(QWidget *parent){
	CircleDialog *dlg = new CircleDialog(parent,this);
	dlg->exec();
}

void Circle::glCommands(bool, bool, bool) {
	float circle_x, circle_y;

	glBegin(GL_LINES);
	for (int foo = 0; foo < 180; foo++) {
		circle_x = R * cos(foo);// - h;
		circle_y = R * sin(foo);// + k;
		glVertex3f(circle_x + center->X, center->Y - circle_y, 0);

		circle_x = R * cos(foo + 0.1);// - h;
		circle_y = R * sin(foo + 0.1);// + k;
		glVertex3f(circle_x + center->X, center->Y - circle_y, 0);
	}
	glEnd();
}


CADObject *Circle::MakeACopy(void)const{
	Circle *new_circle = new Circle(*this);
	return new_circle;
}
