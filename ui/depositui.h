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
    //덧셈기 뺄셈기 변경
    void setType(bool type){m_type=type;}
    void init();
    void clearDigit();
    void displayDigit();

signals:
    void changeWidget(int index);

private slots:
    void on_OKButton_clicked();

    void on_CancelButton_clicked();

private:
    Ui::DepositUI *ui;
    long long DigitRes;
    bool m_type;
};

#endif // DEPOSITUI_H
