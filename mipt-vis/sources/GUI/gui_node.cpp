/**
 * File: GUI/GuiNode.cpp - Implementation of GuiNode in MiptVis
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */

#include "gui_impl.h"
#include<QtGui/QAction>
#include<QtGui/QMenu>
#include<QtGui/QMenu>

/**
 * Init NodeProperties
 */
NodeProperties::NodeProperties (StyleSheet* style_sheet_) :node_style_sheet_priv (style_sheet_), node_style_priv (style_sheet_->getId ("default"))
{
	node_label_priv = 0;
	shape_priv = 0;
	text_priv = 0;
}
/**
 * Constructor of GuiNode class
 */
GuiNode::GuiNode(  QString * text, GuiGraph * graph_p, int _id, StyleSheet* node_style_sheet_priv,
        QGraphicsItem * parent, QGraphicsScene * scene):
		NodeProperties (node_style_sheet_priv),
	    node_text(),
        adjust_priv(0),
        QGraphicsTextItem( parent, scene),
        NodeAux( static_cast<GraphAux *> ( graph_p), _id)
{
	QGraphicsItem::setCursor( Qt::ArrowCursor);
	setPlainText( *text);
	setTextWidth ( 100); //Set width of node
	NodeAux::setWidth( 100);
	setAdjust( real()? 3 : 1);
	setNodeText( "");
	setFlag( QGraphicsItem::ItemIsMovable, true); // Set node can move
	setFlag( QGraphicsItem::ItemIsSelectable, true); // Set node can select
	setTextInteractionFlags( Qt::NoTextInteraction);
	polygon_priv << (boundingRect().bottomLeft()) << (boundingRect().bottomRight())
				  << (boundingRect().topRight()) << (boundingRect().topLeft())
				  << (boundingRect().bottomLeft());
	text_dock = NULL;
}
/**
 * Destructor of GuiNode class
 */
GuiNode::~GuiNode()
{
	out( "last nitems:%d, deletenig: ", addGui( getGraph())->items().count());
	debugPrint();
	addGui( getGraph())->removeItem( addGui(this));
	out( "current nitems%d", addGui( getGraph())->items().count());
}

/**
 * When forus in the text of the node function run
 */
void GuiNode::focusInEvent( QFocusEvent * event)
{
    setSelected( true);
    QGraphicsTextItem::focusInEvent( event);
}

/**
 * When forus out the text of the node function run
 */
void GuiNode::focusOutEvent( QFocusEvent * event)
{
    setSelected( false);
    QGraphicsTextItem::focusOutEvent( event);
}

/**
 * Edit text in the node after double click mouse
 */
void GuiNode::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
	if ( text_dock==NULL)
	{
		emit createNodeTextDock( userId());
	}
	else
	{
		if (!text_dock->isVisible()) text_dock->show();
	}
    QGraphicsTextItem::mouseDoubleClickEvent( mouseEvent);
}

/**
 * Actions for mouse press event
 */
void GuiNode::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent)
{
	update();
    QGraphicsTextItem::mousePressEvent( mouseEvent);
}

/**
 * Actions for mouse release event
 */
void GuiNode::mouseReleaseEvent( QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsTextItem::mouseReleaseEvent( event);
}

/**
 * Draw rectangle around the node
 */
