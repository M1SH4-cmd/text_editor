#include "mainwindow.h"
#include <QApplication>
#include <QStandardPaths>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    if (argc > 1) {
        QString filePath = QString::fromLocal8Bit(argv[1]);
        w.openFromWin(filePath);
    }

    w.show();

    std::ifstream cfg("config.json");
    if (!cfg.is_open()) {
        QMessageBox::warning(&w, "Error", "Cannot open config.json file!\nDefault settings will be loaded\n");
    } else {
        cfg.close();
    }

    return a.exec();
}
