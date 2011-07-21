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


#ifndef MECHCAM_H
#define MECHCAM_H

#include <QtGui/QMainWindow>
#include <qmenu.h>
#include "caddrawing.h"
#include "operationsmanager.h"
#include "systemconfiguration.h"
#include "Heeks_Read/heeks_reader.h"
#include "MechCAM_Read/mechcam_read.h"
extern "C"{
#include <lua5.1/lua.h>
#include <lua5.1/lauxlib.h>
#include <lua5.1/lualib.h>
}
class MechCAM : public QMainWindow
{
Q_OBJECT
public:
    MechCAM();
    virtual ~MechCAM();
    CADDrawing* drawing;
    OperationsManager* ops_man;
    SystemConfiguration* sys_config;
    lua_State* luaVM;
private:
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *analyzeMenu;
    QMenu *createMenu;
    QMenu *solidsMenu;
    QMenu *transformMenu;
    QMenu *machineMenu;
    QMenu *toolpathsMenu;
    QMenu *screenMenu;
    QMenu *settingsMenu;
    QMenu *helpMenu;
    QLabel * m_statusLeft;
    QLabel * m_statusMiddle;
    QStatusBar *status;
    void buildMenu();
    void openXMLFile(const char *);
private slots:
    void setLocation(double x, double y, double z);
    void showBGColorDialog();
    void showCreateCircleDialog();
    void showCreatePointDialog();
    void showCreateVariableDialog();
    void notImplementedMessage();
    void about();
    void showSysConfig();
    void fileOpen();
};

#endif // MECHCAM_H
