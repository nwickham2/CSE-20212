#ifndef FINDDIALOG_H
#define FINDDIALOG_H
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
        FindDialog(QWidget *parent = 0);

    signals:
        void findNext(const QString &str, Qt::CaseSensitivity cs);
        void findPrevious(const QString &str, Qt::CaseSensitivity cs);
        void replace(const QString &str);

    private slots:
        void findClicked();
        void replaceClicked();
        void enableFindButton(const QString &text);
        void enableReplaceButton(const QString &text);

    private:
        QLabel *label;
        QLabel *label2;
        QLineEdit *lineEdit;
        QLineEdit *replaceEdit;
        QCheckBox *caseCheckBox;
        QCheckBox *backwardCheckBox;
        QPushButton *findButton;
        QPushButton *replaceButton;
        QPushButton *closeButton;
};

#endif
