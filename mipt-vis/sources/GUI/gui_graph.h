/**
 * File: GUI/GuiGraph.h - Implementational header for GuiGraph of GUI in MiptVis tool.
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */
#ifndef GUI_GRAPH_H
#define GUI_GRAPH_H
#include <QtGui/QGraphicsScene>
#include "../Layout/layout_iface.h"
#include "gui_style_sheet.h"
class GuiEdge;
class GuiNode;

/**
 * Class GuiGraph
 */
class GuiGraph:public QGraphicsScene, public GraphAux
{
    Q_OBJECT
public:
	
    GuiGraph( QObject * parent=0);
    GuiGraph( char * filename, QObject * parent = 0);
    enum mode { insertLine, moveItem, insertRect};
    enum type { node, point, edge};
    
	virtual void readAttribsFromXml (xmlNode * a_node);
	virtual void writeAttribsByXmlWriter (xmlTextWriterPtr writer);

    NodeAux * createNode();
    
    EdgeAux * createEdge( Node * pred, Node * succ); //!!! may be need another return type

    inline QGraphicsItem * getNodeItem()
    {
        GuiNode * gui_node_p = ( GuiNode *)firstNode();
        return (QGraphicsItem *) gui_node_p;       
    }
	
    virtual NodeAux * insertNodeOnEdge (EdgeAux* e);

	virtual void removeEdge (Edge* e);
	virtual void removeNode (Node* n);

	void switchVnodesShow();
	void switchEdgeLabelsShow();

	bool showVnodes();
	bool showEdgeLabels();

protected:
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent);
    void mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent);
    void mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent);
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent);
    bool applayLayout();
	void setEnabledToNodes();
	void setDisabledToNodes();

private:
    int number_priv;
    mode mode_priv;
    QGraphicsLineItem * line_priv;
    xmlDoc * xml_doc_priv;
	StyleSheet graph_style_sheet_priv;
	bool draw_virtual_nodes_priv;
	bool draw_edge_labels_priv;

signals:
    void newNodeCreated( int number);

public slots:
	void deleteNode( int number);
};
#endif
