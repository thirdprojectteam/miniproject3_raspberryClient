#ifndef SELECTIONQRRFID_H
#define SELECTIONQRRFID_H

#include <QWidget>

namespace Ui {
class SelectionUI;
}

class SelectionUI : public QWidget
{
    Q_OBJECT

public:
    explicit SelectionUI(QWidget *parent = nullptr);
    ~SelectionUI();

signals:
    void qrSelected();
    void rfidSelected();

private slots:
    void on_qrButton_clicked();
    void on_rfidButton_clicked();

private:
    Ui::SelectionUI *ui;
};

#endif // SELECTIONQRRFID_H
