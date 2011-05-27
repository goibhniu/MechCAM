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
#include "caddrawing.h"


MechCAM::MechCAM()
{
    QSplitter* splitter = new QSplitter( this );
    drawing = new CADDrawing(0,this,"");
    statusBar();
    QLabel* l = new QLabel( splitter );
    l->setText( "Hello World!" );
    splitter->addWidget( l );
    splitter->addWidget( drawing );
    setCentralWidget( splitter );
    buildMenu();


}

MechCAM::~MechCAM()
{}

void MechCAM::buildMenu()
{

// File Menu Images
    QPixmap newpix("new.png");
    QPixmap openpix("open.png");
    QPixmap quitpix("quit.png");
// File Menu Actions
    QAction* actionQuit = new QAction(quitpix,"&Quit",this);
    actionQuit->setShortcut(tr("CTRL+Q"));
    actionQuit->setStatusTip(tr("Quit Program"));
    connect(actionQuit, SIGNAL(triggered()), SLOT(close()) );
// Create Menu Actions
    QAction* actionCreatePoint = new QAction(this);
    actionCreatePoint->setText("Change Screen Color");
    connect(actionCreatePoint,SIGNAL(triggered()), SLOT(showCreatePointDialog()));
// Settings Menu Actions
    QAction* actionChangeBGColor = new QAction(this);
    actionChangeBGColor->setText("Change Screen Color");
    connect(actionChangeBGColor,SIGNAL(triggered()), SLOT(showBGColorDialog()));
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
    settingsMenu->addAction(actionChangeBGColor);
// Build Help Menu
    // TODO - Build Help Menu
    helpMenu = menuBar()->addMenu("Help");
}


void MechCAM::showBGColorDialog()
{

      QColorDialog dlg;
  if( dlg.exec() == QDialog::Accepted )
  {
    this->drawing->ClearBlue = dlg.selectedColor().blue();
    this->drawing->ClearGreen = dlg.selectedColor().green();
    this->drawing->ClearRed = dlg.selectedColor().red();
    this->drawing->qglClearColor(QColor(this->drawing->ClearRed,this->drawing->ClearGreen,this->drawing->ClearBlue));

  }


}

void MechCAM::showCreatePointDialog()
{

}


#include "MechCAM.moc"
