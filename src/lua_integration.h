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

#ifndef LUA_INTEGRATION_H
#define LUA_INTEGRATION_H

extern "C" int l_addPoint(lua_State* luaVM);
extern "C" int l_addLine(lua_State* luaVM);
extern "C" int l_addCircle(lua_State* luaVM);

#endif // LUA_INTEGRATION_H
