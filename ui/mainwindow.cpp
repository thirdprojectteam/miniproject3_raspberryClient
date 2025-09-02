#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mycore/backend.h"
#include "mycore/webclient.h"
#include <QScreen>          //for primaryScreenSize

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //resize ui
    this->setWindowFlags(Qt::FramelessWindowHint);
    QSize screenSize = QGuiApplication::primaryScreen()->availableGeometry().size();
    this->resize(screenSize);
    this->move(0,0);

    //통신 http 시작
    if(!WebClient::getInstance().initServer()){
        return;
    }
    //얼굴인식
    quint16 port = 9000;
    m_TcpServer = new TcpServer(port);

    //피에조 부처와 점등 만들기
    m_piezo = new Piezo(this);
    m_light = new light(this);

    //초음파는 별개의 스레드로 동작
    worker = new SensorWorker();
    sensorThread = new QThread(this);
    worker->moveToThread(sensorThread);

    //debug always on
    m_light = new light(this);

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
        //qDebug()<<"distance : "<<dist<<"cm";
    });
    connect(qApp,&QApplication::aboutToQuit,worker,&SensorWorker::stop);
    connect(qApp,&QApplication::aboutToQuit,sensorThread,&QThread::quit);
    connect(sensorThread,&QThread::finished,worker,&QObject::deleteLater);
    connect(sensorThread,&QThread::finished,sensorThread,&QObject::deleteLater);

    //make ui here
    m_MainMenuUI = new MainMenuUI(this);
    m_SelectionUI = new SelectionUI(this);
    m_QRCodeUI = new QRCodeUI(this);
    m_RFIDUI = new RFIDUI(this);
    m_DepositUI = new DepositUI(this);
    m_CheckUI = new CheckUI(this);
    m_SendUI = new SendUI(this);
    m_FinishUI = new FinishUI(this);
    m_NormalMainMenuUI = new NormalMainMenuUI(this);

    //add ui to stackedWidget
    ui->mainStackedWidget->addWidget(m_NormalMainMenuUI); // 0
    ui->mainStackedWidget->addWidget(m_MainMenuUI);  // 1

    ui->mainStackedWidget->addWidget(m_SelectionUI); // 2
    ui->mainStackedWidget->addWidget(m_QRCodeUI);    // 3
    ui->mainStackedWidget->addWidget(m_RFIDUI);      // 4
    ui->mainStackedWidget->addWidget(m_DepositUI);   // 5
                                                     // 5-2 = minus
    ui->mainStackedWidget->addWidget(m_CheckUI);     // 6
    ui->mainStackedWidget->addWidget(m_SendUI);      // 7
    ui->mainStackedWidget->addWidget(m_FinishUI);    // 8

    //connect ChangeWidgetUI
    connect(&WebClient::getInstance(),&WebClient::onGetSuccess,this,[=](){
        if(ui->mainStackedWidget->currentIndex()== 3||ui->mainStackedWidget->currentIndex()== 4){
            //nxtUI -> dep,send,check
            changePageHandler(Backend::getInstance().nxtUI);
            m_light->turnOnLightOnce();
        } else {
            //finish ui
            changePageHandler(8);
        }
    });
    connect(m_RFIDUI,&RFIDUI::changeWidget,this,&MainWindow::changePageHandler);
    connect(m_DepositUI,&DepositUI::changeWidget,this,&MainWindow::changePageHandler);
    connect(m_CheckUI,&CheckUI::changeWidget,this,&MainWindow::changePageHandler);
    connect(m_SendUI,&SendUI::changeWidget,this,&MainWindow::changePageHandler);
    connect(m_FinishUI,&FinishUI::changeWidget,this,&MainWindow::changePageHandler);
    
    // QRCodeUI connect
    connect(m_QRCodeUI,&QRCodeUI::changeWidget,this,&MainWindow::changePageHandler);
    connect(m_MainMenuUI,&MainMenuUI::changeRFID,[=](int idx){
        m_RFIDUI->setidx(idx);
        m_RFIDUI->rfidThreadStart();
        changePageHandler(1);
    });

    // main menu btn select
    connect(m_MainMenuUI,&MainMenuUI::selectQRRFID,[=](int idx){
        if(idx==0){
            changePageHandler(idx);
        }else{
            Backend::getInstance().nxtUI=idx;
            changePageHandler(2);  // SelectionUI로 이동
        }
    });

    // normal main menu btn select
    connect(m_NormalMainMenuUI,&NormalMainMenuUI::selectQRRFID,[=](int idx){
        if(idx==1){
            changePageHandler(idx);
        }else{
            Backend::getInstance().nxtUI=idx;
            changePageHandler(2);  // SelectionUI로 이동
        }
    });
    
    // SelectionUI에서 QR 또는 RFID 선택 시 처리
    connect(m_SelectionUI,&SelectionUI::qrSelected,[=](){
        // QR 선택 시 처리
        qDebug() << "QR Selected";
        changePageHandler(3);  // QRCodeUI로 이동
    });
    
    connect(m_SelectionUI,&SelectionUI::rfidSelected,[=](){
        // RFID 선택 시 처리  
        qDebug() << "RFID Selected";
        changePageHandler(4);  // RFIDUI
    });

    // sensorThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
//0 normal 1 elder 2 select 3 qrcode 4 rfid 5 depos 6 with 7 check 8 send 9 fin
void MainWindow::changePageHandler(int idx){
    if(idx==0){
        //normal
        //nothing happen here backend clear all?
        Backend::getInstance().nxtUI=0;
     }else if(idx ==1){
        //elder
        //nothing happen here backend clear all?
        Backend::getInstance().nxtUI=0;
     }else if(idx ==2){
        //selection UI

     }else if(idx==3){
        //QRcode UI
         m_QRCodeUI->init();
     }else if(idx ==4){
        //rfid ui
        m_RFIDUI->init();
        m_RFIDUI->rfidThreadStart();
     }else if(idx ==5){
        //deposit ui
        m_DepositUI->setType(0);
        m_DepositUI->init();
     }else if(idx ==6){
        //withdraw ui
        idx--;
        m_DepositUI->setType(1);
        m_DepositUI->init();
     }else if(idx==7){
        //check ui
        idx--;
        m_CheckUI->init();
     }else if(idx==8){
        //send ui
        idx--;
        m_SendUI->init();
     }else if(idx==9){
        //finish ui
        idx--;
        m_FinishUI->init();
     }
    
    ui->mainStackedWidget->setCurrentIndex(idx);
}
