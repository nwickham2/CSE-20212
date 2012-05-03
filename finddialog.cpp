/*
Nate Wickham
Dylan Zaragoza
Byron Zaragoza

finddialog.cpp
5/2/2012

FindDialog.cpp sets up the find dialog widget that is used to find and replace text in the textarea.
Also emits signals that activet slots used in MainWindow to actualy find and repalce the text.
*/


#include <QtGui>
#include <QDebug>
#include "finddialog.h"

FindDialog::FindDialog(QWidget *parent): QDialog(parent)
{
    label = new QLabel(tr("Keyword:"));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);

    label2 = new QLabel(tr("Replace With"));
    replaceEdit = new QLineEdit;
    label2->setBuddy(replaceEdit);

    caseCheckBox = new QCheckBox(tr("Match &case"));
    backwardCheckBox = new QCheckBox(tr("Search &backward"));

    findButton = new QPushButton(tr("&Find"));
    findButton->setDefault(true);
    findButton->setEnabled(false);

    replaceButton = new QPushButton(tr("&Replace"));
    replaceButton->setDefault(true);
    replaceButton->setEnabled(false);

    closeButton = new QPushButton(tr("Close"));

    connect(lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableFindButton(const QString &)));
    connect(replaceEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enableReplaceButton(const QString &)));
    connect(findButton, SIGNAL(clicked()), this, SLOT(findClicked()));
    connect(replaceButton, SIGNAL(clicked()), this, SLOT(replaceClicked()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));



    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(label);
    topLeftLayout->addWidget(lineEdit);
    topLeftLayout->addWidget(label2);
    topLeftLayout->addWidget(replaceEdit);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addWidget(caseCheckBox);
    leftLayout->addWidget(backwardCheckBox);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(findButton);
    rightLayout->addWidget(replaceButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    setLayout(mainLayout);
    setWindowTitle(tr("Find"));
    setFixedHeight(sizeHint().height());
}

void FindDialog::findClicked()
{

    QString text = lineEdit->text();
    Qt::CaseSensitivity cs = caseCheckBox->isChecked() ? Qt::CaseSensitive: Qt::CaseInsensitive;
    if (backwardCheckBox->isChecked()) {
        emit findPrevious(text, cs);
    } else {
        emit findNext(text, cs);
    }
}

void FindDialog::replaceClicked()
{
    QString newText = replaceEdit->text();
    emit replace(newText);
}

void FindDialog::enableFindButton(const QString &text)
{
    findButton->setEnabled(!text.isEmpty());
}

void FindDialog::enableReplaceButton(const QString &text)
{
    replaceButton->setEnabled(!text.isEmpty());
}

