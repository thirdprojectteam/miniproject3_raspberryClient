#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QSet>
#include <QMessageBox>
#include <QTimer>

class TcpServer : public QTcpServer {
    Q_OBJECT
public:
    explicit TcpServer(quint16 port, QObject* parent=nullptr);
protected:
    void incomingConnection(qintptr socketDescriptor) override;

signals:
    void SwitchUI();
    void PiezoOnce();
private:
    QSet<QTcpSocket*> clients_;
    QMessageBox *warn;
    QScreen *screen = nullptr;
    QWindow *win = nullptr;
};

class MMessageBox : public QMessageBox{
    Q_OBJECT
public:
    using QMessageBox::QMessageBox;
protected:
    void resizeEvent(QResizeEvent *e) override{
        QMessageBox::resizeEvent(e);
        this->setFixedSize(1000,700);
    };
};

#endif // TCP_SERVER_H
