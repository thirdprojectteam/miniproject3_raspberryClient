#include "rfidui.h"
#include "ui_rfidui.h"
#include "backend.h"
#include "webclient.h"

RFIDUI::RFIDUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RFIDUI)
{
    ui->setupUi(this);
    m_rfWorker = new RF522Worker();
    rfidThread = new QThread(this);

    m_rfWorker->moveToThread(rfidThread);

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

void RFIDUI::init(){
    ui->UIDlineEdit->clear();
    ui->DatalineEdit->clear();
    Backend::getInstance().setBudget(0);
    Backend::getInstance().setName(QString());
    Backend::getInstance().setUID(QString());
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
    Backend::getInstance().setUID(ui->UIDlineEdit->text());
    Backend::getInstance().setName(ui->DatalineEdit->text());
    rfidThreadEnd();
    //여기서 api send해서 receive받을듯.
    WebClient::getInstance().RequestGet();
}

void RFIDUI::on_retryButton_clicked()
{
    //초기화
    ui->UIDlineEdit->clear();
    ui->DatalineEdit->clear();
    Backend::getInstance().setName(QString());
    Backend::getInstance().setUID(QString());
    //재시도
    if(!rfidThread->isRunning()){
        rfidThread->start();
    }
}

void RFIDUI::on_cancelButton_clicked()
{
    //초기화
    ui->UIDlineEdit->clear();
    ui->DatalineEdit->clear();
    Backend::getInstance().setName(QString());
    Backend::getInstance().setUID(QString());
    //끄기
    if(rfidThread->isRunning()){
        rfidThread->quit();
    }
    //돌아가기
    emit changeWidget(0);
}

