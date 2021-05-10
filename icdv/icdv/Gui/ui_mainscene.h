/********************************************************************************
** Form generated from reading UI file 'mainscene.ui'
**
** Created: Fri Apr 22 21:13:56 2011
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINSCENE_H
#define UI_MAINSCENE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainScene
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionExit;
    QAction *actionAuthors;
    QAction *actionVersion;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QAction *actionSet_Layout_iterations;
    QAction *actionRedraw_graph;
    QWidget *centralwidget;
    QGraphicsView *CFGView;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuAbout;
    QMenu *menuScene;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainScene)
    {
        if (MainScene->objectName().isEmpty())
            MainScene->setObjectName(QString::fromUtf8("MainScene"));
        MainScene->resize(631, 640);
        actionOpen = new QAction(MainScene);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave = new QAction(MainScene);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionExit = new QAction(MainScene);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAuthors = new QAction(MainScene);
        actionAuthors->setObjectName(QString::fromUtf8("actionAuthors"));
        actionVersion = new QAction(MainScene);
        actionVersion->setObjectName(QString::fromUtf8("actionVersion"));
        actionZoom_In = new QAction(MainScene);
        actionZoom_In->setObjectName(QString::fromUtf8("actionZoom_In"));
        actionZoom_Out = new QAction(MainScene);
        actionZoom_Out->setObjectName(QString::fromUtf8("actionZoom_Out"));
        actionSet_Layout_iterations = new QAction(MainScene);
        actionSet_Layout_iterations->setObjectName(QString::fromUtf8("actionSet_Layout_iterations"));
        actionRedraw_graph = new QAction(MainScene);
        actionRedraw_graph->setObjectName(QString::fromUtf8("actionRedraw_graph"));
        centralwidget = new QWidget(MainScene);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        CFGView = new QGraphicsView(centralwidget);
        CFGView->setObjectName(QString::fromUtf8("CFGView"));
        CFGView->setGeometry(QRect(11, 11, 611, 611));
        MainScene->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainScene);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 631, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuAbout = new QMenu(menubar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        menuScene = new QMenu(menubar);
        menuScene->setObjectName(QString::fromUtf8("menuScene"));
        MainScene->setMenuBar(menubar);
        statusbar = new QStatusBar(MainScene);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainScene->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuScene->menuAction());
        menubar->addAction(menuAbout->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuAbout->addAction(actionVersion);
        menuScene->addAction(actionZoom_In);
        menuScene->addAction(actionZoom_Out);
        menuScene->addAction(actionSet_Layout_iterations);

        retranslateUi(MainScene);
        QObject::connect(actionAuthors, SIGNAL(triggered()), MainScene, SLOT(Author()));
        QObject::connect(actionOpen, SIGNAL(triggered()), MainScene, SLOT(LoadDump()));
        QObject::connect(actionSave, SIGNAL(triggered()), MainScene, SLOT(Save()));
        QObject::connect(actionVersion, SIGNAL(triggered()), MainScene, SLOT(Version()));
        QObject::connect(MainScene, SIGNAL(iconSizeChanged(QSize)), MainScene, SLOT(Resize(QSize)));
        QObject::connect(actionZoom_In, SIGNAL(triggered()), MainScene, SLOT(ZoomIn()));
        QObject::connect(actionZoom_Out, SIGNAL(triggered()), MainScene, SLOT(ZoomOut()));
        QObject::connect(MainScene, SIGNAL(destroyed()), MainScene, SLOT(Exit()));
        QObject::connect(actionSet_Layout_iterations, SIGNAL(triggered()), MainScene, SLOT(SetLayoutIteratrions()));

        QMetaObject::connectSlotsByName(MainScene);
    } // setupUi

    void retranslateUi(QMainWindow *MainScene)
    {
        MainScene->setWindowTitle(QApplication::translate("MainScene", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainScene", "Open", 0, QApplication::UnicodeUTF8));
        actionOpen->setShortcut(QApplication::translate("MainScene", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("MainScene", "Save", 0, QApplication::UnicodeUTF8));
        actionSave->setShortcut(QApplication::translate("MainScene", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainScene", "Exit", 0, QApplication::UnicodeUTF8));
        actionExit->setShortcut(QApplication::translate("MainScene", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionAuthors->setText(QApplication::translate("MainScene", "Authors", 0, QApplication::UnicodeUTF8));
        actionVersion->setText(QApplication::translate("MainScene", "Version", 0, QApplication::UnicodeUTF8));
        actionZoom_In->setText(QApplication::translate("MainScene", "Zoom in", 0, QApplication::UnicodeUTF8));
        actionZoom_In->setShortcut(QApplication::translate("MainScene", "+", 0, QApplication::UnicodeUTF8));
        actionZoom_Out->setText(QApplication::translate("MainScene", "Zoom out", 0, QApplication::UnicodeUTF8));
        actionZoom_Out->setShortcut(QApplication::translate("MainScene", "_", 0, QApplication::UnicodeUTF8));
        actionSet_Layout_iterations->setText(QApplication::translate("MainScene", "Set layout iterations", 0, QApplication::UnicodeUTF8));
        actionRedraw_graph->setText(QApplication::translate("MainScene", "Redraw graph", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainScene", "File", 0, QApplication::UnicodeUTF8));
        menuAbout->setTitle(QApplication::translate("MainScene", "About", 0, QApplication::UnicodeUTF8));
        menuScene->setTitle(QApplication::translate("MainScene", "Scene", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainScene: public Ui_MainScene {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINSCENE_H
