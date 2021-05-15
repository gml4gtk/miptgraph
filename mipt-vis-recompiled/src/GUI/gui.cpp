/**
 * File: GUI/gui.cpp - Implementation of GUI in MiptVis
 * Copyright (C) 2009  Lev Rumyantsev, Ivan Mashintsev
 */
#include "gui_impl.h"

/** 
 * Test scene with rectangles and text
 */
bool showScene( int argc, char * argv[], QApplication *app)
{
    MainWindow mw;
    mw.show();
    app->exec();
    return true;
}

#if 0
int main(int argc, char **argv)
{
    bool status = 0;
    QApplication app( argc, argv);
    GuiGraph * graph = new GuiGraph();
    /** Test Scene with nodes */
    if (1) status = showScene( argc, argv, &app);
    /* status = showGraph( argc, argv, &app); */
    return 0;
}
#endif

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("mipt-vis");
    app.setApplicationName("mipt-vis");
    MainWindow mainWin;
    GuiGraph * graph = new GuiGraph();
    mainWin.show();
    return app.exec();
}
