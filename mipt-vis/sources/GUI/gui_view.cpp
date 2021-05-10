/**
 * File: GUI/GuiView.cpp - Implementation of GuiView in MiptVis
 * Copyright (C) 2009  MiptVis, Le Manh Ha
 */
#include "gui_view.h"
#include <QtCore/QDebug>
#include <QtGui/QGraphicsScene>
#include <QtGui/QWheelEvent>
#include <math.h>

/**
 * Constructor of GuiView class
 */
GuiView::GuiView( GuiGraph *graph)
{
    setScene( graph);
    setCacheMode( QGraphicsView::CacheBackground );
    setViewportUpdateMode( QGraphicsView::FullViewportUpdate);
    setRenderHints( QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setOptimizationFlags( QGraphicsView::DontSavePainterState | QGraphicsView::DontClipPainter);
    setResizeAnchor( QGraphicsView::AnchorViewCenter);
    setDragMode( QGraphicsView::ScrollHandDrag); 
    setMinimumSize( 400, 400);
}


/**
 * Actions for wheel event
 */
void GuiView::wheelEvent( QWheelEvent *event)
{
    scaleView( pow((double)2, event->delta() / 240.0));
}

/**
 * Scales the current view transformation
 */
void GuiView::scaleView( qreal scaleFactor)
{
    qreal factor = matrix().scale( scaleFactor, scaleFactor).mapRect( QRectF( 0, 0, 1, 1)).width();
    scale( scaleFactor, scaleFactor);
} 
