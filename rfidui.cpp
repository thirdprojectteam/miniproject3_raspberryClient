#include "rfidui.h"
#include "ui_rfidui.h"

RFIDUI::RFIDUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RFIDUI)
{
    ui->setupUi(this);
}

RFIDUI::~RFIDUI()
{
    delete ui;
}

void RFIDUI::on_nextButton_clicked()
{
    //현재는 바로 넘어가지만, 원래는 응답 성공시 처리
    emit changeWidget(m_idx);
}

