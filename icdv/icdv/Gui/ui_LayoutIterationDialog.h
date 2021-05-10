/********************************************************************************
** Form generated from reading UI file 'LayoutIterationDialog.ui'
**
** Created: Fri Apr 22 09:45:17 2011
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAYOUTITERATIONDIALOG_H
#define UI_LAYOUTITERATIONDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LayoutIterationDialog
{
public:
    QWidget *widget;
    QGridLayout *gridLayout;
    QSpinBox *layoutSpinBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *LayoutIterationDialog)
    {
        if (LayoutIterationDialog->objectName().isEmpty())
            LayoutIterationDialog->setObjectName(QString::fromUtf8("LayoutIterationDialog"));
        LayoutIterationDialog->resize(153, 74);
        widget = new QWidget(LayoutIterationDialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 154, 74));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        layoutSpinBox = new QSpinBox(widget);
        layoutSpinBox->setObjectName(QString::fromUtf8("layoutSpinBox"));

        gridLayout->addWidget(layoutSpinBox, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(widget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 1);


        retranslateUi(LayoutIterationDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), LayoutIterationDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), LayoutIterationDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(LayoutIterationDialog);
    } // setupUi

    void retranslateUi(QDialog *LayoutIterationDialog)
    {
        LayoutIterationDialog->setWindowTitle(QApplication::translate("LayoutIterationDialog", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LayoutIterationDialog: public Ui_LayoutIterationDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAYOUTITERATIONDIALOG_H
