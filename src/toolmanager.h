#ifndef TOOLMANAGER_H
#define TOOLMANAGER_H

#include <QDialog>

namespace Ui {
    class ToolManager;
}

class ToolManager : public QDialog
{
    Q_OBJECT

public:
    explicit ToolManager(QWidget *parent = 0);
    ~ToolManager();

private:
    Ui::ToolManager *ui;
};

#endif // TOOLMANAGER_H
