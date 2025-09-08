#include "mainmenuui.h"
#include "ui_mainmenuui.h"
#include "mycore/backend.h"

MainMenuUI::MainMenuUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMenuUI)
{
    ui->setupUi(this);
}

MainMenuUI::~MainMenuUI()
{
    delete ui;
}

void MainMenuUI::on_QuitButton_clicked()
{
    //quit
    qApp->quit();
}

//deposit이든 뭐든 일단 rfid화면으로 가는데
//지정된 idx로 rfid next idx정함
//예금
void MainMenuUI::on_DepositButton_clicked()
{
    emit selectQRRFID(5);
    //emit changeRFID(2);
}
//출금
void MainMenuUI::on_WithdrawButton_clicked()
{
    emit selectQRRFID(6);
    // emit changeRFID(3);
}
//조회
void MainMenuUI::on_BalanceButton_clicked()
{
    emit selectQRRFID(7);
    // emit changeRFID(4);
}
//송금
void MainMenuUI::on_SendButton_clicked()
{
    emit selectQRRFID(8);
    // emit changeRFID(5);
}

//ui change
void MainMenuUI::on_ChangeButton_clicked()
{
    Backend::getInstance().setAge("young");
    emit selectQRRFID(0);
}

