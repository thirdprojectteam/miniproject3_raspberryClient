#include "selectionqrrfid.h"
#include "ui_selectionqrrfid.h"

SelectionUI::SelectionUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectionUI)
{
    ui->setupUi(this);
}

SelectionUI::~SelectionUI()
{
    delete ui;
}

void SelectionUI::on_qrButton_clicked()
{
    emit qrSelected();
}

void SelectionUI::on_rfidButton_clicked()
{
    emit rfidSelected();
}
