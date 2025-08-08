#include "preferencies_dialog.h"


PrefDialog::PrefDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("Preferences"));

    auto *centralLayout = new QVBoxLayout(this);

    auto *themeOptLayout = new QHBoxLayout();

    themeOptLabel = new QLabel(tr("Theme:"), this);
    themesCBX = new QComboBox();
    themesCBX->addItems(themesList);

    //выбор по умолчанию
    themesCBX->setCurrentText("Default");

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

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(themesCBX, &QComboBox::currentTextChanged, this, &PrefDialog::applyTheme);

    centralLayout->addWidget(buttonBox);
}

void PrefDialog::applyTheme(const QString &themeName) {
    if (themeName == "Default") {
        qApp->setStyleSheet("");
    } else {
        QFile themeFile(QString(":/darkThemes/%1.qss").arg(themeName));
        if (themeFile.open(QFile::ReadOnly)) {
            QString styleSheet = themeFile.readAll();
            qApp->setStyleSheet(styleSheet);
            themeFile.close();
        } else {
            qDebug() << "Could not open theme file:" << themeName;
        }
    }
}
