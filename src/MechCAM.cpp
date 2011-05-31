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


#include "MechCAM.h"

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

MechCAM::MechCAM()
{

    QCoreApplication::setOrganizationName("GobhainnSaorArmail");
    QCoreApplication::setOrganizationDomain("gobhainnsaorarmail.com");
    QCoreApplication::setApplicationName("MechCAM");
    QSplitter* splitter = new QSplitter( this );
    drawing = new CADDrawing(0,this,0);
    ops_man = new OperationsManager(splitter);
    statusBar();

 //   QLabel* l = new QLabel( splitter );
 //   l->setText( "Hello World!" );
    splitter->addWidget( ops_man );
    splitter->addWidget( drawing );
    splitter->setStretchFactor(0,1);
    splitter->setStretchFactor(1,8);
    setCentralWidget( splitter );
    buildMenu();

}

MechCAM::~MechCAM()
{
    QSettings settings;
    settings.setValue("mainwindow/size", this->size());
    settings.setValue("mainwindow/fullScreen", this->isFullScreen());
}

void MechCAM::buildMenu()
{

// File Menu Actions
    QAction* actionQuit = new QAction(QIcon::fromTheme("application-exit"),"&Quit",this);
    actionQuit->setShortcut(tr("CTRL+Q"));
    actionQuit->setStatusTip(tr("Quit Program"));
    connect(actionQuit, SIGNAL(triggered()), SLOT(close()) );
// Create Menu Actions
    QAction* actionCreatePoint = new QAction(this);
    actionCreatePoint->setText("Point");
    connect(actionCreatePoint,SIGNAL(triggered()), SLOT(showCreatePointDialog()));
// Transform Menu Actions
    QAction* actionMirror = new QAction(this);
    actionMirror->setText("Mirror");
    //connect(actionChangeBGColor,SIGNAL(triggered()), SLOT(showBGColorDialog()));
    QAction* actionRotate = new QAction(this);
    actionRotate->setText("Rotate");
    connect(actionRotate,SIGNAL(triggered()), SLOT(notImplementedMessage()));
    QAction* actionScale = new QAction(this);
    actionScale->setText("Scale");
    connect(actionScale,SIGNAL(triggered()), SLOT(notImplementedMessage()));
    QAction* actionScaleXYZ = new QAction(this);
    actionScaleXYZ->setText("Scale XYZ");
    connect(actionScaleXYZ,SIGNAL(triggered()), SLOT(notImplementedMessage()));
    QAction* actionTranslate = new QAction(this);
    actionTranslate->setText("Translate");
    connect(actionTranslate,SIGNAL(triggered()), SLOT(notImplementedMessage()));
    QAction* actionOffset = new QAction(this);
    actionOffset->setText("Offset");
    connect(actionOffset,SIGNAL(triggered()), SLOT(notImplementedMessage()));
    QAction* actionContour = new QAction(this);
    actionContour->setText("Contour Offset");
    connect(actionContour,SIGNAL(triggered()), SLOT(notImplementedMessage()));
    QAction* actionGroup = new QAction(this);
    actionGroup->setText("Group");
    connect(actionGroup,SIGNAL(triggered()), SLOT(notImplementedMessage()));
    QAction* actionStretch = new QAction(this);
    actionStretch->setText("Stretch");
    connect(actionStretch,SIGNAL(triggered()), SLOT(notImplementedMessage()));
// Settings Menu Actions
    QAction* actionConfiguration = new QAction(this);
    actionConfiguration->setText("Configuration...");
    connect(actionConfiguration,SIGNAL(triggered()), SLOT(showSysConfig()));
    QAction* actionCustomize = new QAction(this);
    actionCustomize->setText("Customize...");
    connect(actionCustomize,SIGNAL(triggered()), SLOT(notImplementedMessage()));
    QAction* actionKeyMapping = new QAction(this);
    actionKeyMapping->setText("Key Mapping...");
    connect(actionKeyMapping,SIGNAL(triggered()), SLOT(notImplementedMessage()));
    QAction* actionToolBarStates = new QAction(this);
    actionToolBarStates->setText("Toolbar States...");
    connect(actionToolBarStates,SIGNAL(triggered()), SLOT(notImplementedMessage()));
    QAction* actionRunUserApplication = new QAction(this);
    actionRunUserApplication->setText("Run User Application");
    connect(actionRunUserApplication,SIGNAL(triggered()), SLOT(notImplementedMessage()));
    QAction* actionChangeBGColor = new QAction(this);
    actionChangeBGColor->setText("Change Screen Color");
    connect(actionChangeBGColor,SIGNAL(triggered()), SLOT(showBGColorDialog()));
// Help Menu Actions
    QAction* actionAbout = new QAction(this);
    actionAbout->setText(tr("About"));
    connect(actionAbout,SIGNAL(triggered()),SLOT(about()));
// Build File Menu
    fileMenu = menuBar()->addMenu( "File" );
    fileMenu->addAction( actionQuit );
// Build Edit Menu
    // TODO - Build Edit Menu
    editMenu = menuBar()->addMenu("Edit");
// Build View Menu
    // TODO - Build View Menu
    viewMenu = menuBar()->addMenu("View");
// Build Analyze Menu
    // TODO - Build Analyze Menu
    analyzeMenu = menuBar()->addMenu("Analyze");
// Build Create Menu
    // TODO - Build Create Menu
    createMenu = menuBar()->addMenu("Create");
    createMenu->addAction( actionCreatePoint);
// Build Solids Menu
    // TODO - Build Solids Menu
    solidsMenu = menuBar()->addMenu("Solids");
// Build Transform Menu
    // TODO - Build Transform Menu
    transformMenu = menuBar()->addMenu("Transform");
    transformMenu->addAction(actionMirror);
    transformMenu->addAction(actionRotate);
    transformMenu->addAction(actionScale);
    transformMenu->addAction(actionScaleXYZ);
    transformMenu->addAction(actionTranslate);
    transformMenu->addAction(actionOffset);
    transformMenu->addAction(actionContour);
    transformMenu->addAction(actionGroup);
    transformMenu->addAction(actionStretch);
// Build Machine Menu
    // TODO - Build Machine Menu
    machineMenu = menuBar()->addMenu("Machine");
// Build Toolpaths Menu
    // TODO - Build Toolpaths Menu
    toolpathsMenu = menuBar()->addMenu("Toolpaths");
// Build Screen Menu
    // TODO - Build Screen Menu
    screenMenu = menuBar()->addMenu("Screen");
// Build Settings Menu
    // TODO - Build Settings Menu
    settingsMenu = menuBar()->addMenu("Settings");
    settingsMenu->addAction(actionConfiguration);
    settingsMenu->addAction(actionCustomize);
    settingsMenu->addAction(actionKeyMapping);
    settingsMenu->addAction(actionToolBarStates);
    settingsMenu->addAction(actionRunUserApplication);
    settingsMenu->addAction(actionChangeBGColor);
// Build Help Menu
    // TODO - Build Help Menu
    helpMenu = menuBar()->addMenu("Help");
    helpMenu->addAction(actionAbout);
}


void MechCAM::showBGColorDialog()
{
      QSettings settings;
      QColorDialog dlg;
  if( dlg.exec() == QDialog::Accepted )
  {
    this->drawing->ClearBlue = dlg.selectedColor().blue();
    this->drawing->ClearGreen = dlg.selectedColor().green();
    this->drawing->ClearRed = dlg.selectedColor().red();
    this->drawing->qglClearColor(QColor(this->drawing->ClearRed,this->drawing->ClearGreen,this->drawing->ClearBlue));
    settings.setValue("drawingarea/bgcolor", dlg.selectedColor());
  }


}


void MechCAM::notImplementedMessage()
 {
     QMessageBox::StandardButton reply;
     reply = QMessageBox::information(this, tr("Not Yet Implemented"), tr("This Function Not Yet Implemented"));

 }


void MechCAM::showCreatePointDialog()
{

}

void MechCAM::showSysConfig()
{
    sys_config = new SystemConfiguration(this);
    //sys_config->exec();
}

void MechCAM::about()
{
   QMessageBox::about(this, tr("About MechCAM"),
            tr("<b>MechCAM</b> Copyright 2011 - Larry W Housner"));
}

#include "MechCAM.moc"
