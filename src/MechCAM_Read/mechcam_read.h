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

#ifndef MECHCAM_READ_H
#define MECHCAM_READ_H
#include "../MechCAM.h"
#include "../tinyxml/tinyxml.h"



void readMechCAMFile(TiXmlElement*);
void loadArc(TiXmlElement*);
void loadCircle(TiXmlElement*);
void loadLine(TiXmlElement*);
void loadPoint(TiXmlElement*);
void loadVariable(TiXmlElement*);
#endif // MECHCAM_READ_H
