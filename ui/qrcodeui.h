#ifndef QRCODEUI_H
#define QRCODEUI_H

#include <QWidget>
#include <QPixmap>
#include <QString>

namespace Ui {
class QRCodeUI;
}

class AWSWebSocketClient;

class QRCodeUI : public QWidget
{
    Q_OBJECT

public:
    explicit QRCodeUI(QWidget *parent = nullptr);
    ~QRCodeUI();
    
    void init();

signals:
    void changeWidget(int idx);

private slots:
    void on_backButton_clicked();
    void on_generateButton_clicked();
    void onQRDataReceived(const QString &data);
    void onAWSConnected();
    void onAWSDisconnected();
    void onAWSError(const QString &error);

private:
    Ui::QRCodeUI *ui;
    AWSWebSocketClient *m_awsClient;
    QString clientId;
    void displayQRCode(const QString &data);
    void processQRData(const QString &data);
};

#endif // QRCODEUI_H
