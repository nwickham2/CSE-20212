#ifndef FINDDIALOG_H
#define FINDDIALOG_H
/*
Nate Wickham
Dylan Zaragoza
Byron Zaragoza

finddialog.h
5/2/2012

FindDialog.h sets up the find dialog widget that is used to find and replace text in the textarea.
Also emits signals that activet slots used in MainWindow to actualy find and repalce the text.
*/


#include <QDialog>
#include <QInputDialog>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QInputDialog;

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    FindDialog(QWidget *parent = 0); //constructor

signals:
    void findNext(const QString &str, Qt::CaseSensitivity cs); // finds the next instance of str going forward
    void findPrevious(const QString &str, Qt::CaseSensitivity cs); // finds the next instance of str going backward
    void replace(const QString &str); //replaces the current selection with str

private slots:
    void findClicked(); // called when the find butotn clicked
    void replaceClicked(); // called when replace button clicked
    void enableFindButton(const QString &text); // lets user click find button
    void enableReplaceButton(const QString &text); //lets user click replace button

private:
    QLabel *label; // "Find"
    QLabel *label2; // "Replace"
    QLineEdit *lineEdit; // word to be found
    QLineEdit *replaceEdit; // word to replace found word
    QCheckBox *caseCheckBox; // case specific find if checked
    QCheckBox *backwardCheckBox; // seraches for word going up page if checked
    QPushButton *findButton;
    QPushButton *replaceButton;
    QPushButton *closeButton; // closes dialog
};

#endif
