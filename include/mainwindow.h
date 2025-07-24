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
#include <fstream>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void open();
    void saveAs();
    void saveFile();

private:
    QString text;
    QString bufferFilePath;
    QStringList fonts;

    QWidget *centralWidget;

    QTextEdit *textEdit;
    QVBoxLayout *overallLayout;

    QPushButton *openBtn;
    QPushButton *saveAsBtn;
    QPushButton *saveFileBtn;
    QToolBar *btnToolBar;

    QComboBox *fontComboBox;
    QSpinBox *fontValue;

    QMenuBar *menuBar;
    QMenu *fileMenu;

    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;

};

#endif
