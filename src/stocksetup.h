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

#ifndef STOCKSETUP_H
#define STOCKSETUP_H

#include <QDialog>
#include <QPushButton>
#include <QLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>

class StockSetup : public QDialog
{
    Q_OBJECT

public:
    StockSetup(QWidget *parent = 0);
    double getStockWidth();
    double getStockLength();
    double getStockThickness();
    double getStockXOrigin();
    double getStockYOrigin();
    double getStockZOrigin();
signals:
    void stockChanged(double x_dim, double y_dim, double z_dim, double x_off, double y_off, double z_off);
public slots:
    void new_dimensions_accepted();
private:
    QPushButton *button_ok;
    QPushButton *button_cancel;
    QGridLayout *hlay1;
    QFormLayout *offset_form;
    QGroupBox   *groupbox;
    QLabel      *labelxoff;
    QLabel      *labelyoff;
    QLabel      *labelzoff;
    QLineEdit   *editxoff;
    QLineEdit   *edityoff;
    QLineEdit   *editzoff;
    QFormLayout *offset_Stock;
    QGroupBox   *StockGroupBox;
    QLabel      *labelxdim;
    QLabel      *labelydim;
    QLabel      *labelzdim;
    QLineEdit   *editxdim;
    QLineEdit   *editydim;
    QLineEdit   *editzdim;
    double      x_orig;
    double      y_orig;
    double      z_orig;
    double      xdim;
    double      ydim;
    double      zdim;
};

#endif // STOCKSETUP_H
