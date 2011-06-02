#include "systemconfiguration.h"

SystemConfiguration::SystemConfiguration(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("MechCAM - System Configuration"));
    //resize(800,600);
    settingsList = new QTreeWidget(this);
    settingsList->setHeaderHidden(true);
    analyze = new QTreeWidgetItem(settingsList);
    analyze->setText(0,tr("Analyze"));
    backplot = new QTreeWidgetItem(settingsList);
    backplot->setText(0,tr("Backplot"));
    cadsettings = new QTreeWidgetItem(settingsList);
    cadsettings->setText(0,tr("CAD Settings"));
    chaining = new QTreeWidgetItem(settingsList);
    chaining->setText(0,tr("Chaining"));
    colors = new QTreeWidgetItem(settingsList);
    colors->setText(0,tr("Colors"));
    communications = new QTreeWidgetItem(settingsList);
    communications->setText(0,tr("Communications"));
    converters = new QTreeWidgetItem(settingsList);
    converters->setText(0,tr("Converters"));
    default_machines = new QTreeWidgetItem(settingsList);
    default_machines->setText(0,tr("Default Machines"));
    dimensions = new QTreeWidgetItem(settingsList);
    dimensions->setText(0,tr("Dimensions"));
    files = new QTreeWidgetItem(settingsList);
    files->setText(0,tr("Files"));
    postdialog = new QTreeWidgetItem(settingsList);
    postdialog->setText(0,tr("Post Dialog"));
    printing = new QTreeWidgetItem(settingsList);
    printing->setText(0,tr("Printing"));
    screen = new QTreeWidgetItem(settingsList);
    screen->setText(0,tr("Screen"));
    shading = new QTreeWidgetItem(settingsList);
    shading->setText(0,tr("Shading"));
    solids = new QTreeWidgetItem(settingsList);
    solids->setText(0,tr("Solids"));
    startup = new QTreeWidgetItem(settingsList);
    startup->setText(0,tr("Startup"));
    tolerances = new QTreeWidgetItem(settingsList);
    tolerances->setText(0,tr("Tolerances"));

}


#include "systemconfiguration.moc"
