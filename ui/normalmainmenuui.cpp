#include "normalmainmenuui.h"
#include "ui_normalmainmenuui.h"
#include "mycore/backend.h"
#include <QPixmap>
#include <QDir>
#include <QDebug>

NormalMainMenuUI::NormalMainMenuUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NormalMainMenuUI) {
    ui->setupUi(this);

    // 이미지 로드
    loadImages();

    // 타이머 설정 3초
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &NormalMainMenuUI::changeImage);
    timer->start(3000);
}

NormalMainMenuUI::~NormalMainMenuUI() {
    timer->stop();
    delete ui;
}

void NormalMainMenuUI::loadImages() {
    imagePathList << ":/image/ad_1.png"
                  << ":/image/ad_2.png"
                  << ":/image/ad_3.png";

    // 이미지를 스택 위젯에 추가
    for (const QString &path : imagePathList) {
        QLabel *imageLabel = new QLabel();
        QPixmap pixmap(path);
        if (pixmap.isNull()) {
            qDebug() << "광고 이미지 오류" << path;
            imageLabel->setText("Empty");
        } else {
            imageLabel->setPixmap(pixmap.scaled(1000,790,
                                                Qt::IgnoreAspectRatio,
                                                Qt::SmoothTransformation));
            imageLabel->setAlignment(Qt::AlignCenter);
        }
        ui->stackedWidget->addWidget(imageLabel);
    }
}

void NormalMainMenuUI::changeImage() {
    int nextIndex = (ui->stackedWidget->currentIndex() + 1) % ui->stackedWidget->count();
    ui->stackedWidget->setCurrentIndex(nextIndex);
}

void NormalMainMenuUI::on_DepositButton_clicked()
{
    emit selectQRRFID(5);
}

void NormalMainMenuUI::on_WithdrawButton_clicked()
{
    emit selectQRRFID(6);
}

void NormalMainMenuUI::on_BalanceButton_clicked()
{
    emit selectQRRFID(7);
}

void NormalMainMenuUI::on_SendButton_clicked()
{
    emit selectQRRFID(8);
}

void NormalMainMenuUI::on_QuitButton_clicked()
{
    qApp->quit();
}

void NormalMainMenuUI::on_SelectButton_clicked()
{
    //To Elder UI
    Backend::getInstance().setAge("elder");
    emit selectQRRFID(1);
}

void NormalMainMenuUI::on_resetButton_clicked()
{
    Backend::getInstance().setAge("unknown");
}

void NormalMainMenuUI::on_WithdrawButton_3_clicked()
{
    //for debug
    emit piezo_low();
}

void NormalMainMenuUI::on_WithdrawButton_2_clicked()
{
    //for debug
    emit piezo_high();
}

