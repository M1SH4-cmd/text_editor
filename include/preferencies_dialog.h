#ifndef PREFERENCIES_DIALOG_H
#define PREFERENCIES_DIALOG_H
#pragma once

#include "jsonparser.h"
#include <QApplication>

#include <QDialog>
#include <QPushButton>
#include <QComboBox>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QStringList>
#include <QDialogButtonBox>
#include <QFile>

class PrefDialog : public QDialog
{
Q_OBJECT;

public:
    explicit PrefDialog(QWidget *parent);
    ~PrefDialog();

    CFG currentCFG();
    void applySettings();

signals:
    void settingsChanged(const CFG &newCfg);

private slots:
    void applyTheme(const QString &themeName);

private:
    // Params in 'Preferencies'
    CFG cfg;

    QComboBox *themesCBX;
    QComboBox *languagesCBX;

    QComboBox *wndSizes;
    QCheckBox *wndMaximize;

    QLabel *languagesLabel;
    QLabel *themeOptLabel;
    QLabel *wndSizesLabel;
    QLabel *wndMaximizeLabel;

    QStringList themesList = {{"Default"}, {"DarkBlue"}, {"DarkGreen"}, {"DarkMaroon"}, {"DarkPurple"}};
    QStringList languagesList = {{"English"}, {"Russian"}};

};

#endif // PREFERENCIES_DIALOG_H
