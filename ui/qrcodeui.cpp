#include "qrcodeui.h"
#include "ui_qrcodeui.h"
#include <QPainter>
#include <QDateTime>
#include <QDebug>

QRCodeUI::QRCodeUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QRCodeUI)
{
    ui->setupUi(this);
}

QRCodeUI::~QRCodeUI()
{
    delete ui;
}

void QRCodeUI::init()
{
    // QR 코드 데이터 생성 및 표시
    QString qrData = generateSimpleQRData();
    displayQRCode(qrData);
}

QString QRCodeUI::generateSimpleQRData()
{
    // 하드코딩된 QR 데이터
    QString qrData = "123456789";
    return qrData;
}

void QRCodeUI::generateQRCode(const QString &data)
{
    // 실제 QR 코드 생성 로직
    // 여기서는 간단한 시각적 표현을 위해 텍스트와 패턴을 표시
    displayQRCode(data);
}

void QRCodeUI::displayQRCode(const QString &data)
{
    // testqr1.png 이미지 로드
    QPixmap pixmap(":/image/testqr1.png");

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
