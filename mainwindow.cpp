#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "light.h"

#include <QScreen>          //for primaryScreenSize

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //resize ui
    QSize screenSize = QGuiApplication::primaryScreen()->availableGeometry().size();
    this->resize(screenSize);
    this->move(0,0);
    //make worker and make qthread -> move to thread
    m_piezo = new Piezo(this);
    worker = new SensorWorker();
    sensorThread = new QThread(this);
    worker->moveToThread(sensorThread);

    //debug always on
    //m_light = new light(this);

    //worker connect delete later, start stop
    connect(sensorThread,&QThread::started,worker,&SensorWorker::start);
    connect(worker,&SensorWorker::sensorValueReady,this,[=](double dist){
        if(dist<0||dist>200){
            qDebug()<<"error distance";
        } else if(dist < 50){
            qDebug()<<"piezo";
            //one second piezo;
            m_piezo->turnOnBuzzerOnce();
        }
        qDebug()<<"distance : "<<dist<<"cm";
    });
    connect(qApp,&QApplication::aboutToQuit,worker,&SensorWorker::stop);
    connect(qApp,&QApplication::aboutToQuit,sensorThread,&QThread::quit);
    connect(sensorThread,&QThread::finished,worker,&QObject::deleteLater);
    connect(sensorThread,&QThread::finished,sensorThread,&QObject::deleteLater);

    //make ui here
    m_MainMenuUI = new MainMenuUI(this);
    m_RFIDUI = new RFIDUI(this);
    m_DepositUI = new DepositUI(this);

    //add ui to stackedWidget
    ui->mainStackedWidget->addWidget(m_MainMenuUI);
    ui->mainStackedWidget->addWidget(m_RFIDUI);
    ui->mainStackedWidget->addWidget(m_DepositUI);

    //connect ChangeWidgetUI
    connect(m_RFIDUI,&RFIDUI::changeWidget,this,&MainWindow::changePageHandler);
    connect(m_DepositUI,&DepositUI::changeWidget,this,&MainWindow::changePageHandler);
    connect(m_MainMenuUI,&MainMenuUI::changeRFID,[=](int idx){
        m_RFIDUI->setidx(idx);
        changePageHandler(1);
    });


    sensorThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changePageHandler(int idx){
    ui->mainStackedWidget->setCurrentIndex(idx);
}
