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
#include <QKeyEvent>
#include <QStatusBar>

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
    //void updateFormattingControls();

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

    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;

    QAction *controlsAction;
    QAction *aboutAction;


};

#endif
