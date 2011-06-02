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

#include "stocksetup.h"
#include <QPushButton>
#include <QFormLayout>

StockSetup::StockSetup(QWidget *parent) : QDialog(parent)
{
    // set some reasonable default values for the stock
    x_orig      = 0;
    y_orig      = 0;
    z_orig      = 0;
    xdim        = 1;
    ydim        = 1;
    zdim        = 0.125;
    // build stock setup window
    hlay1 = new QGridLayout(this);
    groupbox = new QGroupBox(tr("Stock Origin"),this);
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
    StockGroupBox = new QGroupBox(tr("Stock Size"),this);
    offset_Stock = new QFormLayout(StockGroupBox);
    labelxdim = new QLabel(tr("X Dimension:"),this,0);
    labelydim = new QLabel(tr("Y Dimension:"),this,0);
    labelzdim = new QLabel(tr("Z Dimension:"),this,0);
    editxdim = new QLineEdit(this);
    editxdim->setText(QString::number(xdim,'f',-1));
    editydim = new QLineEdit(this);
    editydim->setText(QString::number(ydim,'f',-1));
    editzdim = new QLineEdit(this);
    editzdim->setText(QString::number(zdim,'f',-1));
    offset_Stock->addRow(labelxdim,editxdim);
    offset_Stock->addRow(labelydim,editydim);
    offset_Stock->addRow(labelzdim,editzdim);

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

void StockSetup::new_dimensions_accepted()
{
    xdim = editxdim->text().toDouble();
    ydim = editydim->text().toDouble();
    zdim = editzdim->text().toDouble();
    x_orig = editxoff->text().toDouble();
    y_orig = edityoff->text().toDouble();
    z_orig = editzoff->text().toDouble();
    emit stockChanged(xdim,ydim,zdim,x_orig,y_orig,z_orig);
    close();
}

double StockSetup::getStockXOrigin()
{
    return x_orig;
}

double StockSetup::getStockYOrigin()
{
    return y_orig;
}

double StockSetup::getStockZOrigin()
{
    return z_orig;
}

double StockSetup::getStockWidth()
{
    return ydim;
}

double StockSetup::getStockLength()
{
    return xdim;
}
double StockSetup::getStockThickness()
{
    return zdim;
}

#include "stocksetup.moc"
