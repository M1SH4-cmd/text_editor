#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if (argc > 1) {
        QString filePath = QString::fromLocal8Bit(argv[1]);
        w.openFromWin(filePath);
    }
    w.show();
    return a.exec();
}