void GuiNode::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	nodeApplStyle (painter, option);
	if ( real())
	{
		painter->drawRect( boundingRect());
		QGraphicsTextItem::paint( painter, option, widget);
		polygon_priv << ( boundingRect().bottomLeft()) << ( boundingRect().bottomRight())
		                  << ( boundingRect().topRight()) << ( boundingRect().topLeft())
		                  << ( boundingRect().bottomLeft());
	}
	else
	{
		if (!addGui (graph)->showVnodes()) return;//do not draw virtual nodes
		painter->setPen( QPen( Qt::black, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		painter->drawRect (boundingRect());
		polygon_priv << ( boundingRect().bottomLeft()) << ( boundingRect().bottomRight())
		                  << ( boundingRect().topRight()) << ( boundingRect().topLeft())
		                  << ( boundingRect().bottomLeft());
	}
}

/**
 * Get bounding rectangle of the node
 */
QRectF GuiNode::boundingRect() const
{
	if (!real()) return QRectF (0, 0, 15, 15);//!!! magic namber
    return QGraphicsTextItem::boundingRect()
               .adjusted( -adjust_priv, -adjust_priv, adjust_priv, adjust_priv);
}


/**
 * If node's position is changed, run this function
 */
QVariant GuiNode::itemChange( GraphicsItemChange change, const QVariant &value)
{
    if( change == QGraphicsItem::ItemPositionChange 
		|| change == QGraphicsItem::ItemPositionHasChanged)
    {
        setWidth( boundingRect().width());
        setHeight( boundingRect().height());
        msetX( QGraphicsItem::x() + width()/2);
        msetY( QGraphicsItem::y() + height()/2);
        
		EdgeAux* iter;
		ForEdges( this, iter, Succ)
			addGui(iter)->updatePosition();
		ForEdges( this, iter, Pred)
			addGui( iter)->updatePosition();

    }
    return value;
}

/**
 *  commitPos
 */
void GuiNode::commitPos( int x, int y)
{
    msetX( x);
    msetY( y);

    setPos( x - width() / 2, y - height() / 2);

	EdgeAux* iter;
	ForEdges( this, iter, Succ)
		addGui( iter)->updatePosition();
	ForEdges( this, iter, Pred)
		addGui( iter)->updatePosition();
}

/**
 *  superscribe
 */
void GuiNode::superScribe ( QColor color, QString text)
{
//    setMyColor( color);
    setPlainText( text);
}

/**
 *  setMyText
 */
void GuiNode::setNodeText( const QString & str)
{
    node_text = str;
}

/**
 *  textChange
 */
void GuiNode::textChange()
{
    QByteArray strByteArray = getNodeText().toAscii();
    char *strChar;
    strChar = ( char*) calloc( strByteArray.size(),sizeof( char));
    if ( strChar==NULL) return;
    int i;
    for ( i=0; i<=strByteArray.size(); ++i) strChar[i] = strByteArray[i];
    setTextPriv( strChar);
}

/**
 *  Write to xml
 */
void GuiNode::writeByXmlWriter( xmlTextWriterPtr writer)
{
	NodeAux::writeByXmlWriter ( writer);
	if ( nodeLabel()) xmlTextWriterWriteAttribute( writer, BAD_CAST "label", BAD_CAST nodeLabel());
	if ( NodeProperties::shape())
		xmlTextWriterWriteAttribute( writer, BAD_CAST "shape", BAD_CAST NodeProperties::shape());

	if ( textPriv()) xmlTextWriterWriteAttribute( writer, BAD_CAST "textPriv", BAD_CAST textPriv());

	if (0 != nodeStName().compare ("default", Qt::CaseInsensitive))
		xmlTextWriterWriteAttribute( writer, BAD_CAST "style", BAD_CAST nodeStName().toAscii().data());
}

/**
 *  Read from xml
 */
void GuiNode::readByXml( xmlNode * cur_node)
{
	NodeAux::readByXml ( cur_node);
	setTextPriv( "");
	for ( xmlAttr* props = cur_node->properties; props; props = props->next)
	{
		if ( xmlStrEqual( props->name, xmlCharStrdup( "label")))
		{
			setNodeLabel( ( char *)( props->children->content));
		} else if ( xmlStrEqual( props->name, xmlCharStrdup( "shape")))
		{
			setShape( ( char *)( props->children->content));
		}
		
		else if ( xmlStrEqual( props->name, xmlCharStrdup( "textPriv")))
		{
			setTextPriv( ( char *)( props->children->content));
		}
		
		else if ( xmlStrEqual( props->name, xmlCharStrdup( "style")))
		{
			setNodeStyle( ( char *)( props->children->content));
		}
	}
}

/**
 *  contextMenuEvent
 */
void GuiNode::contextMenuEvent( QGraphicsSceneContextMenuEvent *event)
{
	QMenu menu;

	QAction *deleteAct = menu.addAction( "Delete");
	connect( deleteAct, SIGNAL( triggered()), this, SLOT( emitDelete()));

	QString mode_text;
	if ( textInteractionFlags() == Qt::NoTextInteraction) mode_text = QString( "Set Edit Mode");
	else mode_text = QString( "Set Drag Mode");

	QAction *editLabelAct = menu.addAction( mode_text);
	connect( editLabelAct, SIGNAL( triggered()), this, SLOT( changeMode()));

	menu.exec( event->screenPos());
}

/**
 *  emit delete
 */
void GuiNode::emitDelete()
{
	emit deleteGuiNode( this->userId());
}

/**
 *  save text
 */
void GuiNode::saveText()
{
	node_text = text_edit->toPlainText();
}

/**
 *  changeMode
 */
void GuiNode::changeMode()
{
	if ( textInteractionFlags() == Qt::NoTextInteraction)
	{ 
		setTextInteractionFlags( Qt::TextEditorInteraction);
	}
	else 
	{
		setTextInteractionFlags( Qt::NoTextInteraction);
	}
}
