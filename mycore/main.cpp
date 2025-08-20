#include "ui/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.setCursor(Qt::BlankCursor);
    w.showFullScreen();
    return a.exec();
}
