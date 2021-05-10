/**
 * File: GUI/GuiView.h - Implementational header for GuiView of GUI in MiptVis tool.
 * Copyright (C) 2009  MiptVis, Le Manh Ha
 */
#ifndef GUI_VIEW_H
#define GUI_VIEW_H

#include <QtGui/QGraphicsView>
#include "gui_impl.h"

QT_BEGIN_NAMESPACE
class GuiView;
QT_END_NAMESPACE

/**
 * Class GuiView
 */
class GuiView : public QGraphicsView
{
    Q_OBJECT
 public:
    GuiView( GuiGraph *graph);
    void wheelEvent( QWheelEvent *event);
    void scaleView( qreal scaleFactor);
};

#endif 