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


#include "MechCAM.h"
#include "lua_integration.h"
#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QtGui/QSplitter>
#include <QtGui/QColorDialog>
#include <QGLWidget>
#include <QMessageBox>
#include <QCoreApplication>
#include <QSettings>
#include <stdio.h>
#include "caddrawing.h"
#include "operationsmanager.h"
extern "C"{
#include <lua5.1/lua.h>
#include <lua5.1/lauxlib.h>
#include <lua5.1/lualib.h>
}

extern MechCAM* foo;

extern "C" int l_addPoint(lua_State* luaVM)
{
    double x,y,z;
    x = lua_tonumber(luaVM,-3);
    y = lua_tonumber(luaVM,-2);
    z = lua_tonumber(luaVM,-1);
    foo->drawing->addPoint(x,y,z);
}

int l_addLine(lua_State* luaVM)
{
    double xi,xe,yi,ye,zi,ze;
    xi = lua_tonumber(luaVM,-6);
    yi = lua_tonumber(luaVM,-5);
    zi = lua_tonumber(luaVM,-4);
    xe = lua_tonumber(luaVM,-3);
    ye = lua_tonumber(luaVM,-2);
    ze = lua_tonumber(luaVM,-1);
    foo->drawing->addLine(xi,yi,zi,xe,ye,ze);
}

int l_addCircle(lua_State* luaVM)
{
    double x,y,z,r,i,j,k;
    x = lua_tonumber(luaVM,-7);
    y = lua_tonumber(luaVM,-6);
    z = lua_tonumber(luaVM,-5);
    r = lua_tonumber(luaVM,-4);
    i = lua_tonumber(luaVM,-3);
    j = lua_tonumber(luaVM,-2);
    k = lua_tonumber(luaVM,-1);
    foo->drawing->addCircle(x,y,z,r,i,j,k);
}
