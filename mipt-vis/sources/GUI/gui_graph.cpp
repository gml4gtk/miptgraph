/**
 * File: GUI/GuiGraph.cpp - Implementation of GuiGraph in MiptVis
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */

#include "gui_impl.h"

/**
 * Create element after double click mouse.
 */
void GuiGraph::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    update();
    if( mouseEvent->button() != Qt::LeftButton)
    {
        QGraphicsScene::mouseDoubleClickEvent( mouseEvent);
        return;  
    }
    if( selectedItems().isEmpty())
    {
        number_priv++;
        GuiNode * node = ( GuiNode *)newNode();
        node->setPos( mouseEvent->scenePos());
		node->setNodeLabel( "Node" + number_priv);
		node->setShape( "rectangle");
		node->setTextPriv( "");        
        node->setWidth( node->boundingRect().width());
        node->setHeight( node->boundingRect().height());
		node->msetX( node->QGraphicsItem::x() + node->width()/2);
		node->msetY( node->QGraphicsItem::y() + node->height()/2);

		QString text =  QString( "Node %1").arg( node->userId());
		node->setPlainText( text);

		QObject::connect( node, SIGNAL( deleteGuiNode( int)), this, SLOT( deleteNode( int)));
		emit newNodeCreated( node->userId());

        QGraphicsScene::mouseDoubleClickEvent( mouseEvent);
    }
    QGraphicsScene::mouseDoubleClickEvent( mouseEvent);
}

/**
 * Constructor for class GuiGraph.
 */
GuiGraph::GuiGraph( QObject * parent):mode_priv( insertRect), GraphAux(), QGraphicsScene( parent)
{
    number_priv = 0;
    line_priv = NULL;
}

/**
 * Constructor for class GuiGraph.
 */
GuiGraph::GuiGraph( char * filename, QObject * parent):mode_priv( insertRect), GraphAux(), QGraphicsScene( parent)
{
    GuiNode * node;
    GuiEdge * edge;
    number_priv = 0;
    line_priv = NULL;

    readFromXml( filename);

    for ( node = ( GuiNode *)firstNode(); isNotNullP( node); node = ( GuiNode *)node->nextNode())
	{
        node->setPos( node->NodeAux::x(), node->NodeAux::y());

		node->setNodeText( QString( node->textPriv()));

        node->msetX( node->QGraphicsItem::x());
        node->msetY( node->QGraphicsItem::y());
		node->setAdjust( node->real()? 3 : 1);

		QString text =  QString( "Node %1").arg( node->userId());
		node->setPlainText( text);

		QObject::connect( node, SIGNAL( deleteGuiNode( int)), this, SLOT( deleteNode( int)));
	}
	for ( edge = ( GuiEdge *)firstEdge(); isNotNullP( edge); edge = ( GuiEdge *) edge->nextEdge())
	{
        if ( edge != NULL)
        {
		    edge->updatePosition();
        }
	}
}
/**
 * Create line after right button clicked
 */
void GuiGraph::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
	setEnabledToNodes();
    if( mouseEvent->button() & Qt::RightButton)
    {
        mode_priv = insertLine;
        line_priv = new QGraphicsLineItem( QLineF( mouseEvent->scenePos(), mouseEvent->scenePos()));
        line_priv->setPen( QPen( Qt::black));
        addItem( line_priv);
    }
    update();
    QGraphicsScene::mousePressEvent( mouseEvent);
}

/**
 * Move line when move mouse.
 */
void GuiGraph::mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    update();
    if ( mode_priv == insertLine && line_priv != 0)
    {
        QLineF newLine( line_priv->line().p1(), mouseEvent->scenePos());
        line_priv->setLine( newLine);
    } 
    else if ( mode_priv == moveItem)
    {
        QGraphicsScene::mouseMoveEvent( mouseEvent);
    }
}

/**
 * Insert line on the scene after release the mouse.
 */
void GuiGraph::mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
    update();
    if ( line_priv != 0 && mode_priv == insertLine)
    {
        QList<QGraphicsItem *> startItems = items( line_priv->line().p1());
        if ( startItems.count() && startItems.first() == line_priv)
        {
            startItems.removeFirst();
        }
        QList<QGraphicsItem *> endItems = items( line_priv->line().p2());
        if ( endItems.count() && endItems.first() == line_priv)
        {
            endItems.removeFirst();
        }
        removeItem( line_priv);
        delete line_priv;
        if ( startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first() != endItems.first() && 
            startItems.first()->type() == GuiNode::Type &&
            endItems.first()->type() == GuiNode::Type)
			{
				GuiNode * startItem = qgraphicsitem_cast< GuiNode *> ( startItems.first());
				GuiNode * endItem = qgraphicsitem_cast< GuiNode *> ( endItems.first());
				GuiEdge * lin = (GuiEdge *)newEdge( startItem, endItem);
				setDisabledToNodes();
			}
    }
    line_priv = NULL;
    mode_priv = moveItem;
    QGraphicsScene::mouseReleaseEvent( mouseEvent);
	update();
}

