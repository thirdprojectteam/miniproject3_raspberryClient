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
    // if(!WebClient::getInstance().initServer()){
    //     return;
    // }
    //얼굴인식
    // quint16 port = 9000;
    // m_TcpServer = new TcpServer(port);

    //피에조 부처와 점등 만들기
    // m_piezo = new Piezo(this);
    // m_light = new light(this);

    // //초음파는 별개의 스레드로 동작
    // worker = new SensorWorker();
    // sensorThread = new QThread(this);
    // worker->moveToThread(sensorThread);

    //debug always on
    //m_light = new light(this);

    //worker connect delete later, start stop
    // connect(sensorThread,&QThread::started,worker,&SensorWorker::start);
    // connect(worker,&SensorWorker::sensorValueReady,this,[=](double dist){
    //     if(dist<0||dist>200){
    //         qDebug()<<"error distance";
    //     } else if(dist < 50){
    //         qDebug()<<"piezo";
    //         //one second piezo;
    //         m_piezo->turnOnBuzzerOnce();
    //     }
    //     //qDebug()<<"distance : "<<dist<<"cm";
    // });
    // connect(qApp,&QApplication::aboutToQuit,worker,&SensorWorker::stop);
    // connect(qApp,&QApplication::aboutToQuit,sensorThread,&QThread::quit);
    // connect(sensorThread,&QThread::finished,worker,&QObject::deleteLater);
    // connect(sensorThread,&QThread::finished,sensorThread,&QObject::deleteLater);

    //make ui here
    m_MainMenuUI = new MainMenuUI(this);
    m_SelectionUI = new SelectionUI(this);
    m_QRCodeUI = new QRCodeUI(this);
    // m_RFIDUI = new RFIDUI(this);
    // m_DepositUI = new DepositUI(this);
    // m_CheckUI = new CheckUI(this);
    // m_SendUI = new SendUI(this);
    // m_FinishUI = new FinishUI(this);

    //add ui to stackedWidget
    ui->mainStackedWidget->addWidget(m_MainMenuUI);  // 0
    ui->mainStackedWidget->addWidget(m_SelectionUI); // 1
    ui->mainStackedWidget->addWidget(m_QRCodeUI);    // 2
    // ui->mainStackedWidget->addWidget(m_RFIDUI);      // 3
    // ui->mainStackedWidget->addWidget(m_DepositUI);   // 4
    // ui->mainStackedWidget->addWidget(m_CheckUI);     // 5
    // ui->mainStackedWidget->addWidget(m_SendUI);      // 6
    // ui->mainStackedWidget->addWidget(m_FinishUI);    // 7

    //connect ChangeWidgetUI
    // connect(&WebClient::getInstance(),&WebClient::onGetSuccess,this,[=](){
    //     if(ui->mainStackedWidget->currentIndex()==1){
    //         changePageHandler(m_RFIDUI->getidx());
    //         m_light->turnOnLightOnce();
    //     } else {
    //         changePageHandler(6);
    //     }
    // });
    // connect(m_RFIDUI,&RFIDUI::changeWidget,this,&MainWindow::changePageHandler);
    // connect(m_DepositUI,&DepositUI::changeWidget,this,&MainWindow::changePageHandler);
    // connect(m_CheckUI,&CheckUI::changeWidget,this,&MainWindow::changePageHandler);
    // connect(m_SendUI,&SendUI::changeWidget,this,&MainWindow::changePageHandler);
    // connect(m_FinishUI,&FinishUI::changeWidget,this,&MainWindow::changePageHandler);
    
    // QRCodeUI connect
    connect(m_QRCodeUI,&QRCodeUI::changeWidget,this,&MainWindow::changePageHandler);
    // connect(m_MainMenuUI,&MainMenuUI::changeRFID,[=](int idx){
    //     m_RFIDUI->setidx(idx);
    //     m_RFIDUI->rfidThreadStart();
    //     changePageHandler(1);
    // });

    connect(m_MainMenuUI,&MainMenuUI::selectQRRFID,[=](){
        changePageHandler(1);  // SelectionUI로 이동
    });
    
    // SelectionUI에서 QR 또는 RFID 선택 시 처리
    connect(m_SelectionUI,&SelectionUI::qrSelected,[=](){
        // QR 선택 시 처리
        qDebug() << "QR Selected";
        m_QRCodeUI->init();
        changePageHandler(2);  // QRCodeUI로 이동
    });
    
    connect(m_SelectionUI,&SelectionUI::rfidSelected,[=](){
        // RFID 선택 시 처리  
        qDebug() << "RFID Selected";
        changePageHandler(0);  // MainMenuUI로 돌아가기 (RFID UI가 비활성화되어 있으므로)
    });

    // sensorThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changePageHandler(int idx){
    // if(idx==0){
    //     //nothing happen here backend clear all?
    // }else if(idx ==1){
    //     m_RFIDUI->init();
    // }else if(idx ==2){
    //     m_DepositUI->setType(0);
    //     m_DepositUI->init();
    // }else if(idx ==3){
    //     idx=2;
    //     m_DepositUI->setType(1);
    //     m_DepositUI->init();
    // }else if(idx==4){
    //     idx=3;
    //     m_CheckUI->init();
    // }else if(idx==5){
    //     idx=4;
    //     m_SendUI->init();
    // }else if(idx==6){
    //     idx=5;
    //     m_FinishUI->init();
    // }
    
    // SelectionQRRFID 추가
    if(idx==0){
        //nothing happen here backend clear all?
    }else if(idx ==1){
        // SelectionQRRFID page - no init needed
    }else if(idx ==2){
        // QRCodeUI page
    }
    
    ui->mainStackedWidget->setCurrentIndex(idx);
}
