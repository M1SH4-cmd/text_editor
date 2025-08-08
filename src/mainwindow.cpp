#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/resources/mainWindowIcon.png"));

    QTimer *timer = new QTimer(this);
    timer->start(10000);
    connect(timer, &QTimer::timeout, [this, timer](){
        if (!bufferFilePath.isEmpty()) {
            MainWindow::saveFile();
        }
        timer->stop();
        timer->start(10000);
    });

    centralWidget = new QWidget();
    this->resize(1280, 720);
    textEdit = new QTextEdit();

    p = textEdit->palette();
    p.setColor(QPalette::Highlight, Qt::green);  // цвет фона выделения (подсветки)
    p.setColor(QPalette::HighlightedText, Qt::black); // цвет выделенного текста
    textEdit->setPalette(p);

    fonts = QFontDatabase::families();

    menuBar = new QMenuBar();

    fileMenu = menuBar->addMenu("File");

    openAction = fileMenu->addAction(QIcon(":resources/openActionIcon.png"), "Open");
    saveAction = fileMenu->addAction(QIcon(":resources/saveActionIcon.png"), "Save");
    saveAsAction = fileMenu->addAction(QIcon(":resources/saveAsActionIcon.png"), "Save as...");

    editMenu = menuBar->addMenu("Edit");

    find = editMenu->addAction(QIcon(":resources/findActionIcon.png"), "Find");
    findDialog = new FindDialog(this);
    selectAll = editMenu->addAction(QIcon(":resources/selectAllActionIcon.png"), "Select all");
    dateTime = editMenu->addAction(QIcon(":resources/insertDateActionIcon.png"), "Insert date");

    helpMenu = menuBar->addMenu("Help");

    controlsAction = helpMenu->addAction(QIcon(":resources/controlsActionIcon.png"), "Controls");
    aboutAction = helpMenu->addAction(QIcon(":resources/aboutActionIcon.png"), "About...");

    overallLayout = new QVBoxLayout();

    fontComboBox = new QComboBox();
    fontComboBox->addItems(fonts);
    fontComboBox->setCurrentText("Times New Roman");

    fontValue = new QSpinBox();
    fontValue->setRange(2, 72);
    fontValue->setSingleStep(2);
    fontValue->setValue(12);

    btnToolBar = new QToolBar();
    btnToolBar->addWidget(fontComboBox);
    btnToolBar->addWidget(fontValue);

    overallLayout->setMenuBar(menuBar);
    overallLayout->addWidget(btnToolBar);
    overallLayout->addWidget(textEdit);

    connect(openAction, &QAction::triggered, this, &MainWindow::open);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);

    connect(controlsAction, &QAction::triggered, [](){
        QMessageBox message;
        message.setText(QString("Undo: Ctrl + Z\n"
                                "Redo: Ctrl + Y\n"
                                "Save: Ctrl + S\n"
                                "Open: Ctrl + O\n"
                                "Button 'Save as...' saves a new file\n"
                                "Button 'Save' saves current opened file\n"
                                "Button 'Open' opens selected file\n"));
        message.setFont(QFont("Roboto", 14));
        message.exec();
    });

    connect(find, &QAction::triggered, [this](){
        findDialog->show();
        findDialog->raise();
        findDialog->activateWindow();
    });
    connect(findDialog, &FindDialog::findNext, this, &MainWindow::findNext);
    connect(findDialog, &FindDialog::findPrev, this, &MainWindow::findPrev);

    connect(dateTime, &QAction::triggered, [this](){
        QTextCursor cursor = textEdit->textCursor();
        textEdit->insertPlainText(QString(QDateTime::currentDateTime(QTimeZone::LocalTime).toString("dd/MM/yyyy hh:mm:ss")));
    });

    connect(selectAll, &QAction::triggered, [this](){
        auto e = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_A, Qt::ControlModifier);
        QApplication::sendEvent(textEdit, e);
    });

    connect(aboutAction, &QAction::triggered, [](){
        QMessageBox message;
        message.setText(QString("A casual 'Notebook' app that can open and edit casual .txt files\n"
                                "Check 'controls' to see all hotkeys\n\n\n"
                                "Made by OG_Michael\n"));

        message.setFont(QFont("Roboto", 14));
        message.exec();
    });

    connect(fontComboBox, &QComboBox::currentTextChanged, [this](){
        QTextCursor cursor(textEdit->document());
        cursor.select(QTextCursor::Document);
        QTextCharFormat format;
        format.setFontFamily(fontComboBox->currentText());
        cursor.mergeCharFormat(format);
        textEdit->setCurrentCharFormat(format);
    });

    connect(fontValue, &QSpinBox::valueChanged, [this](){
        QTextCursor cursor(textEdit->document());
        cursor.select(QTextCursor::Document);
        QTextCharFormat format;
        format.setFontPointSize(fontValue->value());
        cursor.mergeCharFormat(format);
        textEdit->setCurrentCharFormat(format);
    });

    centralWidget->setLayout(overallLayout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {}

void MainWindow::open() {
    QString path = QFileDialog::getOpenFileName(nullptr, "Open txt file",
                                                QDir::homePath() + "/Desktop/",
                                                "Text files (*.txt)");
    if (path.isEmpty()) return;

    bufferFilePath = "";
    bufferFilePath = path;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open file");
        return;
    }

    QTextStream in(&file);
    textEdit->setPlainText(in.readAll());
    file.close();
    statusBar()->showMessage("File loaded: " + bufferFilePath, 2000);
}

