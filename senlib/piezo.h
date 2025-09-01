#ifndef PIEZO_H
#define PIEZO_H

#include <QTimer>
#include <QObject>
// #include <gpiod.h>

class Piezo: public QObject
{
    Q_OBJECT
public:
    explicit Piezo(QObject *parent = nullptr);
    ~Piezo();
    void turnOnBuzzerOnce();

private:
    bool buzzing;
    // gpiod_chip *m_chip;
    // gpiod_line *m_piezo;
};

#endif // PIEZO_H
