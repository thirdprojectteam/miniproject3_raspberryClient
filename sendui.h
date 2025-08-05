#ifndef SENDUI_H
#define SENDUI_H

#include <QWidget>
#include <QLineEdit>
#include "mylineedit.h"

namespace Ui {
class SendUI;
}

class SendUI : public QWidget
{
    Q_OBJECT

public:
    explicit SendUI(QWidget *parent = nullptr);
    ~SendUI();
    void appendDigit(int num);
    void clearDigit();
    void eraseDigit();
    void init();

private:
    Ui::SendUI *ui;
    QLineEdit *curLineEdit;
};

#endif // SENDUI_H
