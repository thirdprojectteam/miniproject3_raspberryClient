#include "sendui.h"
#include "ui_sendui.h"

SendUI::SendUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SendUI)
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
    connect(ui->clearButton,&QPushButton::clicked,[=](){clearDigit();});

    connect(ui->eraseButton,&QPushButton::clicked,[=](){eraseDigit();});

    connect(ui->budgetlineEdit,&myLineEdit::clicked,[=](){
        curLineEdit = ui->budgetlineEdit;
    });
    connect(ui->namelineEdit,&myLineEdit::clicked,[=](){
        curLineEdit = ui->namelineEdit;
    });
    connect(ui->UIDlineEdit,&myLineEdit::clicked,[=](){
        curLineEdit = ui->UIDlineEdit;
    });
}

SendUI::~SendUI()
{
    delete ui;
}

void SendUI::init(){
    ui->UIDlineEdit->clear();
    ui->namelineEdit->clear();
    ui->budgetlineEdit->clear();
    curLineEdit=ui->UIDlineEdit;
}

void SendUI::clearDigit(){
    curLineEdit->clear();
}

void SendUI::appendDigit(int num){
    //초기
    curLineEdit->setText(curLineEdit->text()+QString::number(num));
}

void SendUI::eraseDigit(){
    QString curText= curLineEdit->text();
    if(!curText.isEmpty()){
        curText.chop(1);
        curLineEdit->setText(curText);
    }
}
