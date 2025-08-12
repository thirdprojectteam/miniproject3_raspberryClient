#ifndef RF522WORKER_H
#define RF522WORKER_H

#include <QObject>
#include <QTimer>
#include <QString>
#include <senlib/MFRC522.h>

class RF522Worker:public QObject
{
    Q_OBJECT
public:
    explicit RF522Worker(QObject *parent = nullptr);
    ~RF522Worker();
public slots:
    void startLoop();
    void endLoop();
//    void writeToCard(QString data);
//    void readFromCard();
    void pollingData();

signals:
//    void cardDetected(QString uid);

    void dataRead(QString uid,QString data);

//    void finished();

//    void requestRead();

//    void requestWrite(QString data);

private:
    bool m_running;
    MFRC522 *m_MFRC522;
    MFRC522::MIFARE_Key key;
    QTimer *pollingTimer;
};

#endif // RF522WORKER_H
