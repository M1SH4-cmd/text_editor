#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QLabel>

class QLineEdit;
class QPushButton;
class QCheckBox;

class FindDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FindDialog(QWidget *parent = nullptr);

signals:
    void findNext(const QString &str, bool caseSensitive);
    void findPrev(const QString &str, bool caseSensitive);

private slots:
    void findClicked();
    void enableFindButton(const QString &text);

private:
    QLabel *label;
    QLineEdit *lineEdit;
    QCheckBox *caseCheckBox;
    QCheckBox *backwardCheckBox;
    QPushButton *findButton;
    QPushButton *closeButton;
};

#endif // FINDDIALOG_H
