#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    centralWidget = new QWidget();
    this->resize(1280, 720);
    textEdit = new QTextEdit();

    fonts = QFontDatabase::families();

    menuBar = new QMenuBar();
    fileMenu = menuBar->addMenu("File");

    openAction = fileMenu->addAction("Open");
    saveAction = fileMenu->addAction("Save");
    saveAsAction = fileMenu->addAction("Save As...");

    overallLayout = new QVBoxLayout();

    openBtn = new QPushButton("Open");
    openBtn->setFont(QFont("Roboto", 10));
    saveAsBtn = new QPushButton("Save as...");
    saveAsBtn->setFont(QFont("Roboto", 10));
    saveFileBtn = new QPushButton("Save");
    saveFileBtn->setFont(QFont("Roboto", 10));

    fontComboBox = new QComboBox();

    fontComboBox->addItems(fonts);

    fontValue = new QSpinBox();
    fontValue->setRange(2, 72);
    fontValue->setSingleStep(2);

    btnToolBar = new QToolBar();
    btnToolBar->addWidget(fontComboBox);
    btnToolBar->addWidget(fontValue);

    overallLayout->setMenuBar(menuBar);
    overallLayout->addWidget(btnToolBar);
    overallLayout->addWidget(textEdit);

    connect(openAction, &QAction::triggered, this, &MainWindow::open);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);

    connect(fontComboBox, &QComboBox::currentTextChanged, [this](){
        QTextCursor cursor = textEdit->textCursor();
        if (cursor.hasSelection()) {
            QTextCharFormat format;
            format.setFont(fontComboBox->currentText());
            cursor.setCharFormat(format);
        }
    });

    connect(fontValue, &QSpinBox::valueChanged, [this](){
        QTextCursor cursor = textEdit->textCursor();
        if (cursor.hasSelection()) {
            QTextCharFormat format;
            format.setFontPointSize(fontValue->value());
            cursor.setCharFormat(format);
        }
    });

    centralWidget->setLayout(overallLayout);
    setCentralWidget(centralWidget);
}


MainWindow::~MainWindow() {}

void MainWindow::open() {
    QString path = QFileDialog::getOpenFileName(nullptr, "Open txt file",
                                                QDir::homePath() + "/Desktop/",
                                                "Text files (*.txt)");
    bufferFilePath = path;
    std::ifstream txtinF(path.toStdString());

    if (!txtinF.is_open()) return;

    while (!txtinF.eof()) {
        std::string buffer = "";
        txtinF >> buffer;
        text += buffer + "\n";
    }

    textEdit->setText(text);
    txtinF.close();
}

void MainWindow::saveFile() {
    std::ofstream txtoutF(bufferFilePath.toStdString());

    if (!txtoutF.is_open()) return;

    txtoutF << textEdit->toPlainText().toStdString();
    txtoutF.close();
}


void MainWindow::saveAs() {
    QString path = QFileDialog::getSaveFileName(nullptr, "Save txt file",
                                                QDir::homePath() + "/Desktop/",
                                                "Text files (*txt)");
    std::ofstream txtoutF(path.toStdString());
    if (!txtoutF.is_open()) return;

    txtoutF << textEdit->toPlainText().toStdString();
    txtoutF.close();
}
