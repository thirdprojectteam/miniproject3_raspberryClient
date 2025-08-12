#ifndef FINISHUI_H
#define FINISHUI_H

#include <QWidget>

namespace Ui {
class FinishUI;
}

class FinishUI : public QWidget
{
    Q_OBJECT

public:
    explicit FinishUI(QWidget *parent = nullptr);
    ~FinishUI();

    void init();
signals:
    void changeWidget(int idx);
private slots:
    void on_OKButton_clicked();

private:
    Ui::FinishUI *ui;
};

#endif // FINISHUI_H
