#ifndef OPERATIONSMANAGER_H
#define OPERATIONSMANAGER_H

#include <QWidget>
#include <QTreeWidget>
#include "machine.h"



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

protected:
 //   bool event(QEvent *event);

private slots:
 //   void updateSetting(QTreeWidgetItem *item);

private:
 //   void updateChildItems(QTreeWidgetItem *parent);
 //   QTreeWidgetItem *createItem(const QString &text, QTreeWidgetItem *parent,
 //                               int index);
 //   QTreeWidgetItem *childAt(QTreeWidgetItem *parent, int index);
 //   int childCount(QTreeWidgetItem *parent);
 //   int findChild(QTreeWidgetItem *parent, const QString &text, int startIndex);
 //   void moveItemForward(QTreeWidgetItem *parent, int oldIndex, int newIndex);
    void build_manager();
    //QSettings *settings;
    QTreeWidgetItem *ops;
    QTreeWidgetItem *setup;
    QTreeWidgetItem *stock;
    QTreeWidgetItem *files;
    QTreeWidgetItem *safety;
    QTreeWidgetItem *tools;
    //QTimer refreshTimer;
    bool autoRefresh;
    QIcon groupIcon;
    QIcon keyIcon;
};

#endif // OPERATIONSMANAGER_H
