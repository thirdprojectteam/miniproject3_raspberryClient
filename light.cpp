#include "light.h"

light::light(QObject *parent)
    :QObject(parent)
{
    qDebug()<<"light on";
    m_chip = gpiod_chip_open_by_name("gpiochip0");
    m_gpio = gpiod_chip_get_line(m_chip,17);
    gpiod_line_request_output(m_gpio,"gpiotest",1);
    gpiod_line_release(m_gpio);
}

light::~light(){
    qDebug()<<"light off";
    gpiod_line_set_value(m_gpio,0);
    gpiod_line_release(m_gpio);
    gpiod_chip_close(m_chip);
}
