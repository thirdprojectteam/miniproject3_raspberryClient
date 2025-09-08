#include "piezo.h"
#include <QDebug>
#include <time.h>
#include <unistd.h>
#include "mycore/webclient.h"

#define piezo_pin 18

static void sleep_ns(long ns) {
    timespec ts;
    ts.tv_sec  = ns / 1000000000L;
    ts.tv_nsec = ns % 1000000000L;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, nullptr);
}

Piezo::Piezo(QObject *parent)
    :QObject(parent),buzzing(false)
{
    m_chip = gpiod_chip_open_by_name("gpiochip0");
    m_piezo = gpiod_chip_get_line(m_chip,piezo_pin);
    ensureRequested();
    gpiod_line_set_value(m_piezo, 0);
}

Piezo::~Piezo(){
    stop();
    releaseLine();
    qDebug()<<"piezo deleted!";
    gpiod_line_request_output(m_piezo,"piezo",0);
    gpiod_line_release(m_piezo);
    gpiod_chip_close(m_chip);
}

void Piezo::ensureRequested() {
    if (!m_piezo) return;
    // 이미 요청된 상태면 skip
    // libgpiod에는 직접 "is_requested" 체크가 없어도, 재요청 시 -EBUSY가 나올 수 있음.
    // 안전하게 한번 Low로 세트 시도
    if (gpiod_line_set_value(m_piezo, 0) < 0) {
        // 아직 요청 전일 수 있으니 요청
        if (gpiod_line_request_output(m_piezo, "piezo", 0) < 0) {
            qWarning() << "[Piezo] line_request_output failed";
        }
    }
}

void Piezo::releaseLine() {
    if (m_piezo) {
        // 출력 0로 바꾸고 해제
        gpiod_line_set_value(m_piezo, 0);
        gpiod_line_release(m_piezo);
        m_piezo = nullptr; // (선택) 다시 사용할 거면 nullptr로 두지 말고 유지
    }
}


void Piezo::turnOnBuzzerOnce(){
    qDebug()<<"entered"<<buzzing;
    stop();             // 다른 톤 재생 중이면 먼저 중지
    ensureRequested();
    if (buzzing) return;
    gpiod_line_set_value(m_piezo, 1);
    buzzing=true;
    // 1000ms 대기 (간단히 usleep)
    QTimer::singleShot(1000,this,[this](){
        gpiod_line_set_value(m_piezo,0);
        gpiod_line_release(m_piezo);
        buzzing=false;
        // 후처리 콜백
        WebClient::getInstance().RequestPost(1);
    });
}

void Piezo::tone(double freq_hz, int duration_ms, int duty_percent) {
    if (freq_hz <= 0 || duration_ms <= 0) return;
    if (duty_percent < 0) duty_percent = 0;
    if (duty_percent > 100) duty_percent = 100;

    stop();             // 기존 워커 정지
    ensureRequested();
    if (!m_piezo) return;

    playing_.store(true);

    // 기존 스레드 종료 후 새 워커 시작
    worker_ = std::thread([=]() {
        // 주기(ns)
        const long period_ns = (long)(1e9 / freq_hz);
        const long on_ns  = (long)(period_ns * (duty_percent / 100.0));
        const long off_ns = period_ns - on_ns;

        // 총 지속시간(ns)
        const long total_ns = (long)duration_ms * 1000000L;

        // 단순 경과 측정
        long elapsed = 0;
        while (playing_.load() && elapsed < total_ns) {
            gpiod_line_set_value(m_piezo, 1);
            sleep_ns(on_ns);
            elapsed += on_ns;
            if (!playing_.load() || elapsed >= total_ns) break;

            gpiod_line_set_value(m_piezo, 0);
            sleep_ns(off_ns);
            elapsed += off_ns;
        }

        // 안전 종료: Low
        gpiod_line_set_value(m_piezo, 0);
        playing_.store(false);
    });

    // 필요시 여기서 detach 말고 join 제어를 위해 stop()에서 join 처리
}

void Piezo::stop() {
    if (playing_.exchange(false)) {
        // 워커가 돌고 있었다면 종료 대기
        if (worker_.joinable()) worker_.join();
    } else {
        // 돌고 있지 않아도 워커 스레드가 남아있을 수 있으니 join 시도
        if (worker_.joinable()) worker_.join();
    }
    // 출력 Low 보장
    if (m_piezo) gpiod_line_set_value(m_piezo, 0);
}



