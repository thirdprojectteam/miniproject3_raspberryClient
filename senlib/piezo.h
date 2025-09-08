#ifndef PIEZO_H
#define PIEZO_H

#include <QTimer>
#include <QObject>
#include <atomic>
#include <thread>

class Piezo: public QObject
{
    Q_OBJECT
public:
    explicit Piezo(QObject *parent = nullptr);
    ~Piezo();
    void turnOnBuzzerOnce();
    // 주파수 톤 출력: freq(Hz), duration(ms), duty(0~1000, 기본 50.0%)
    void tone(double freq_hz, int duration_ms, int duty_percent = 500);

    // 즉시 정지
    void stop();

private:
    void ensureRequested();
    bool isHardwarePwmPin(int gpio) const;

    bool buzzing;

    int m_gpio = 18;
    bool pigpio_ok_ = false;
    std::atomic_bool playing_{false};
};

#endif // PIEZO_H
