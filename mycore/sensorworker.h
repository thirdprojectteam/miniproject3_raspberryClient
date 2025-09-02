#ifndef SENSORWORKER_H
#define SENSORWORKER_H

#include <QObject>
#include <QTimer>
#include <gpiod.h>

class SensorWorker: public QObject
{
    Q_OBJECT
public:
    explicit SensorWorker(QObject *parent = nullptr);
    ~SensorWorker();
public slots:
    void start();
    void stop();
signals:
    void sensorValueReady(double distance);

private slots:
    void readSensor();
private:
     gpiod_chip *m_chip;
     gpiod_line *m_trig;
     gpiod_line *m_echo;
    QTimer *m_timer;
    double measureDistance();

};

#endif // SENSORWORKER_H
