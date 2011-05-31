#include "operationsmanager.h"
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
}


#include "operationsmanager.moc"
