#include "awswebsocketclient.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "mycore/backend.h"
#include "mycore/webclient.h"

AWSWebSocketClient::AWSWebSocketClient(QObject *parent)
    : QObject(parent)
    , m_webSocket(new QWebSocket())
    , m_isConnected(false)
    , m_pingTimer(new QTimer(this))
{
    connect(m_webSocket, &QWebSocket::connected, this, &AWSWebSocketClient::onConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &AWSWebSocketClient::onDisconnected);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &AWSWebSocketClient::onTextMessageReceived);
    connect(m_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            this, &AWSWebSocketClient::onError);
    connect(m_webSocket, &QWebSocket::pong, this, &AWSWebSocketClient::onPong);
    
    connect(m_pingTimer, &QTimer::timeout, this, &AWSWebSocketClient::sendPing);
}

AWSWebSocketClient::~AWSWebSocketClient()
{
    disconnectFromAWS();
    delete m_webSocket;
}

void AWSWebSocketClient::connectToAWS(const QString &awsUrl)
{
    if (m_isConnected) {
        qDebug() << "Already connected to AWS";
        return;
    }
    
    m_awsUrl = awsUrl;
    QUrl url(m_awsUrl);
    
    qDebug() << "Connecting to AWS WebSocket:" << url.toString();
    m_webSocket->open(url);
}

void AWSWebSocketClient::disconnectFromAWS()
{
    if (m_webSocket->state() == QAbstractSocket::ConnectedState) {
        m_webSocket->close();
    }
    stopPingTimer();
}

void AWSWebSocketClient::sendQRData(const QString &qrData)
{
    if (!m_isConnected) {
        qWarning() << "Not connected to AWS. Cannot send QR data.";
        emit errorOccurred("Not connected to AWS server");
        return;
    }
    
    QJsonObject message;
    message["type"] = "qr_data";
    message["clientId"] = m_clientId;
    message["data"] = qrData;
    message["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    
    QJsonDocument doc(message);
    QString jsonString = doc.toJson(QJsonDocument::Compact);
    
    qDebug() << "Sending QR data to AWS:" << jsonString;
    m_webSocket->sendTextMessage(jsonString);
}

void AWSWebSocketClient::onConnected()
{
    m_isConnected = true;
    qDebug() << "Connected to AWS WebSocket";
    
    QJsonObject connectMessage;
    connectMessage["type"] = "connect";
    connectMessage["clientId"] = m_clientId;
    connectMessage["deviceType"] = "raspberry_client";
    
    QJsonDocument doc(connectMessage);
    m_webSocket->sendTextMessage(doc.toJson(QJsonDocument::Compact));
    
    startPingTimer();
    emit connected();
}

void AWSWebSocketClient::onDisconnected()
{
    m_isConnected = false;
    qDebug() << "Disconnected from AWS WebSocket";
    stopPingTimer();
    emit disconnected();
    
    setupReconnection();
}

void AWSWebSocketClient::onTextMessageReceived(const QString &message)
{
    qDebug() << "Received message from AWS:" << message;
    
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON received from AWS";
        return;
    }
    
    QJsonObject jsonObj = doc.object();
    QString messageType = jsonObj["type"].toString();
    
    if (messageType == "qr_scan") {
        // Android 앱에서 스캔한 QR 데이터 처리
        QJsonObject data = jsonObj["data"].toObject();
        QString qrCode = data["qr_code"].toString();
        QString userId = data["user_id"].toString();
        QString userPass = data["user_pass"].toString();
        QString transactionId = data["transaction_id"].toString();

        qDebug() << "QR Scan received - QR Code:" << qrCode 
                 << "User ID:" << userId 
                 << "User PASS:" << userPass
                 << "Transaction ID:" << transactionId;
        
        // QR 코드 데이터만 전달 (또는 전체 JSON 전달 가능)
        Backend::getInstance().setUID(userPass);
        Backend::getInstance().setName(userId);
        WebClient::getInstance().RequestPost(0);
        WebClient::getInstance().RequestGet();
        emit qrDataReceived(qrCode);
//        emit messageReceived(jsonObj);
    } else if (messageType == "qr_data") {
        QString qrData = jsonObj["data"].toString();
        emit qrDataReceived(qrData);
    } else if (messageType == "pong") {
        qDebug() << "Received pong from server";
    } else {
        emit messageReceived(jsonObj);
    }
}

void AWSWebSocketClient::onError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    QString errorString = m_webSocket->errorString();
    qWarning() << "WebSocket error:" << errorString;
    emit errorOccurred(errorString);
}

void AWSWebSocketClient::onPong(quint64 elapsedTime, const QByteArray &payload)
{
    Q_UNUSED(payload)
    qDebug() << "Pong received. Latency:" << elapsedTime << "ms";
}

void AWSWebSocketClient::sendPing()
{
    if (m_isConnected) {
        QJsonObject pingMessage;
        pingMessage["type"] = "ping";
        pingMessage["clientId"] = m_clientId;
        
        QJsonDocument doc(pingMessage);
        m_webSocket->sendTextMessage(doc.toJson(QJsonDocument::Compact));
    }
}

void AWSWebSocketClient::setupReconnection()
{
    QTimer::singleShot(5000, this, [this]() {
        if (!m_isConnected && !m_awsUrl.isEmpty()) {
            qDebug() << "Attempting to reconnect to AWS...";
            connectToAWS(m_awsUrl);
        }
    });
}

void AWSWebSocketClient::startPingTimer()
{
    m_pingTimer->start(30000);
}

void AWSWebSocketClient::stopPingTimer()
{
    m_pingTimer->stop();
}
