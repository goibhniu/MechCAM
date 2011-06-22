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

class MechCAM : public QMainWindow
{
Q_OBJECT
public:
    MechCAM();
    virtual ~MechCAM();
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
    CADDrawing* drawing;
    OperationsManager* ops_man;
    SystemConfiguration* sys_config;
    void buildMenu();
private slots:
    void showBGColorDialog();
    void showCreatePointDialog();
    void showCreateVariableDialog();
    void notImplementedMessage();
    void about();
    void showSysConfig();
};

#endif // MECHCAM_H
