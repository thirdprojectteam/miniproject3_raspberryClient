#ifndef QRCODEUI_H
#define QRCODEUI_H

#include <QWidget>
#include <QPixmap>
#include <QString>

namespace Ui {
class QRCodeUI;
}

class QRCodeUI : public QWidget
{
    Q_OBJECT

public:
    explicit QRCodeUI(QWidget *parent = nullptr);
    ~QRCodeUI();
    
    void generateQRCode(const QString &data);
    void init();

signals:
    void changeWidget(int idx);

private slots:
    void on_backButton_clicked();
    void on_generateButton_clicked();

private:
    Ui::QRCodeUI *ui;
    QString generateSimpleQRData();
    void displayQRCode(const QString &data);
};

#endif // QRCODEUI_H