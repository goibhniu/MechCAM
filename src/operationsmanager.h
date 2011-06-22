#ifndef OPERATIONSMANAGER_H
#define OPERATIONSMANAGER_H

#include <QWidget>
#include <QTreeWidget>
#include <QSettings>
#include "machine.h"
#include "stocksetup.h"



class OperationsManager : public QTreeWidget
{
    Q_OBJECT

public:
    OperationsManager(QWidget *parent = 0);

    //void setSettingsObject(QSettings *settings);
 //   QSize sizeHint() const;

public slots:
 //   void setAutoRefresh(bool autoRefresh);
 //   void setFallbacksEnabled(bool enabled);
 //   void maybeRefresh();
 //   void refresh();
    void doSomething(QTreeWidgetItem *item, int column);
    void stockUpdate(double xdim, double ydim, double zdim, double xoff, double yoff, double zoff);

signals:
    void stockUpdated(double xdim, double ydim, double zdim, double xoff, double yoff, double zoff);

protected:
 //   bool event(QEvent *event);

private slots:
 //   void updateSetting(QTreeWidgetItem *item);

private:

    void build_manager();
    void showStockSetup();
    QSettings *settings;
    StockSetup      *stock_setup;
    QTreeWidgetItem *ops;
    QTreeWidgetItem *setup;
    QTreeWidgetItem *stock;
    QTreeWidgetItem *files;
    QTreeWidgetItem *safety;
    QTreeWidgetItem *tools;
    QTreeWidgetItem *vars;
    QTreeWidgetItem *Geometry;
    //QTimer refreshTimer;
    bool autoRefresh;
    QIcon groupIcon;
    QIcon keyIcon;
};

#endif // OPERATIONSMANAGER_H
