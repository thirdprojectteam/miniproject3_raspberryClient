#include "depositui.h"
#include "ui_depositui.h"

DepositUI::DepositUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DepositUI),DigitRes(0)
{
    ui->setupUi(this);

    //계산기 로직
    connect(ui->num1Button,&QPushButton::clicked,[=](){appendDigit(1);});
    connect(ui->num2Button,&QPushButton::clicked,[=](){appendDigit(2);});
    connect(ui->num3Button,&QPushButton::clicked,[=](){appendDigit(3);});
    connect(ui->num4Button,&QPushButton::clicked,[=](){appendDigit(4);});
    connect(ui->num5Button,&QPushButton::clicked,[=](){appendDigit(5);});
    connect(ui->num6Button,&QPushButton::clicked,[=](){appendDigit(6);});
    connect(ui->num7Button,&QPushButton::clicked,[=](){appendDigit(7);});
    connect(ui->num8Button,&QPushButton::clicked,[=](){appendDigit(8);});
    connect(ui->num9Button,&QPushButton::clicked,[=](){appendDigit(9);});
    connect(ui->num0Button,&QPushButton::clicked,[=](){appendDigit(0);});
    connect(ui->num00Button,&QPushButton::clicked,[=](){appendDigit(0);appendDigit(0);});
    connect(ui->clearButton,&QPushButton::clicked,[=](){clearDigit();});
}

DepositUI::~DepositUI()
{
    delete ui;
}

void DepositUI::on_OKButton_clicked()
{
    emit changeWidget(0);
}

void DepositUI::displayDigit(){
    ui->NumLineEdit->setText(QString::number(DigitRes));
}

void DepositUI::clearDigit(){
    DigitRes=0;
    displayDigit();
}

void DepositUI::appendDigit(int num){
    //초기
    if(DigitRes==0){
        if(num==0){
            return;
        }else{
            DigitRes+=num;
        }
    } else {
        DigitRes*=10;
        DigitRes+=num;
    }
    displayDigit();
}

