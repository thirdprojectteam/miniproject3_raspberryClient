#include "qrcodeui.h"
#include "ui_qrcodeui.h"
#include "../mycore/awswebsocketclient.h"
#include <QPainter>
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>

#include "QZXing.h"

QRCodeUI::QRCodeUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QRCodeUI)
{
    ui->setupUi(this);
    
    m_awsClient = &AWSWebSocketClient::getInstance();
    
    connect(m_awsClient, &AWSWebSocketClient::connected, 
            this, &QRCodeUI::onAWSConnected);
    connect(m_awsClient, &AWSWebSocketClient::disconnected, 
            this, &QRCodeUI::onAWSDisconnected);
    connect(m_awsClient, &AWSWebSocketClient::qrDataReceived, 
            this, &QRCodeUI::onQRDataReceived);
    connect(m_awsClient, &AWSWebSocketClient::errorOccurred, 
            this, &QRCodeUI::onAWSError);
}

QRCodeUI::~QRCodeUI()
{
    delete ui;
}

void QRCodeUI::init()
{
    // AWS 연결은 MainMenuUI에서 이미 처리됨
    // 클라이언트 ID 가져오기
    if (m_awsClient->isConnected()) {
        clientId = m_awsClient->getClientId();
    } else {
        clientId = QString("raspberry_client_%1").arg(QDateTime::currentMSecsSinceEpoch());
    }

    // QR 코드 데이터 생성 및 표시
    displayQRCode(clientId);
}

void QRCodeUI::displayQRCode(const QString &data)
{
    QImage image = QZXing::encodeData(data,
                                      QZXing::EncoderFormat_QR_CODE,
                                      QSize(300, 300));
    QPixmap pixmap = QPixmap::fromImage(image);

    // testqr1.png 이미지 로드
    // QPixmap pixmap(":/image/testqr1.png");

    // 이미지가 없을 경우 기본 이미지 표시
    if(pixmap.isNull()) {
        qDebug() << "testqr1.png 파일을 찾을 수 없습니다.";
        // 빈 QR 코드 표시
        pixmap = QPixmap(800, 800);
        pixmap.fill(Qt::lightGray);
        QPainter painter(&pixmap);
        painter.setPen(Qt::black);
        painter.drawText(pixmap.rect(), Qt::AlignCenter, "QR Code\nImage Not Found");
    }
    
    // QR 코드 이미지 표시
    ui->qrCodeLabel->setPixmap(pixmap.scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    
}

void QRCodeUI::on_backButton_clicked()
{
    emit changeWidget(0); // MainMenuUI로 돌아가기
}

void QRCodeUI::on_generateButton_clicked()
{
    // 버튼이 제거되었으므로 이 함수는 사용되지 않음
}

// AWS 연결은 이제 MainMenuUI에서 처리됨
// void QRCodeUI::connectToAWS() 함수 제거

void QRCodeUI::onQRDataReceived(const QString &data)
{
    qDebug() << "QR data received from AWS:" << data;
    
    // 확인/취소 메시지 박스 표시
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("QR 코드 수신");
    msgBox.setText("QR 코드가 수신되었습니다.");
    msgBox.setInformativeText(QString("받은 데이터: %1\n\n이 데이터를 처리하시겠습니까?").arg(data));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setButtonText(QMessageBox::Ok, "확인");
    msgBox.setButtonText(QMessageBox::Cancel, "취소");
    msgBox.setIcon(QMessageBox::Information);
    
    msgBox.setStyleSheet("QMessageBox { color: black; }"
                         "QMessageBox QLabel { color: black; }");

    int ret = msgBox.exec();
    
    if (ret == QMessageBox::Ok) {
        // 확인 버튼 클릭 시 처리
        qDebug() << "User confirmed QR data processing";
        
        // QR 데이터 처리
        processQRData(data);
        
        // 처리 완료 메시지 (옵션)
        QMessageBox::information(this, "처리 완료", "QR 코드 데이터가 처리되었습니다.");
    } else {
        // 취소 버튼 클릭 시
        qDebug() << "User cancelled QR data processing";
        QMessageBox::information(this, "처리 취소", "QR 코드 처리가 취소되었습니다.");
    }
}

void QRCodeUI::processQRData(const QString &data)
{
    // QR 데이터 파싱 및 처리
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    
    if (doc.isObject()) {
        QJsonObject obj = doc.object();
        
        // 예시: 사용자 정보나 거래 정보 처리
        if (obj.contains("userId")) {
            QString userId = obj["userId"].toString();
            qDebug() << "User ID from QR:" << userId;
            
            // 필요한 경우 다른 UI로 전환하거나 추가 처리
            // emit changeWidget(다른_UI_인덱스);
        }
        
        if (obj.contains("amount")) {
            double amount = obj["amount"].toDouble();
            qDebug() << "Transaction amount:" << amount;
        }
        
        if (obj.contains("action")) {
            QString action = obj["action"].toString();
            qDebug() << "Action type:" << action;
            
            // 액션에 따른 처리
            if (action == "deposit") {
                // 예금 처리
            } else if (action == "withdraw") {
                // 출금 처리
            } else if (action == "transfer") {
                // 송금 처리
            }
        }
    } else {
        // 단순 문자열 데이터 처리
        qDebug() << "Simple QR data:" << data;
    }
}

void QRCodeUI::onAWSConnected()
{
    qDebug() << "Connected to AWS server successfully";
    
    // 연결 성공 메시지 표시 (옵션)
    // QMessageBox::information(this, "연결 성공", "AWS 서버에 연결되었습니다.");
}

void QRCodeUI::onAWSDisconnected()
{
    qDebug() << "Disconnected from AWS server";
    
    // 연결 해제 메시지 표시 (옵션)
    // QMessageBox::warning(this, "연결 해제", "AWS 서버와의 연결이 해제되었습니다.");
}

void QRCodeUI::onAWSError(const QString &error)
{
    qDebug() << "AWS connection error:" << error;
    
    // 에러 메시지 표시 (옵션)
    // QMessageBox::critical(this, "연결 오류", QString("AWS 서버 연결 오류: %1").arg(error));
}
