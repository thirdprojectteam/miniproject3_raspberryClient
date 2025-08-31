#include "tcp_server.h"
#include "mycore/backend.h"
#include <QCoreApplication>
#include <QHostAddress>
#include <QDebug>
#include <QCursor>
#include <QGuiApplication>
#include <QWindow>
#include <QScreen>

TcpServer::TcpServer(quint16 port, QObject* parent)
    : QTcpServer(parent)
{
    if (!listen(QHostAddress::Any, port)) {
        qCritical() << "[TCP] Listen failed on port" << port << ":" << errorString();
        QCoreApplication::exit(1);
    }
    warn = nullptr;

    qInfo() << "[TCP] Listening on tcp://0.0.0.0:" << port;
}

void TcpServer::incomingConnection(qintptr sd) {
    auto* sock = new QTcpSocket(this);
    if (!sock->setSocketDescriptor(sd)) {
        qWarning() << "[TCP] setSocketDescriptor failed:" << sock->errorString();
        sock->deleteLater();
        return;
    }
    qInfo() << "[TCP] Client connected" << sock->peerAddress().toString() << ":" << sock->peerPort();
    clients_.insert(sock);
    //    warn = new QMessageBox(QMessageBox::Warning,"",
    //                           "얼굴이 인식되지 않습니다",QMessageBox::NoButton);
    connect(sock, &QTcpSocket::readyRead, this, [this, sock](){
        while (sock->canReadLine()) {
            const QByteArray line = sock->readLine(); // "1" or "0"
            QString s = QString::fromUtf8(line).trimmed();
            qInfo() << "[TCP] Received:" << line;
            if(!warn){
                warn = new QMessageBox();
                warn->setIcon(QMessageBox::Warning);
                warn->setStandardButtons(QMessageBox::NoButton);
                warn->setText("\n\n\n\n얼굴 인식 불가");
                warn->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
                warn->setStyleSheet("QLabel {font-size: 48pt;qproperty-alignment:AlignCenter;}");

            }
            int mask = -1;
            QString ageLabel = "unknown";
            // 우선 CSV: "1,elder" 포맷 처리
            if(s.contains(',')){
                const QStringList parts = s.split(',', Qt::KeepEmptyParts);
                if (parts.size() >= 1) mask = parts.at(0).trimmed().toInt();
                if (parts.size() >= 2) ageLabel = parts.at(1).trimmed();
            }
            //age label설정
            if(Backend::getInstance().getAge()=="unknown"){
                Backend::getInstance().setAge(ageLabel);
            }
            qDebug()<<ageLabel;

            if(mask == 1){
                qDebug() << "face ok";
                warn->done(0);
            }else{
                qDebug() << "don't see face";
                if(!(warn->isVisible()))
                    warn->open();
                QTimer::singleShot(0,warn,[this]{
                    warn->setFixedSize(1000,700);
                    if(win = QGuiApplication::focusWindow()){
                        screen = win->screen();
                    }
                    if(!screen){
                        screen = QGuiApplication::primaryScreen();
                    }
                    const QRect geom = screen->availableGeometry();
                    const QPoint center = geom.center() - QPoint(warn->width()/2,warn->height()/2);
                    warn->move(center);
                });
            }

        }
    });
    connect(sock, &QTcpSocket::disconnected, this, [this, sock](){
        qInfo() << "[TCP] Client disconnected";
        clients_.remove(sock);
        sock->deleteLater();
    });
}
