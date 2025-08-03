#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <find_dialog_window.h>

#include <QMainWindow>
#include <QIcon>
#include <QApplication>

#include <QDate>
#include <QTimer>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QTextEdit>
#include <QFontDatabase>
#include <QPalette>
#include <QTextDocumentWriter>
#include <QSpinBox>
#include <QComboBox>
#include <QToolBar>
#include <QMenuBar>

#include <QDir>
#include <QFileDialog>
#include <QFile>

#include <QMessageBox>
#include <QKeyEvent>
#include <QStatusBar>



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void open();
    void openFromWin(QString filePath);
    void saveAs();
    void saveFile();
    void keyPressEvent(QKeyEvent *e);

private slots:
    void findNext(const QString &str, bool cs);
    void findPrev(const QString &str, bool cs);

private:
    QString text;
    QString bufferFilePath;
    QStringList fonts;

    QWidget *centralWidget;

    QTextEdit *textEdit;
    QPalette p;
    QVBoxLayout *overallLayout;
    FindDialog *findDialog;

    QToolBar *btnToolBar;

    QComboBox *fontComboBox;
    QSpinBox *fontValue;

    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *editMenu;

    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;

    QAction *controlsAction;
    QAction *aboutAction;

    QAction *find;
    QAction *selectAll;
    QAction *dateTime;
};

#endif
