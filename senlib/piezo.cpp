#include "piezo.h"
#include <QDebug>
#include <time.h>
#include <unistd.h>
#include "mycore/webclient.h"
#include <pigpio.h>

static std::atomic_bool g_pigpio_inited{false};

Piezo::Piezo(QObject *parent)
    :QObject(parent),buzzing(false)
{
    ensureRequested();
    if (!pigpio_ok_) {
        qWarning() << "[Piezo] pigpio init failed";
        return;
    }
    // gpiod_line_set_value(m_piezo, 0);
    gpioSetMode(m_gpio,PI_OUTPUT);

    if(isHardwarePwmPin(m_gpio)){
        gpioHardwarePWM(m_gpio,0,0);
    }else {
        gpioSetPWMfrequency(m_gpio,4000);
        gpioPWM(m_gpio,0);
    }
}

Piezo::~Piezo(){
    qDebug()<<"Piezo deleted!";
    stop();
}

void Piezo::ensureRequested() {
    if (!g_pigpio_inited.load()) {
        if (gpioInitialise() >= 0) {
            g_pigpio_inited.store(true);
        }
    }
    pigpio_ok_ = g_pigpio_inited.load();
}

bool Piezo::isHardwarePwmPin(int gpio) const {
    return (gpio == 12 || gpio == 13 || gpio == 18 || gpio == 19);
}

void Piezo::turnOnBuzzerOnce(){
    tone(2000, 1000, 500); // 50% 듀티
    // 필요 시 콜백
    QTimer::singleShot(1000, this, []() {
        WebClient::getInstance().RequestPost(1);
    });
}

void Piezo::tone(double freq_hz, int duration_ms, int duty_percent) {
    qDebug()<<"tone";
    if (!pigpio_ok_ ||freq_hz <= 0 || duration_ms <= 0) return;
    qDebug()<<"tone2";
    if (duty_percent < 0) duty_percent = 0;
    if (duty_percent > 1000) duty_percent = 1000;

    stop();             // 기존 워커 정지
    playing_.store(true);

    if (isHardwarePwmPin(m_gpio)) {
        // 듀티: 0~1,000,000 (ppm)
        gpioHardwarePWM(m_gpio, freq_hz, duty_percent * 1000);
    } else {
        // 일반 PWM(모든 핀) — 가능하면 하드웨어 PWM 핀 사용 추천
        gpioSetPWMfrequency(m_gpio, (int)freq_hz);
        // 0~255 → 비슷한 듀티로 맵핑
        unsigned duty = (255 * duty_percent) / 1000;
        gpioPWM(m_gpio, duty);
    }

    // 비동기 종료 예약 (UI 블로킹 없음)
    QTimer::singleShot(duration_ms, this, [this]() {
        if (!playing_.load()) return;
        stop();
    });
    // 필요시 여기서 detach 말고 join 제어를 위해 stop()에서 join 처리
}

void Piezo::stop() {
    if (!pigpio_ok_) return;
    playing_.store(false);

    if (isHardwarePwmPin(m_gpio)) {
        gpioHardwarePWM(m_gpio, 0, 0);
    } else {
        gpioPWM(m_gpio, 0);
    }
}



