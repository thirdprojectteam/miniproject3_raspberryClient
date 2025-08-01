#ifndef RFIDUI_H
#define RFIDUI_H

#include <QWidget>

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
signals:
    void changeWidget(int index);

private slots:
    void on_nextButton_clicked();

private:
    Ui::RFIDUI *ui;
    int m_idx;
};

#endif // RFIDUI_H
