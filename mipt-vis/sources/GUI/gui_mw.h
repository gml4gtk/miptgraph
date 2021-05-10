/**
 * File: GUI/GuiMW.h - Implementational header for MainWindow of GUI in MiptVis tool.
 * Copyright (C) 2009  MiptVis, Le Manh Ha
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "gui_impl.h"

QT_BEGIN_NAMESPACE
class QMenu;
class QAction;
class GuiView;
class QHBoxLayout;
class QVBoxLayout;
class QWidget;
class QSplitter;
class QString;
class GuiGraph;
class GuiTextEdit;
class QGroupBox;
class QPushButton;
class QPlainTextEdit;
class QListWidget;
class QListWidgetItem;
class QDockWidget;
class QToolBar;
QT_END_NAMESPACE

/**
 * Class MainWindow
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void load();
    void save();
    void help();
    void about();
    void doLayoutSlot();
    void centreOnNode();
    void convertDumpToXmlSlot();
    void doCentreOnNode( int nodeNumber);
	void addNewTextDock( int number);
	void searchButtonClicked();
	void nodeClickedFromList( QListWidgetItem * item);

	void enableGravity();
	void disableGravity();
	void makeGravity();
	void switchVnodesShow();
	void switchEdgeLabelsShow();
	void reactToGravityToggle(bool checked);
	void zoomViewIn();
	void zoomViewOut();
	void hotKeysInfo();

private:
    void createMenus();
    void createActions();
    void createStatusBar();
    void setCurrentFile( const QString & fileName);
	void createConnectionsToNodes();
	void createDockWindows();
	void createToolBars();
	void createHotKeys();
	void saveNodeTexts();
	void closeOldDocks();

    QString strippedName( const QString &fullFileName);
    
    QMenu *file_menu;
    QMenu *view_menu;
    QMenu *tools_menu;
    QMenu *help_menu;
	QMenu *settings_menu;

    QAction *load_act;
    QAction *save_act;
    QAction *help_act;
    QAction *about_act;
    QAction *do_layout_act;
    QAction *convert_dump_to_xml_act;
    QAction *show_virtual_nodes_act;
    QAction *show_edge_labels_act;
	QAction *do_gravity_act;
	QAction *hot_keys_act;

	QToolBar *file_tool_bar;
	QToolBar *find_tool_bar;
	QToolBar *focus_tool_bar;

	QTimer *gravity_timer;

    GuiView *view;
    GuiGraph *graph;
    QString current_file;

    QHBoxLayout *centre_on_node_layout;
    QPushButton *centre_on_node_button;
	QSpinBox *centre_on_node_spin_box;
    QWidget *centre_on_node_widget;

	QLineEdit *search_text_edit;
	QPushButton *search_push_button;
	QHBoxLayout *search_layout;
	QWidget *search_widget;

	QList<GuiNode*> result_list;
	QListWidget *search_result_list;
	QDockWidget *search_result_dock;

	QList<QShortcut*> *list_of_hot_keys;
};

#endif
