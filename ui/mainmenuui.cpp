#include "mainmenuui.h"
#include "ui_mainmenuui.h"

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
    emit changeRFID(2);
}
//출금
void MainMenuUI::on_WithdrawButton_clicked()
{
    emit changeRFID(3);
}
//조회
void MainMenuUI::on_BalanceButton_clicked()
{
    emit changeRFID(4);
}
//송금
void MainMenuUI::on_SendButton_clicked()
{
    emit changeRFID(5);
}

