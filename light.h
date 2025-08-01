#ifndef LIGHT_H
#define LIGHT_H

#include <gpiod.h>
#include <QObject>
#include <QDebug>

class light:public QObject
{
    Q_OBJECT
public:
    explicit light(QObject *parent = nullptr);
    ~light();
private:
    gpiod_chip *m_chip;
    gpiod_line *m_gpio;
};

#endif // LIGHT_H
