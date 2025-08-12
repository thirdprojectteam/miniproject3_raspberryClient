#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>

class myLineEdit:public QLineEdit
{
    Q_OBJECT
public:
    explicit myLineEdit(QWidget*parent = nullptr):QLineEdit(parent){}

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent*event) override {
        emit clicked();
        QLineEdit::mousePressEvent(event);
    }
};

#endif // MYLINEEDIT_H
