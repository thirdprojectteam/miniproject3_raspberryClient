#include "rfidui.h"
#include "ui_rfidui.h"

RFIDUI::RFIDUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RFIDUI)
{
    ui->setupUi(this);
    m_rfWorker = new RF522Worker();
    rfidThread = new QThread(this);

    m_rfWorker->moveToThread(rfidThread);

    //connect(m_rfWorker,&RF522Worker::requestRead,m_rfWorker,&RF522Worker::readFromCard,Qt::QueuedConnection);
    //QThread started -> rfWorker 동작해서 읽힐때까지 읽는다.
    connect(rfidThread,&QThread::started,m_rfWorker,&RF522Worker::startLoop);
    //QThread finished -> QThread 끝냈으면 불린다.
    connect(rfidThread,&QThread::finished,m_rfWorker,&RF522Worker::endLoop);
    //data read 성공시 불린다
    connect(m_rfWorker,&RF522Worker::dataRead,this,&RFIDUI::onDataRead);
}

RFIDUI::~RFIDUI()
{
    delete ui;
}

void RFIDUI::rfidThreadStart(){
    rfidThread->start();
    qDebug()<<"rfid started";
}
void RFIDUI::rfidThreadEnd(){
    rfidThread->quit();
    qDebug()<<"rfid quit";
}

void RFIDUI::onDataRead(QString uid,QString data){
    ui->UIDlineEdit->setText(uid);
    ui->DatalineEdit->setText(data);
    qDebug()<<uid;
    qDebug()<<data;
    rfidThreadEnd();
}

void RFIDUI::on_nextButton_clicked()
{
    //현재는 바로 넘어가지만, 원래는 응답 성공시 처리
    emit changeWidget(m_idx);
}

void RFIDUI::on_retryButton_clicked()
{
    if(!rfidThread->isRunning()){
        rfidThread->start();
    }
}

void RFIDUI::on_cancelButton_clicked()
{
    if(rfidThread->isRunning()){
        rfidThread->quit();
    }
    emit changeWidget(0);
}

