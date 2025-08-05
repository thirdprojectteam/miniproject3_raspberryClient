#include "depositui.h"
#include "ui_depositui.h"
#include "backend.h"

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

void DepositUI::init(){
    //초기 설정.
    ui->cusNamelineEdit->setText(Backend::getInstance().getName());
    ui->UIDlineEdit->setText(Backend::getInstance().getUID());
    ui->budgetlineEdit->setText(QString::number(Backend::getInstance().getBudget()));
    ui->expectlineEdit->setText(QString::number(Backend::getInstance().getBudget()));

    if(!m_type)
        ui->StatusLabel->setText("예금입니다. 원하시는 값을 입력해주세요.");
    else
        ui->StatusLabel->setText("출금입니다. 원하시는 값을 입력해주세요.");
    clearDigit();
}

void DepositUI::on_OKButton_clicked()
{
    //원래 여기서 데이터 송수신해서 처리받고 넘어가야됨.
    emit changeWidget(0);
}

void DepositUI::displayDigit(){
    ui->NumLineEdit->setText(QString::number(DigitRes));
    if(m_type==0)ui->expectlineEdit->setText(QString::number(Backend::getInstance().getBudget()+DigitRes));
    else ui->expectlineEdit->setText(QString::number(Backend::getInstance().getBudget()-DigitRes));
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


void DepositUI::on_CancelButton_clicked()
{
    emit changeWidget(0);
}

