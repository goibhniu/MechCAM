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


#include "mechcam_write.h"
#include <MechCAM.h>
#include <iostream>
#include "stdio.h"
#include <strings.h>
extern "C"{
#include <lua5.1/lua.h>
#include <lua5.1/lauxlib.h>
#include <lua5.1/lualib.h>
}

extern MechCAM *foo;

void writeMechCAMFile(TiXmlNode* root)
{
    TiXmlElement* bar;

    char oldlocale[1000];
    strcpy(oldlocale, setlocale(LC_NUMERIC, "C"));

    setlocale(LC_NUMERIC, oldlocale);
}

void writeArc(TiXmlNode* data)
{

}

void writeCircle(TiXmlNode* data)
{


}

void writeLine(TiXmlNode* data)
{


}

void writePoint(TiXmlNode* data)
{


}

void writeVariable(TiXmlNode* data)
{


        TiXmlElement * element;
    	element = new TiXmlElement( "Variable" );
    	data->LinkEndChild( element );

    	element->SetAttribute("name", "stuff");
    	element->SetAttribute("value", "things");


}
