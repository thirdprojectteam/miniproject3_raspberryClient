#include "mainmenuui.h"
#include "ui_mainmenuui.h"
#include "../mycore/awswebsocketclient.h"
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>

MainMenuUI::MainMenuUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMenuUI)
{
    ui->setupUi(this);
    
    m_awsClient = &AWSWebSocketClient::getInstance();
    
    connect(m_awsClient, &AWSWebSocketClient::connected, 
            this, &MainMenuUI::onAWSConnected);
    connect(m_awsClient, &AWSWebSocketClient::disconnected, 
            this, &MainMenuUI::onAWSDisconnected);
    connect(m_awsClient, &AWSWebSocketClient::errorOccurred, 
            this, &MainMenuUI::onAWSError);
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
    connectToAWS();
    // emit changeRFID(2);
}
//출금
void MainMenuUI::on_WithdrawButton_clicked()
{
    connectToAWS();
    // emit changeRFID(3);
}
//조회
void MainMenuUI::on_BalanceButton_clicked()
{
    connectToAWS();
    // emit changeRFID(4);
}
//송금
void MainMenuUI::on_SendButton_clicked()
{
    connectToAWS();
    // emit changeRFID(5);
}

void MainMenuUI::connectToAWS()
{
    if (!m_awsClient->isConnected()) {
        clientId = QString("raspberry_client_%1").arg(QDateTime::currentMSecsSinceEpoch());
        m_awsClient->setClientId(clientId);
        m_awsClient->connectToAWS();
        qDebug() << "Connecting to AWS server from MainMenu";
    }
}

void MainMenuUI::onAWSConnected()
{
    qDebug() << "MainMenu: Connected to AWS server successfully";
    emit selectQRRFID();
}

void MainMenuUI::onAWSDisconnected()
{
    qDebug() << "MainMenu: Disconnected from AWS server";
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("서버 연결");
    msgBox.setText("서버 연결에 실패했습니다.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setButtonText(QMessageBox::Ok, "확인");

    msgBox.setStyleSheet("QMessageBox { color: black; }"
                         "QMessageBox QLabel { color: black; }");

}

void MainMenuUI::onAWSError(const QString &error)
{
    qDebug() << "MainMenu: AWS connection error:" << error;
}