void MainWindow::openFromWin(QString path) {
    if (path.isEmpty()) return;

    bufferFilePath = "";
    bufferFilePath = path;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open file");
        return;
    }

    QTextStream in(&file);
    textEdit->setPlainText(in.readAll());
    file.close();
    statusBar()->showMessage("File loaded: " + bufferFilePath, 2000);
}

void MainWindow::saveFile() {
    if (bufferFilePath.isEmpty()) {
        saveAs();
        return;
    }

    QFile file(bufferFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot save file");
        return;
    }

    QTextStream out(&file);
    out << textEdit->toPlainText();
    file.close();
    statusBar()->showMessage("File saved: " + bufferFilePath, 2000);
}

void MainWindow::saveAs() {
    QString path = QFileDialog::getSaveFileName(
        this,
        "Save txt file",
        QDir::homePath() + "/Desktop/",
        "Text files (*.txt)"
        );

    if (path.isEmpty()) {
        return;
    }

    if (!path.endsWith(".txt", Qt::CaseInsensitive)) {
        path += ".txt";
    }

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot save file");
        return;
    }

    QTextStream out(&file);
    out << textEdit->toPlainText();
    file.close();

    bufferFilePath = path;

    statusBar()->showMessage("File saved: " + path, 2000);
}

void MainWindow::findNext(const QString &str, bool caseSensitive)
{
    QTextDocument::FindFlags options;
    if (caseSensitive)
        options |= QTextDocument::FindCaseSensitively;

    bool found = textEdit->find(str, options);
    if (!found) {
        QMessageBox::information(this, tr("Find"),
                                 tr("Cannot find \"%1\"").arg(str));
        // Вернуть курсор в начало для цикличного поиска, если нужно
        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::Start);
        textEdit->setTextCursor(cursor);
    }
}

void MainWindow::findPrev(const QString &str, bool caseSensitive)
{
    QTextDocument::FindFlags options = QTextDocument::FindBackward;
    if (caseSensitive)
        options |= QTextDocument::FindCaseSensitively;

    bool found = textEdit->find(str, options);
    if (!found) {
        QMessageBox::information(this, tr("Find"),
                                 tr("Cannot find \"%1\"").arg(str));
        // Вернуть курсор в конец для цикличного поиска назад, если нужно
        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        textEdit->setTextCursor(cursor);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    switch(e->key()) {
    case Qt::Key_S:
        if (e->modifiers() & Qt::ControlModifier) {
            saveFile();
        }
        break;

    case Qt::Key_O:
        if (e->modifiers() & Qt::ControlModifier) {
            open();
        }
        break;

    case Qt::Key_F:
        if (e->modifiers() & Qt::ControlModifier) {
            find->trigger();
        }
        break;

    case Qt::Key_I:
        if (e->modifiers() & Qt::ControlModifier) {
            dateTime->trigger();
        }
        break;

    default:
        QMainWindow::keyPressEvent(e);
    }
}
