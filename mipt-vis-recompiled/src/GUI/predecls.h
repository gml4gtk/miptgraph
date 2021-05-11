/**
 * File: GUI/predecls.h - Predeclarations for interface of Graph library,
 * an internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Lev Rumyantsev
 */
#ifndef GUI_PREDECLS_H
#define GUI_PREDECLS_H

class GuiGraph;
class GuiNode;
class GuiEdge;
class GuiEdgePart;
class MainWindow;

#include <math.h>
#include "direct.h"
#include <QtGui/QtGui>
/* qt4: #include <QtGui/QApplication>*/
#include <QtWidgets/QApplication>
#include <QtCore/QLineF>

#include "../Utils/utils_iface.h"
#include "../Graph/graph_iface.h"
#include "../Layout/layout_iface.h"

#endif 