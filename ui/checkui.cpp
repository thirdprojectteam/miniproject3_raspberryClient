#include "checkui.h"
#include "ui_checkui.h"
#include "mycore/backend.h"

CheckUI::CheckUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckUI)
{
    ui->setupUi(this);
}

CheckUI::~CheckUI()
{
    delete ui;
}

void CheckUI::init(){
    ui->namelineEdit->setText(Backend::getInstance().getName());
    ui->UIDlineEdit->setText(Backend::getInstance().getUID());
    ui->budgetlineEdit->setText(QString::number(Backend::getInstance().getBudget()));
}

void CheckUI::on_DepositButton_clicked()
{
    emit changeWidget(2);
}

void CheckUI::on_WithdrawpushButton_clicked()
{
    emit changeWidget(3);
}

void CheckUI::on_SendButton_clicked()
{
    emit changeWidget(5);
}

void CheckUI::on_QuitButton_clicked()
{
    if(Backend::getInstance().getAge()!="elder")
        emit changeWidget(0);
    else
        emit changeWidget(1);
}

