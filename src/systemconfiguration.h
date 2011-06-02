#ifndef SYSTEMCONFIGURATION_H
#define SYSTEMCONFIGURATION_H

#include <QDialog>
#include <QTreeWidget>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;




class SystemConfiguration : public QDialog
{
    Q_OBJECT
public:
    SystemConfiguration(QWidget *parent = 0);

signals:

public slots:

private:
    QTreeWidget     *settingsList;
    QTreeWidgetItem *analyze;
    QTreeWidgetItem *backplot;
    QTreeWidgetItem *cadsettings;
    QTreeWidgetItem *chaining;
    QTreeWidgetItem *colors;
    QTreeWidgetItem *communications;
    QTreeWidgetItem *converters;
    QTreeWidgetItem *default_machines;
    QTreeWidgetItem *dimensions;
    QTreeWidgetItem *files;
    QTreeWidgetItem *postdialog;
    QTreeWidgetItem *printing;
    QTreeWidgetItem *screen;
    QTreeWidgetItem *shading;
    QTreeWidgetItem *solids;
    QTreeWidgetItem *startup;
    QTreeWidgetItem *tolerances;
};

#endif // SYSTEMCONFIGURATION_H
