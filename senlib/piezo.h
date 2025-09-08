#ifndef PIEZO_H
#define PIEZO_H

#include <QTimer>
#include <QObject>
#include <gpiod.h>
#include <atomic>
#include <thread>

class Piezo: public QObject
{
    Q_OBJECT
public:
    explicit Piezo(QObject *parent = nullptr);
    ~Piezo();
    void turnOnBuzzerOnce();
    // 주파수 톤 출력: freq(Hz), duration(ms), duty(0~100, 기본 50%)
    void tone(double freq_hz, int duration_ms, int duty_percent = 50);

    // 즉시 정지
    void stop();

private:
    void ensureRequested();   // line request 보장
    void releaseLine();       // line release 보장

    bool buzzing;
    gpiod_chip *m_chip;
    gpiod_line *m_piezo;

    int m_gpio = 18;

    std::atomic_bool playing_{false};
    std::thread worker_;
};

#endif // PIEZO_H
