#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QToolBar>
#include <QMenuBar>
#include <QSpinBox>
#include <QDir>
#include <QFileDialog>
#include <QDialog>
#include <QFile>
#include <QFontDatabase>
#include <QTextDocumentWriter>
#include <QMessageBox>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDate>
#include <QApplication>
#include <QTimer>
#include <QIcon>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void open();
    void saveAs();
    void saveFile();
    void keyPressEvent(QKeyEvent *e);

private:
    QString text;
    QString bufferFilePath;
    QStringList fonts;

    QWidget *centralWidget;

    QTextEdit *textEdit;
    QVBoxLayout *overallLayout;

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
    QAction *findNext;
    QAction *selectAll;
    QAction *dateTime;
};

#endif
