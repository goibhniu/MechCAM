#include "systemconfiguration.h"

SystemConfiguration::SystemConfiguration(QWidget *parent) :
    QDialog(parent)
{
    // setWindowTitle(tr("System Configuration"));
    //resize(800,600);
    settingsList = new QTreeWidget(this);
    analyze = new QTreeWidgetItem(settingsList);
    analyze->setText(0,tr("Properties"));
    backplot = new QTreeWidgetItem(settingsList);
    backplot->setText(0,tr("Properties"));
    cadsettings = new QTreeWidgetItem(settingsList);
    cadsettings->setText(0,tr("Properties"));
    chaining = new QTreeWidgetItem(settingsList);
    chaining->setText(0,tr("Properties"));
    colors = new QTreeWidgetItem(settingsList);
    colors->setText(0,tr("Properties"));
    communications = new QTreeWidgetItem(settingsList);
    communications->setText(0,tr("Properties"));
    converters = new QTreeWidgetItem(settingsList);
    converters->setText(0,tr("Properties"));
    default_machines = new QTreeWidgetItem(settingsList);
    default_machines->setText(0,tr("Properties"));
    dimensions = new QTreeWidgetItem(settingsList);
    dimensions->setText(0,tr("Properties"));
    files = new QTreeWidgetItem(settingsList);
    files->setText(0,tr("Properties"));
    postdialog = new QTreeWidgetItem(settingsList);
    postdialog->setText(0,tr("Properties"));
    printing = new QTreeWidgetItem(settingsList);
    printing->setText(0,tr("Properties"));
    screen = new QTreeWidgetItem(settingsList);
    screen->setText(0,tr("Properties"));
    shading = new QTreeWidgetItem(settingsList);
    shading->setText(0,tr("Properties"));
    solids = new QTreeWidgetItem(settingsList);
    solids->setText(0,tr("Properties"));
    startup = new QTreeWidgetItem(settingsList);
    startup->setText(0,tr("Properties"));
    tolerances = new QTreeWidgetItem(settingsList);
    tolerances->setText(0,tr("Properties"));

}


#include "systemconfiguration.moc"
