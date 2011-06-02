#include "operationsmanager.h"
#include "stocksetup.h"
#include <stdio.h>
#include <QTreeWidget>
#include <QTreeWidgetItem>

OperationsManager::OperationsManager(QWidget* parent): QTreeWidget(parent)
{

    build_manager();

}


void OperationsManager::build_manager()
{
// construct operations manager
//    treewidget = new QTreeWidget(this);
// add widget for machine and stock setup
    setHeaderHidden(true);
    setup = new QTreeWidgetItem(this);
    setup->setText(0,tr("Properties"));
    files = new QTreeWidgetItem(setup);
    files->setText(0,tr("Files"));
    tools = new QTreeWidgetItem(setup);
    tools->setText(0,tr("Tool Settings"));
    stock = new QTreeWidgetItem(setup);
    stock->setText(0,tr("Stock Setup"));
    safety = new QTreeWidgetItem(setup);
    safety->setText(0,tr("Safety Zone"));
    ops = new QTreeWidgetItem(this);
    ops->setText(0,tr("Toolpaths"));
    stock_setup = new StockSetup(this);
    connect( this, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
                 this, SLOT(doSomething(QTreeWidgetItem*, int)));
    connect(stock_setup,SIGNAL(stockChanged(double,double,double,double,double,double)),this,SLOT(stockUpdate(double,double,double,double,double,double)));

}
void OperationsManager::doSomething(QTreeWidgetItem *item, int column)
{
    if ( item == stock ) {
        showStockSetup();
    }
}

void OperationsManager::stockUpdate(double xdim, double ydim, double zdim, double xoff, double yoff, double zoff)
{
    emit stockUpdated(xdim,ydim,zdim,xoff,yoff,zoff);
}

void OperationsManager::showStockSetup()
{
    stock_setup->show();
}


#include "operationsmanager.moc"
