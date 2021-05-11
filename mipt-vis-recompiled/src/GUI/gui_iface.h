/**
 * File: GUI/gui_iface.h - Interface of GUI library,
 * an internal representation of graphs in MiptVis tool.
 * Copyright (C) 2009  Lev Rumyantsev, Ivan Mashintsev
 */
#ifndef GUI_IFACE_H
#define GUI_IFACE_H

#include "predecls.h"
#include "gui_node.h"
#include "gui_edge.h"
#include "gui_graph.h"
#include "gui_mw.h"
#include "gui_view.h"
#include "gui_textedit.h"


bool showGraph( int argc, char * argv[], GraphAux * graph);

bool showScene( int argc, char * argv[], QApplication * app);

#define CUT_GUI_DEFINITION( type)			\
inline type* cutGui (Gui##type *what)		\
{											\
	return static_cast <type*> ( what);		\
}
#define ADD_GUI_DEFINITION( type)			\
inline Gui##type* addGui ( type* what)		\
{											\
	return static_cast <Gui##type*> ( what);	\
}
#define ADD_CUT_GUI_DEFINITION( type) CUT_GUI_DEFINITION( type) ADD_GUI_DEFINITION( type)

ADD_CUT_GUI_DEFINITION( Node)
ADD_CUT_GUI_DEFINITION( Graph)
ADD_CUT_GUI_DEFINITION( Edge)

#undef ADD_CUT_GUI_DEFINITION
#undef ADD_GUI_DEFINITION
#undef CUT_GUI_DEFINITION

#endif