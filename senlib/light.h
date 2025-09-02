#ifndef LIGHT_H
#define LIGHT_H

#include <QTimer>
#include <QObject>
#include <gpiod.h>

class light:public QObject
{
    Q_OBJECT
public:
    explicit light(QObject *parent = nullptr);
    ~light();
    void turnOnLightOnce();

private:
    bool lighton;
     gpiod_chip *m_chip;
     gpiod_line *m_gpio;
};

#endif // LIGHT_H
