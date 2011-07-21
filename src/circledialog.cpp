/*
 * 					circledialog.cpp is part of MechCAM.

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
 *
 *  Created on: Jul 20, 2011
 *      Author: lhousner
 */

#include "circledialog.h"

CircleDialog::CircleDialog(QWidget *parent) {
	// set some reasonable default values for the stock
	    x_orig      = 0;
	    y_orig      = 0;
	    z_orig      = 0;
	    radius      = 1;
	    // build stock setup window
	    hlay1 = new QGridLayout(this);
	    groupbox = new QGroupBox(tr("Circle Center"),this);
	    offset_form = new QFormLayout(groupbox);
	    labelxoff = new QLabel(tr("X:"),this,0);
	    labelyoff = new QLabel(tr("Y:"),this,0);
	    labelzoff = new QLabel(tr("Z:"),this,0);
	    editxoff = new QLineEdit(this);
	    editxoff->setText(QString::number(x_orig,'f',-1));
	    edityoff = new QLineEdit(this);
	    edityoff->setText(QString::number(y_orig,'f',-1));
	    editzoff = new QLineEdit(this);
	    editzoff->setText(QString::number(z_orig,'f',-1));
	    offset_form->addRow(labelxoff,editxoff);
	    offset_form->addRow(labelyoff,edityoff);
	    offset_form->addRow(labelzoff,editzoff);
	    StockGroupBox = new QGroupBox(tr("Circle Size"),this);
	    offset_Stock = new QFormLayout(StockGroupBox);
	    labelradius = new QLabel(tr("Radius:"),this,0);

	    editradius = new QLineEdit(this);
	    editradius->setText(QString::number(radius,'f',-1));
	    offset_Stock->addRow(labelradius,editradius);


	    button_ok = new QPushButton(tr("&OK"),this);
	    button_cancel = new QPushButton(tr("&Cancel"),this);
	    hlay1->addWidget(StockGroupBox,1,1,3,3,Qt::AlignCenter);
	    hlay1->addWidget(groupbox,4,1,3,2,Qt::AlignCenter);
	    hlay1->addWidget(button_ok,5,3,1,1,Qt::AlignCenter);
	    hlay1->addWidget(button_cancel,6,3,1,1,Qt::AlignCenter);
	    this->setLayout(hlay1);
	    connect(button_cancel,SIGNAL(clicked()),SLOT(close()));
	    connect(button_ok,SIGNAL(clicked()),SLOT(new_dimensions_accepted()));

}

CircleDialog::CircleDialog(QWidget *parent, Circle *circle){
	// set some reasonable default values for the stock
		circobj = circle;
	    // build stock setup window
	    hlay1 = new QGridLayout(this);
	    groupbox = new QGroupBox(tr("Circle Center"),this);
	    offset_form = new QFormLayout(groupbox);
	    labelxoff = new QLabel(tr("X:"),this,0);
	    labelyoff = new QLabel(tr("Y:"),this,0);
	    labelzoff = new QLabel(tr("Z:"),this,0);
	    editxoff = new QLineEdit(this);
	    editxoff->setText(QString::number(x_orig,'f',-1));
	    edityoff = new QLineEdit(this);
	    edityoff->setText(QString::number(y_orig,'f',-1));
	    editzoff = new QLineEdit(this);
	    editzoff->setText(QString::number(z_orig,'f',-1));
	    offset_form->addRow(labelxoff,editxoff);
	    offset_form->addRow(labelyoff,edityoff);
	    offset_form->addRow(labelzoff,editzoff);
	    StockGroupBox = new QGroupBox(tr("Circle Size"),this);
	    offset_Stock = new QFormLayout(StockGroupBox);
	    labelradius = new QLabel(tr("Radius:"),this,0);

	    editradius = new QLineEdit(this);
	    editradius->setText(QString::number(radius,'f',-1));
	    offset_Stock->addRow(labelradius,editradius);


	    button_ok = new QPushButton(tr("&OK"),this);
	    button_cancel = new QPushButton(tr("&Cancel"),this);
	    hlay1->addWidget(StockGroupBox,1,1,3,3,Qt::AlignCenter);
	    hlay1->addWidget(groupbox,4,1,3,2,Qt::AlignCenter);
	    hlay1->addWidget(button_ok,5,3,1,1,Qt::AlignCenter);
	    hlay1->addWidget(button_cancel,6,3,1,1,Qt::AlignCenter);
	    this->setLayout(hlay1);
	    connect(button_cancel,SIGNAL(clicked()),SLOT(close()));
	    connect(button_ok,SIGNAL(clicked()),SLOT(new_dimensions_accepted()));

}

CircleDialog::~CircleDialog() {
	// TODO Auto-generated destructor stub
}



void CircleDialog::new_dimensions_accepted()
{
    circobj->R = editradius->text().toDouble();
    circobj->center->X = editxoff->text().toDouble();
    circobj->center->Y = edityoff->text().toDouble();
    circobj->center->Z = editzoff->text().toDouble();
    //emit stockChanged(xdim,ydim,zdim,x_orig,y_orig,z_orig);
    accept();
}



#include "circledialog.moc"
