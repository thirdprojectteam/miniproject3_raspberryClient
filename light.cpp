#include "light.h"
#include <QDebug>
#define light_pin 18

light::light(QObject *parent)
    :QObject(parent)
{
    m_chip = gpiod_chip_open_by_name("gpiochip0");
    m_gpio = gpiod_chip_get_line(m_chip,light_pin);
}

light::~light(){
    qDebug()<<"light deleted!";
    gpiod_line_request_output(m_gpio,"gpiotest",0);
    gpiod_line_release(m_gpio);
    gpiod_chip_close(m_chip);
}

void light::turnOnLightOnce(){
    if(!lighton){
        qDebug()<<"turn on light once";
        gpiod_line_request_output(m_gpio,"gpiotest",1);
        lighton=true;
        QTimer::singleShot(3000,this,[this](){
            gpiod_line_set_value(m_gpio,0);
            gpiod_line_release(m_gpio);
            qDebug()<<"singleshot";
            lighton = false;
        });
    }
}
