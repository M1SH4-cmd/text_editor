#include "mainwindow.h"

#include <QDebug>

using json = nlohmann::json;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/resources/mainWindowIcon.png"));

    prefDialog = new PrefDialog(this);
    currentCfg = prefDialog->currentCFG();
    connect(prefDialog, &PrefDialog::settingsChanged, this, &MainWindow::onSettingsChanged);

    onSettingsChanged(prefDialog->currentCFG());

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
    this->setMinimumSize(800, 600);
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
    preferenciesAction = fileMenu->addAction(QIcon(":resources/prefActionIcon.png"), "Preferencies...");

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
    QWidget *spacer = new QWidget();
    spacer->setFixedWidth(10);  // ширина пробела между виджетами в 10 пикселей
    spacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    btnToolBar->addWidget(spacer);

    btnToolBar->addWidget(fontValue);

    auto *wordSymCount = new QHBoxLayout();
    QLabel *wordLabel = new QLabel();
    QLabel *symLabel = new QLabel();
    wordSymCount->addWidget(wordLabel);
    wordSymCount->addWidget(symLabel);
    wordSymCount->addStretch();
    wordSymCount->setSpacing(10);

    overallLayout->setMenuBar(menuBar);
    overallLayout->addWidget(btnToolBar);
    overallLayout->addWidget(textEdit);
    overallLayout->addLayout(wordSymCount);


    connect(textEdit, &QTextEdit::textChanged, [this, wordLabel, symLabel](){
        QString str = textEdit->toPlainText();
        int wrdc = 0;
        int i = 0;

        for (; i < str.length(); i++) {
        }

        wrdc = wordsCounter(str.toStdString());
        wordLabel->setText(QString::fromStdString("Words: " + std::to_string(wrdc)));
        symLabel->setText(QString::fromStdString("Symbols: " + std::to_string(i)));
    });

    connect(openAction, &QAction::triggered, this, &MainWindow::open);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);

    connect(controlsAction, &QAction::triggered, [](){
        QMessageBox message;
        message.setText(QString("Undo: Ctrl + Z\n"
                                "Redo: Ctrl + Y\n"
                                "Save: Ctrl + S\n"
                                "Open: Ctrl + O\n"
                                "Insert Date: Ctrl + I\n"
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

    connect(preferenciesAction, &QAction::triggered, [this](){
        prefDialog->show();
        prefDialog->raise();
        prefDialog->activateWindow();
    });

    connect(findDialog, &FindDialog::findNext, this, &MainWindow::findNext);
    connect(findDialog, &FindDialog::findPrev, this, &MainWindow::findPrev);

    connect(dateTime, &QAction::triggered, [this](){
        QTextCursor cursor = textEdit->textCursor();
        textEdit->insertPlainText(QString(QDateTime::currentDateTime(QTimeZone::LocalTime).toString("dd/MM/yyyy hh:mm:ss")));
    });

    connect(selectAll, &QAction::triggered, [this](){
        textEdit->selectAll();
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

void MainWindow::onSettingsChanged(const CFG &newCfg) {
    currentCfg = newCfg;
    applyCFG(currentCfg);
}

void MainWindow::applyCFG(const CFG &cfg) {
    setWindowState(Qt::WindowNoState);
    resize(cfg.width, cfg.height);
    if (cfg.maximized) {
        setWindowState(Qt::WindowMaximized);
    }
    currentTheme = QString::fromStdString(cfg.theme);
}

// void MainWindow::resizeEvent(QResizeEvent *event)
// {
//     QSize newSize = event->size();
//     QSize oldSize = event->oldSize();

//     cfg.width = newSize.width();
//     cfg.height = newSize.height();
//     cfg.maximized = QMainWindow::isMaximized();

//     // qDebug() << "MainWindow resized: " << newSize; // Логгирование
//     m_parser.saveSettings("config.json", cfg);

//     // Важно: вызвать базовую реализацию
//     QMainWindow::resizeEvent(event);
// }

int MainWindow::wordsCounter(const std::string &str) {  // на вход подаётся строка
    if (str.size() == 1 or str.empty()) return 0; // если строка не содержит слов - возвращаем ноль

    std::vector<size_t> pos_of_dividers = {};   // создаём массив для фиксирования индексов разделителей в строке

    for (size_t i = 0; i < str.size(); ++i) {   // в цикле ищем разделители и добавляем значения индексов в массив
        if (str[i] == ' ' or str[i] == ',' or str[i] == '.')
            pos_of_dividers.push_back(i);
    }

    if (pos_of_dividers.empty()) return 1;  // если разделители не удалось найти, то строка состоит из одного слова

    int number_of_words = 2;    // первое и последнее слово, т.к. у нас варианты,
        //в которых строка всегда начинается со слова и заканчивается словом,
        //а варианты с пустой строкой и одним словом мы уже учли

    for (size_t i = 0; i < pos_of_dividers.size() - 1; ++i) {       //вычитаем единицу, чтобы не выйти за границы массива,
        // т.к. через i мы будем получать доступ к его элементам, причем
        size_t first_index_of_divider = pos_of_dividers[i];         //в последнем прогоне это будет предпоследний элемент
        size_t second_index_of_divider = pos_of_dividers[i + 1];    // а это последний
        if (second_index_of_divider - first_index_of_divider != 1) { //проверяем, что между разделителями что-то есть
            ++number_of_words;
        }
    }

    return number_of_words;
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
