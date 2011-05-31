#include "toolmanager.h"
#include "ui_toolmanager.h"

ToolManager::ToolManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ToolManager)
{
    ui->setupUi(this);
}

ToolManager::~ToolManager()
{
    delete ui;
}
