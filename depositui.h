#ifndef DEPOSITUI_H
#define DEPOSITUI_H

#include <QWidget>

namespace Ui {
class DepositUI;
}

class DepositUI : public QWidget
{
    Q_OBJECT

public:
    explicit DepositUI(QWidget *parent = nullptr);
    ~DepositUI();
    void appendDigit(int i);
    void clearDigit();
    void displayDigit();

signals:
    void changeWidget(int index);

private slots:
    void on_OKButton_clicked();

private:
    Ui::DepositUI *ui;
    long long DigitRes;
};

#endif // DEPOSITUI_H
