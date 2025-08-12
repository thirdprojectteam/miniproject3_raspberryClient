#include "finishui.h"
#include "ui_finishui.h"
#include "mycore/backend.h"

FinishUI::FinishUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FinishUI)
{
    ui->setupUi(this);
}

FinishUI::~FinishUI()
{
    delete ui;
}

void FinishUI::init(){
    ui->budgetlineEdit->setText(QString::number(Backend::getInstance().getBudget()));
    ui->UIDlineEdit->setText(Backend::getInstance().getUID());
    ui->namelineEdit->setText(Backend::getInstance().getName());
}

void FinishUI::on_OKButton_clicked()
{
    emit changeWidget(0);
}

