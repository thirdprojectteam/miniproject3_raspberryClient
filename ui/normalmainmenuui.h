#ifndef NORMALMAINMENUUI_H
#define NORMALMAINMENUUI_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QList>

namespace Ui {
class NormalMainMenuUI;
}

class NormalMainMenuUI : public QWidget
{
    Q_OBJECT

public:
    explicit NormalMainMenuUI(QWidget *parent = nullptr);
    ~NormalMainMenuUI();

signals:
    void selectQRRFID(int index);

private slots:
    void changeImage();

    void on_DepositButton_clicked();

    void on_WithdrawButton_clicked();

    void on_BalanceButton_clicked();

    void on_SendButton_clicked();

    void on_QuitButton_clicked();

    void on_SelectButton_clicked();

    void on_resetButton_clicked();

private:
    Ui::NormalMainMenuUI *ui;
    QTimer *timer;
    QList<QString> imagePathList;
    void loadImages();
};

#endif // NORMALMAINMENUUI_H
