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
#include "CADObject.h"
#include <GL/gl.h>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>


Point::Point() {
	X = 0;
	Y = 0;
	Z = 0;
}

Point::Point(double x, double y, double z) {

	X = x;
	Y = y;
	Z = z;
}

Point::Point(Point* parent, double x, double y, double z) {

	X = x;
	Y = y;
	Z = z;

}

Point::~Point() {


}

// Color Functions

QColor* Point::GetColor() {
	return MyColor;
}

void Point::SetColor(const QColor &color)
{
	MyColor->operator =(color);
}

void Point::glCommands(bool select, bool marked, bool no_color)
{
	if(!no_color)
	{
		//wxGetApp().glColorEnsuringContrast(color);
		//if (wxGetApp().m_allow_opengl_stippling)
		//{
		//	glEnable(GL_LINE_STIPPLE);
		//	glLineStipple(3, 0xaaaa);
		//}
	}
	GLfloat save_depth_range[2];
	if(marked)
	{
		glGetFloatv(GL_DEPTH_RANGE, save_depth_range);
		glDepthRange(0, 0);
		glLineWidth(2);
	}



		glBegin(GL_POINTS);
		glVertex3d(X, Y, Z);
		glEnd();

	if(marked)
	{
		glLineWidth(1);
		glDepthRange(save_depth_range[0], save_depth_range[1]);
	}
	if(!no_color)
	{
			glDisable(GL_LINE_STIPPLE);
	}
}


//#include "point.moc"
