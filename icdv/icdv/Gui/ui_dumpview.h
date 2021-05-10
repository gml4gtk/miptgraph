/********************************************************************************
** Form generated from reading UI file 'dumpview.ui'
**
** Created: Fri Apr 22 09:43:58 2011
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DUMPVIEW_H
#define UI_DUMPVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DumpView
{
public:
    QGraphicsView *DumpLabel;

    void setupUi(QWidget *DumpView)
    {
        if (DumpView->objectName().isEmpty())
            DumpView->setObjectName(QString::fromUtf8("DumpView"));
        DumpView->resize(255, 192);
        DumpLabel = new QGraphicsView(DumpView);
        DumpLabel->setObjectName(QString::fromUtf8("DumpLabel"));
        DumpLabel->setGeometry(QRect(0, 0, 256, 192));

        retranslateUi(DumpView);

        QMetaObject::connectSlotsByName(DumpView);
    } // setupUi

    void retranslateUi(QWidget *DumpView)
    {
        DumpView->setWindowTitle(QApplication::translate("DumpView", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DumpView: public Ui_DumpView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DUMPVIEW_H
