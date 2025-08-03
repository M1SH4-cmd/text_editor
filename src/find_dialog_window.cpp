#include "find_dialog_window.h"
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

FindDialog::FindDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Find"));

    label = new QLabel(tr("Find &what:"), this);
    lineEdit = new QLineEdit(this);
    label->setBuddy(lineEdit);

    caseCheckBox = new QCheckBox(tr("Match &case"), this);
    backwardCheckBox = new QCheckBox(tr("Search &backward"), this);

    findButton = new QPushButton(tr("&Find"), this);
    findButton->setDefault(true);
    findButton->setEnabled(false);

    closeButton = new QPushButton(tr("Close"), this);

    connect(lineEdit, &QLineEdit::textChanged, this, &FindDialog::enableFindButton);
    connect(findButton, &QPushButton::clicked, this, &FindDialog::findClicked);
    connect(closeButton, &QPushButton::clicked, this, &FindDialog::close);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(label);
    topLayout->addWidget(lineEdit);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLayout);
    leftLayout->addWidget(caseCheckBox);
    leftLayout->addWidget(backwardCheckBox);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(findButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    setLayout(mainLayout);
}

void FindDialog::findClicked()
{
    const QString text = lineEdit->text();
    const bool cs = caseCheckBox->isChecked();

    if (backwardCheckBox->isChecked()) {
        emit findPrev(text, cs);
    } else {
        emit findNext(text, cs);
    }
}

void FindDialog::enableFindButton(const QString &text)
{
    findButton->setEnabled(!text.isEmpty());
}
