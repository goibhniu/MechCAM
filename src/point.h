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


#ifndef POINT_H
#define POINT_H

#include "types.h"
#include "CADObject.h"

class Point: public CADObject
{

  public:
    Point();
    Point(double x, double y, double z);
    Point(Point* parent, double x, double y, double z);
    ~Point();
    int GetType()const{return PointType;}
    QColor* GetColor();
    void glCommands(bool select, bool marked, bool no_color);
    const Point& operator=(const Point &CADObj);

    	// virtual functions




 //   	long GetMarkingMask()const{return MARKING_FILTER_POINT;}
 //   	void GetBox(CBox &box);
    	const QChar* GetTypeString(void)const{return QString::fromAscii("Point",5).constData();}
    	CADObject *MakeACopy(void)const;
    	const QBitmap &GetIcon();
    	void ModifyByMatrix(const double *mat);
    	void SetColor(const QColor &col){color = col;}
    	const QColor* GetColor()const{return &color;}
 //   	void GetGripperPositions(std::list<GripData> *list, bool just_for_endof);
 //   	void GetProperties(std::list<Property *> *list);
    	bool GetStartPoint(double* pos);
    	bool GetEndPoint(double* pos);
    	void CopyFrom(const CADObject* object){operator=(*((Point*)object));}
    	void WriteXML(TiXmlNode *root);
    	void LoadFromDoubles();
    	void LoadToDoubles();
    	bool IsDifferent(CADObject* other);
//    	void GetTools(std::list<Tool*>* t_list, const wxPoint* p);

    double X;
    double Y;
    double Z;
  private:
    QColor color;
};

#endif // POINT_H
