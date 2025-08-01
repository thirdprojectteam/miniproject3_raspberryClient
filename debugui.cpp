#include "debugui.h"
#include "ui_debugui.h"

DebugUI::DebugUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DebugUI)
{
    ui->setupUi(this);
}

DebugUI::~DebugUI()
{
    delete ui;
}

void DebugUI::on_QuitButton_clicked()
{
    //quit
    qApp->quit();
}

