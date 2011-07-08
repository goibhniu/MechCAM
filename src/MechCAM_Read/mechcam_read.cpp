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


#include "mechcam_read.h"
#include <iostream>
#include "stdio.h"
#include <strings.h>
extern "C"{
#include <lua5.1/lua.h>
#include <lua5.1/lauxlib.h>
#include <lua5.1/lualib.h>
}

extern MechCAM *foo;

void readMechCAMFile(TiXmlElement* root)
{
    TiXmlElement* bar;

    char oldlocale[1000];
    strcpy(oldlocale, setlocale(LC_NUMERIC, "C"));
    for(bar = root->FirstChildElement(); bar;	bar = bar->NextSiblingElement())
    {

        std::string name(bar->Value());
        if( name == "Point" ){ loadPoint(bar); }
        else if( name == "Variable" ) { loadVariable(bar); }
        else if( name == "Line" ) { loadLine(bar); }
        else printf("\n%s\n",bar->Value());
    }
    setlocale(LC_NUMERIC, oldlocale);
}

void loadArc(TiXmlElement* data)
{
    std::string x,y,z;

    // get the attributes
    for(TiXmlAttribute* a = data->FirstAttribute(); a; a = a->Next())
    {
        std::string name(a->Name());
        if(name == "col"){;}
        else if(name == "x"){ x = std::string(a->Value());}
        else if(name == "y"){ y = std::string(a->Value());}
        else if(name == "z"){ z = std::string(a->Value());}
    }

}

void loadCircle(TiXmlElement* data)
{
    std::string x,y,z;

    // get the attributes
    for(TiXmlAttribute* a = data->FirstAttribute(); a; a = a->Next())
    {
        std::string name(a->Name());
        if(name == "col"){;}
        else if(name == "x"){ x = std::string(a->Value());}
        else if(name == "y"){ y = std::string(a->Value());}
        else if(name == "z"){ z = std::string(a->Value());}
    }

}

void loadLine(TiXmlElement* data)
{
    std::string x,y,z;

    // get the attributes
    for(TiXmlAttribute* a = data->FirstAttribute(); a; a = a->Next())
    {
        std::string name(a->Name());
        if(name == "col"){;}
        else if(name == "x"){ x = std::string(a->Value());}
        else if(name == "y"){ y = std::string(a->Value());}
        else if(name == "z"){ z = std::string(a->Value());}
    }

}

void loadPoint(TiXmlElement* data)
{
    std::string x,y,z;

    // get the attributes
    for(TiXmlAttribute* a = data->FirstAttribute(); a; a = a->Next())
    {
        std::string name(a->Name());
        if(name == "col"){;}
        else if(name == "x"){ x = std::string(a->Value());}
        else if(name == "y"){ y = std::string(a->Value());}
        else if(name == "z"){ z = std::string(a->Value());}
    }


    std::string cmdline("addPoint(");
    cmdline.append(x);
    cmdline.append(",");
    cmdline.append(y);
    cmdline.append(",");
    cmdline.append(z);
    cmdline.append(");");
    luaL_dostring(foo->luaVM,cmdline.c_str());


}

void loadVariable(TiXmlElement* data)
{
    std::string id,value;

    // get the attributes
    for(TiXmlAttribute* a = data->FirstAttribute(); a; a = a->Next())
    {
        std::string name(a->Name());
        if(name == "id"){ id = std::string(a->Value());}
        else if(name == "value"){ value = std::string(a->Value());}
    }

    id.append("=");
    id.append(value);
    id.append(";");
    luaL_dostring(foo->luaVM,id.c_str());
}

