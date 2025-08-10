#include "preferencies_dialog.h"
#include <QResizeEvent>

PrefDialog::PrefDialog(QWidget *parent) : QDialog(parent) {
    parser.loadSettings("config.json", themeCfg, width, height);

    theme = QString::fromStdString(themeCfg);
    PrefDialog::applyTheme(theme);

    setWindowTitle(tr("Preferences"));

    auto *centralLayout = new QVBoxLayout(this);

    auto *themeOptLayout = new QHBoxLayout();

    themeOptLabel = new QLabel(tr("Theme:"), this);
    themesCBX = new QComboBox();
    themesCBX->addItems(themesList);

    //сохранённый выбор
    themesCBX->setCurrentText(theme);

    themeOptLayout->addWidget(themeOptLabel);
    themeOptLayout->addWidget(themesCBX);

    centralLayout->addLayout(themeOptLayout);

    auto *wndSizeLayout = new QHBoxLayout();
    wndSizesLabel = new QLabel(tr("Window size:"), this);
    wndSizes = new QComboBox(this);
    wndSizes->addItem(tr("Small"));
    wndSizes->addItem(tr("Medium"));
    wndSizes->addItem(tr("Large"));
    maximized = false;
    wndSizeLayout->addWidget(wndSizesLabel);
    wndSizeLayout->addWidget(wndSizes);

    centralLayout->addLayout(wndSizeLayout);

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(themesCBX, &QComboBox::currentTextChanged, [this](){
        PrefDialog::applyTheme(themesCBX->currentText());
        parser.saveSettings("config.json", themesCBX->currentText().toStdString(), width, height);
    });

    connect(wndSizes, &QComboBox::currentTextChanged, [this, parent](){
        if (wndSizes->currentText() == "Small" && parent) {
            parent->resize(800, 600);
            maximized = false;
            width = 800;
            height = 600;
        } else if (wndSizes->currentText() == "Medium" && parent) {
            parent->resize(1280, 720);
            maximized = false;
            width = 1280;
            height = 720;
        } else if (wndSizes->currentText() == "Large" && parent) {
            parent->setWindowState(Qt::WindowMaximized);
            maximized = true;
            width = parent->width();
            height = parent->height();
        }
        parser.saveSettings("config.json", themesCBX->currentText().toStdString(), width, height);
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

PrefDialog::~PrefDialog(){
}