/**
 * Read style table
 */
void GuiGraph::readAttribsFromXml (xmlNode * a_node)
{
	graph_style_sheet_priv.loadFromXmlNode (a_node);
	GraphAux::readAttribsFromXml (a_node);
}

/**
 * Read style table
 */
void GuiGraph::writeAttribsByXmlWriter (xmlTextWriterPtr writer)
{
	graph_style_sheet_priv.writeByXmlWriter (writer);
	GraphAux::writeAttribsByXmlWriter (writer);
}

/**
 * Allocation memory for edge
 */
EdgeAux * GuiGraph::createEdge( Node * pred, Node * succ)
{
    GuiEdge * e = new GuiEdge ( this, incEdgeId(), &graph_style_sheet_priv, ( GuiNode *)pred, ( GuiNode *)succ);
    addItem( e);
    e->setEdgeStyle( "Solid");
    e->setZValue( -1000.0);
    e->updatePosition();

    return static_cast <EdgeAux*> (e);
}

/**
 * Allocation memory for node
 */
NodeAux * GuiGraph::createNode()
{
    int num = incNodeId();
    
	QString text =  QString( "Node %1").arg( num);// number here !!!!!!!
    GuiNode * node_p = new GuiNode (&text, this, num, &graph_style_sheet_priv);

    node_p->setZValue( 1);
    addItem( node_p);

    return node_p;
}

/**
* Insert new node on edge in graph
*/
NodeAux * GuiGraph::insertNodeOnEdge (EdgeAux* e)
{
    NodeAux* n = GraphAux::insertNodeOnEdge( e);
	addGui (n->firstSucc())->setEdgeStyle (addGui(n->firstPred())->edgeStName());
    return n;
}
/**
 * adds remove item to graph::removeEdge
 */
void GuiGraph::removeEdge (Edge* e)
{
	removeItem (addGui (e));
	Graph::removeEdge (e);
}

/**
 * adds remove item to graph::removeNode
 */
void GuiGraph::removeNode (Node* n)
{
	removeItem( addGui (n));
	Graph::removeNode (n);
}

/**
 * Virtual nodes show switch
 */
void GuiGraph::switchVnodesShow()
{
	draw_virtual_nodes_priv = !draw_virtual_nodes_priv;
	update();
}

bool GuiGraph::showVnodes()
{
	return draw_virtual_nodes_priv;
}
/**
 * Edge labels show switch
 */
void GuiGraph::switchEdgeLabelsShow()
{
	draw_edge_labels_priv = !draw_edge_labels_priv;
	update();
}

bool GuiGraph::showEdgeLabels()
{
	return draw_edge_labels_priv;
}
/**
 * commit layout
 */
bool GuiGraph::applayLayout()
{
    GuiNode * node;
    GuiEdge * edge;

	for (NodeAux* iter = addAux( firstNode()); iter != 0;
		          iter = addAux( iter->nextNode()))
	{
		iter->commitPos (iter->x(), iter->y());
		if (!iter->real())
			iter->superScribe ( Qt::gray, "unreal");
	}

    for ( node = ( GuiNode *)firstNode(); isNotNullP( node); node = ( GuiNode *)node->nextNode())
	{
        node->setAdjust ( node->real()? 3 : 1);
	}
	for ( edge = ( GuiEdge *)firstEdge(); isNotNullP( edge); edge = ( GuiEdge *) edge->nextEdge())
	{
        if ( edge != NULL)
		    edge->updatePosition();
	}
	return true;
}

/**
 * delete node
 */
void GuiGraph::deleteNode( int number)
{
	GuiNode *node;
	for ( node = ( GuiNode *)firstNode(); isNotNullP( node); node = ( GuiNode *)node->nextNode())
		if ( node->userId() == number)
		{
			node->text_dock->hide();
			removeNode( node);
			break;
		}
}

/**
 * set enabled to nodes
 */
void GuiGraph::setEnabledToNodes()
{
	GuiNode *node;
	for ( node = ( GuiNode *)firstNode(); isNotNullP( node); node = ( GuiNode *)node->nextNode())
		{
			node->setEnabled( true);
		}
}

/**
 * set disabled to nodes
 */
void GuiGraph::setDisabledToNodes()
{
	GuiNode *node;
	for ( node = ( GuiNode *)firstNode(); isNotNullP( node); node = ( GuiNode *)node->nextNode())
		{
			node->setEnabled( false);
		}
}
