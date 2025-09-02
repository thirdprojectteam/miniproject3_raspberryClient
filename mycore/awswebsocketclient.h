#ifndef AWSWEBSOCKETCLIENT_H
#define AWSWEBSOCKETCLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>
#include <QTimer>

class AWSWebSocketClient : public QObject
{
    Q_OBJECT
public:
    static AWSWebSocketClient &getInstance() {
        static AWSWebSocketClient instance;
        return instance;
    }

    void connectToAWS(const QString &awsUrl = "ws://43.220.4.130:3000/qt-websocket");
    void disconnectFromAWS();
    void sendQRData(const QString &qrData);
    bool isConnected() const { return m_isConnected; }
    void setClientId(const QString &clientId) { m_clientId = clientId; }
    QString getClientId() const { return m_clientId; }

signals:
    void connected();
    void disconnected();
    void qrDataReceived(const QString &data);
    void errorOccurred(const QString &error);
    void messageReceived(const QJsonObject &message);

private slots:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(const QString &message);
    void onError(QAbstractSocket::SocketError error);
    void onPong(quint64 elapsedTime, const QByteArray &payload);
    void sendPing();

private:
    AWSWebSocketClient(QObject *parent = nullptr);
    ~AWSWebSocketClient();
    AWSWebSocketClient(const AWSWebSocketClient&) = delete;
    AWSWebSocketClient& operator=(const AWSWebSocketClient&) = delete;

    QWebSocket *m_webSocket;
    QString m_awsUrl;
    QString m_clientId;
    bool m_isConnected;
    QTimer *m_pingTimer;
    
    void setupReconnection();
    void startPingTimer();
    void stopPingTimer();
};

#endif // AWSWEBSOCKETCLIENT_H