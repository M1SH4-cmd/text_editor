#ifndef PREFERENCIES_DIALOG_H
#define PREFERENCIES_DIALOG_H

#include "jsonparser.h"

#include <QApplication>

#include <QDialog>
#include <QPushButton>
#include <QComboBox>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QStringList>
#include <QDialogButtonBox>
#include <QFile>

class PrefDialog : public QDialog
{
Q_OBJECT;

public:
    explicit PrefDialog(QWidget *parent);
    ~PrefDialog();

private slots:
    void applyTheme(const QString &themeName);

private:
    // Params in 'Preferencies'
    QString theme;
    std::string themeCfg;
    int width;
    int height;
    JSONParser parser;
    bool maximized;



    QComboBox * themesCBX;
    QComboBox *wndSizes;
    QLabel *themeOptLabel;
    QLabel *wndSizesLabel;
    QStringList themesList = {{"Default"}, {"DarkBlue"}, {"DarkGreen"}, {"DarkMaroon"}, {"DarkPurple"}};

};

#endif // PREFERENCIES_DIALOG_H
