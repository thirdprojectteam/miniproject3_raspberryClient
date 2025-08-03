#ifndef RFIDUI_H
#define RFIDUI_H

#include <QWidget>
#include "rf522worker.h"
#include <QThread>

namespace Ui {
class RFIDUI;
}

class RFIDUI : public QWidget
{
    Q_OBJECT

public:
    explicit RFIDUI(QWidget *parent = nullptr);
    ~RFIDUI();
    //set idx
    void setidx(int idx) { m_idx=idx;}
    //start thread
    void rfidThreadStart();

    void rfidThreadEnd();

    void onDataRead(QString uid,QString data);
signals:
    void changeWidget(int index);

private slots:
    void on_nextButton_clicked();

    void on_retryButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::RFIDUI *ui;
    int m_idx;
    //rfidThread
    QThread *rfidThread;
    RF522Worker *m_rfWorker;
};

#endif // RFIDUI_H
