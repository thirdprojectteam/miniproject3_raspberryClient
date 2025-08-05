#ifndef MAINMENUUI_H
#define MAINMENUUI_H

#include <QWidget>
//한글 된다 이제

namespace Ui {
class MainMenuUI;
}

class MainMenuUI : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenuUI(QWidget *parent = nullptr);
    ~MainMenuUI();
signals:
    void changeRFID(int index);

private slots:
    void on_QuitButton_clicked();

    void on_DepositButton_clicked();

    void on_WithdrawButton_clicked();

    void on_BalanceButton_clicked();

    void on_SendButton_clicked();

private:
    Ui::MainMenuUI *ui;
};

#endif // MAINMENUUI_H
