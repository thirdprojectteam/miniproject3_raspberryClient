#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "sensorworker.h"
#include "piezo.h"
#include "light.h"
#include "debugui.h"
#include "distance.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    //QThread Sensor
    QThread *sensorThread;
    SensorWorker *worker;
    Piezo *m_piezo;
    //debug light
    light *m_light;

    //UI
    DebugUI *m_debugUI;
};
#endif // MAINWINDOW_H
