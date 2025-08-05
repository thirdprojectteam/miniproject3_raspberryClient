#ifndef CHECKUI_H
#define CHECKUI_H

#include <QWidget>

namespace Ui {
class CheckUI;
}

class CheckUI : public QWidget
{
    Q_OBJECT

public:
    explicit CheckUI(QWidget *parent = nullptr);
    ~CheckUI();
    void init();

private slots:
    void on_DepositButton_clicked();

    void on_WithdrawpushButton_clicked();

    void on_SendButton_clicked();

    void on_QuitButton_clicked();
signals:
    void changeWidget(int idx);

private:
    Ui::CheckUI *ui;
};

#endif // CHECKUI_H
