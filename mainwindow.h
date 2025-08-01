#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//sensor -> 추후 밖으로 처리
#include <QThread>
#include "sensorworker.h"
#include "piezo.h"
#include "light.h"
//UI
#include "mainmenuui.h"
#include "rfidui.h"
#include "depositui.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void changePageHandler(int idx);
private:
    Ui::MainWindow *ui;
    //QThread Sensor
    QThread *sensorThread;
    SensorWorker *worker;
    Piezo *m_piezo;
    //debug light
    light *m_light;

    //UI
    MainMenuUI *m_MainMenuUI;
    RFIDUI *m_RFIDUI;
    DepositUI *m_DepositUI;

};
#endif // MAINWINDOW_H
