#include "mainwindow.h"
#include <QApplication>
#include <QStandardPaths>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QMessageBox>

int main (int argc, char *argv[])
{
    QApplication a(argc, argv);

    // QTranslator translator;

    // QString qtTranslationsPath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
    // translator.load("qt_" + QLocale::system().name(), qtTranslationsPath);
    // a.installTranslator(&translator);

    // // второй переводчик для приложения
    // QTranslator appTranslator;

    // // Попытка загрузки перевода приложения
    // if (appTranslator.load(QLocale::system(), "app", "_", ":/translations")) {
    //     a.installTranslator(&appTranslator);
    // } else {
    //     qDebug() << "Translation not found for:" << QLocale::system().name();
    // }


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
