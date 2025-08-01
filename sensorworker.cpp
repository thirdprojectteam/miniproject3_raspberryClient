#include "sensorworker.h"

#include <chrono>
#include <thread>   //this_thread::sleep_for -> us delay
#include <QDebug>

#define TRIG_PIN 22
#define ECHO_PIN 23

SensorWorker::SensorWorker(QObject *parent)
    :QObject(parent),m_timer(new QTimer(this))
{
    //timeout -> readSensor
    connect(m_timer,&QTimer::timeout,this,&SensorWorker::readSensor);

    //setting gpiod chip open;
    m_chip = gpiod_chip_open_by_name("gpiochip0");
    m_trig = gpiod_chip_get_line(m_chip,TRIG_PIN);
    m_echo = gpiod_chip_get_line(m_chip,ECHO_PIN);
}

SensorWorker::~SensorWorker(){
    qDebug()<<"sensorWorker deleted!";
    stop();
    gpiod_line_set_value(m_trig,0);
    gpiod_chip_close(m_chip);
}

void SensorWorker::start(){
    m_timer->start(100);
}

void SensorWorker::stop(){
    m_timer->stop();
}
void SensorWorker::readSensor(){
    double dist = measureDistance();
    emit sensorValueReady(dist);
}

double SensorWorker::measureDistance(){
    using namespace std::chrono;
    //gpiod output input setting
    gpiod_line_request_output(m_trig, "ultra",0);
    gpiod_line_request_input(m_echo,"ultra");

    //0->1->0
    gpiod_line_set_value(m_trig,0);
    std::this_thread::sleep_for(microseconds(2));
    gpiod_line_set_value(m_trig,1);
    std::this_thread::sleep_for(microseconds(10));
    gpiod_line_set_value(m_trig,0);

    //echo value
    auto start = high_resolution_clock::now();
    while(gpiod_line_get_value(m_echo)==0)
        start = high_resolution_clock::now();

    auto stop = high_resolution_clock::now();
    while(gpiod_line_get_value(m_echo)==1)
        stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop-start).count();
    //calculation /2 for go and back
    return duration*0.034/2.0;
}
