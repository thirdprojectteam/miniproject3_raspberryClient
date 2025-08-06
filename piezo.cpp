#include "piezo.h"
#define piezo_pin 17
#include <QDebug>
#include "webclient.h"

Piezo::Piezo(QObject *parent)
    :QObject(parent),buzzing(false)
{
    m_chip = gpiod_chip_open_by_name("gpiochip0");
    m_piezo = gpiod_chip_get_line(m_chip,piezo_pin);
}

Piezo::~Piezo(){
    qDebug()<<"piezo deleted!";
    gpiod_line_request_output(m_piezo,"piezo",0);
    gpiod_line_release(m_piezo);
    gpiod_chip_close(m_chip);
}

void Piezo::turnOnBuzzerOnce(){
    qDebug()<<"entered"<<buzzing;
    if(!buzzing){
        qDebug()<<"turn on buzzer onec";
        gpiod_line_request_output(m_piezo,"piezo",1);
        buzzing=true;
        QTimer::singleShot(1000,this,[this](){
            gpiod_line_set_value(m_piezo,0);
            gpiod_line_release(m_piezo);
            qDebug()<<"singleshot";
            buzzing = false;
            WebClient::getInstance().RequestPost(1);
        });
    }
}



