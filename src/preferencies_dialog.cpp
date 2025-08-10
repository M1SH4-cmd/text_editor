#include "preferencies_dialog.h"

#include <QResizeEvent>

PrefDialog::PrefDialog(QWidget *parent) : QDialog(parent) {
    parser.loadSettings("config.json", cfg);
    parent->resize(cfg.width, cfg.height);

    PrefDialog::applyTheme(QString::fromStdString(cfg.theme));

    setWindowTitle(tr("Preferences"));

    auto *centralLayout = new QVBoxLayout(this);

    auto *themeOptLayout = new QHBoxLayout();

    themeOptLabel = new QLabel(tr("Theme:"), this);
    themesCBX = new QComboBox();
    themesCBX->addItems(themesList);

    //сохранённый выбор
    themesCBX->setCurrentText(QString::fromStdString(cfg.theme));

    themeOptLayout->addWidget(themeOptLabel);
    themeOptLayout->addWidget(themesCBX);

    centralLayout->addLayout(themeOptLayout);

    auto *wndSizeLayout = new QHBoxLayout();
    wndSizesLabel = new QLabel(tr("Window size:"), this);
    wndSizes = new QComboBox(this);
    wndSizes->addItem(tr("Small"));
    wndSizes->addItem(tr("Medium"));
    wndSizes->addItem(tr("Large"));
    wndSizeLayout->addWidget(wndSizesLabel);
    wndSizeLayout->addWidget(wndSizes);

    centralLayout->addLayout(wndSizeLayout);

    auto *maximizeWndLayout = new QHBoxLayout();
    wndMaximizeLabel = new QLabel(tr("Fullscreen:"), this);
    wndMaximize = new QCheckBox(this);
    //maximized = false;
    maximizeWndLayout->addWidget(wndMaximizeLabel);
    maximizeWndLayout->addWidget(wndMaximize);

    centralLayout->addLayout(maximizeWndLayout);

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    //connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(themesCBX, &QComboBox::currentTextChanged, [this](){
        PrefDialog::applyTheme(themesCBX->currentText());
        cfg.theme = themesCBX->currentText().toStdString();
        parser.saveSettings("config.json", cfg);
    });

    connect(wndSizes, &QComboBox::currentTextChanged, [this, parent](){
        if (wndSizes->currentText() == "Small" && parent) {
            parent->resize(800, 600);
            cfg.maximized = false;
            cfg.width = 800;
            cfg.height = 600;
        } else if (wndSizes->currentText() == "Medium" && parent) {
            parent->resize(1280, 720);
            cfg.maximized = false;
            cfg.width = 1280;
            cfg.height = 720;
        } else if (wndSizes->currentText() == "Large" && parent) {
            parent->resize(1600, 900);
            cfg.maximized = false;
            cfg.width = 1600;
            cfg.height = 900;
        }
        parser.saveSettings("config.json", cfg);
    });

    connect(wndMaximize, &QCheckBox::checkStateChanged, [this, parent](){
        cfg.maximized = wndMaximize->isChecked();
        if (cfg.maximized) {
            parent->setWindowState(Qt::WindowFullScreen);
        } else {
            parent->setWindowState(Qt::WindowNoState);
            parent->resize(cfg.width, cfg.height);
        }
    });

    centralLayout->addWidget(buttonBox);
}

void PrefDialog::applyTheme(const QString &themeName) {
    if (themeName == "Default") {
        qApp->setStyleSheet("");
    } else {
        QFile themeFile(QString(":/themes/%1.qss").arg(themeName));
        if (themeFile.open(QFile::ReadOnly)) {
            QString styleSheet = themeFile.readAll();
            qApp->setStyleSheet(styleSheet);
            themeFile.close();
        } else {
            qDebug() << "Could not open theme file:" << themeName;
        }
    }
}

QString PrefDialog::getTheme() {
    return QString::fromStdString(cfg.theme);
}

JSONParser PrefDialog::getParser() {
    return parser;
}

PrefDialog::~PrefDialog(){
}
