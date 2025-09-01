#include "rf522worker.h"
#include <QDebug>
#include <unistd.h>

RF522Worker::RF522Worker(QObject *parent)
    :QObject(parent),m_running(true)
{
    m_MFRC522 = new MFRC522(MFRC522::SPI);
    m_MFRC522->PCD_Init();
    for(int i=0; i<6; i++) key.keyByte[i] = 0xFF;
}

RF522Worker::~RF522Worker(){
    m_running = false;
}

void RF522Worker::startLoop(){
    pollingTimer = new QTimer(this);
    connect(pollingTimer,&QTimer::timeout,this,&RF522Worker::pollingData);
    pollingTimer->start(100);
}
void RF522Worker::endLoop(){
    if(pollingTimer->isActive()){
        pollingTimer->stop();
    }
}

void RF522Worker::pollingData(){
    if(!m_MFRC522->PICC_IsNewCardPresent()||!m_MFRC522->PICC_ReadCardSerial()){
        qDebug()<<"failed to Write!";
        return;
    }
    if(m_MFRC522->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,4,&key,&m_MFRC522->uid)!=MFRC522::STATUS_OK){
        qDebug()<<"failed authentication";
        return;
    }
    //UID
    //QString uidStr;
    //for(int i=0; i<m_MFRC522->uid.size; i++){
    //    uidStr+=QString::asprintf("%02X",m_MFRC522->uid.uidByte[i]);
    //}
    //READ account Data
    byte readBuffer[18]={0};
    byte size = sizeof(readBuffer);
    memset(readBuffer,0,sizeof(readBuffer));
    QString Account;
    if(m_MFRC522->MIFARE_Read(5,readBuffer,&size)==MFRC522::STATUS_OK){
        Account = QString::fromUtf8((char*)readBuffer,16).trimmed();
        int nullIndex = Account.indexOf(QChar('\0'));
        if(nullIndex!=-1){
            Account = Account.left(nullIndex);
        }
        qDebug()<<"account 읽기 성공"<<Account;
    }else{
        qDebug()<<"account 읽기 실패";
        m_MFRC522->PCD_StopCrypto1();
        m_MFRC522->PICC_HaltA();
        return;
    }

    //READ Name Data
    memset(readBuffer,0,sizeof(readBuffer));
    if(m_MFRC522->MIFARE_Read(4,readBuffer,&size)==MFRC522::STATUS_OK){
        QString data = QString::fromUtf8((char*)readBuffer,16).trimmed();
        int nullIndex = data.indexOf(QChar('\0'));
        if(nullIndex!=-1){
            data = data.left(nullIndex);
        }
        emit dataRead(Account,data);
        qDebug()<<"읽기 성공"<<data<<" "<<Account;
    }else{
        qDebug()<<"읽기 실패";
    }

    m_MFRC522->PCD_StopCrypto1();
    m_MFRC522->PICC_HaltA();
}
