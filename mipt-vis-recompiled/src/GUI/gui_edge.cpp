/**
 * File: GUI/GuiEdge.cpp - Implementation of GuiEdge in MiptVis
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */

#include "gui_impl.h"
#include <QtWidgets/QGraphicsLineItem>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsItem>

/**
 * Constructor of GuiEdge class
 */
#if 0 /* old */
GuiEdge::GuiEdge(  GuiGraph * graph_p, int _id, StyleSheet* ss, GuiNode * startItem, GuiNode * endItem, 
                 QGraphicsItem * parent, QGraphicsScene * scene)
            :QGraphicsItem( parent , scene), EdgeAux( ( GraphAux *)( graph_p), _id, 
            static_cast< NodeAux *>( startItem), static_cast< NodeAux *>( endItem)),
			EdgeProperties (ss)
#endif
GuiEdge::GuiEdge(  GuiGraph * graph_p, int _id, StyleSheet* ss, GuiNode * startItem, GuiNode * endItem, 
                 QGraphicsItem * parent, QGraphicsScene * scene)
            :QGraphicsItem( parent ), EdgeAux( ( GraphAux *)( graph_p), _id, 
            static_cast< NodeAux *>( startItem), static_cast< NodeAux *>( endItem)),
			EdgeProperties (ss)
{
    QGraphicsItem::setCursor( Qt::ArrowCursor);
    setFlag( QGraphicsItem::ItemIsSelectable, true);
}
/**
 * Destructor of GuiEdge class
 */
GuiEdge::~GuiEdge()
{
	out( "last nitems:%d, deletenig: ", addGui ( getGraph())->items().count());
	debugPrint();
	addGui( getGraph())->removeItem( addGui( this));
	out( "current nitems%d", addGui( getGraph())->items().count());
}


QLineF::IntersectType getIntersection (const QLineF& l, const QPolygonF& p, QPointF* intersectPoint)
{
    QPointF p1 = p.first();
    QPointF p2;
    QLineF polyLine;
    for ( int i = 1; i < p.count(); ++i)
    {
        p2 = p.at(i);
        polyLine = QLineF( p1, p2);
        QLineF::IntersectType intersectType = polyLine.intersect( l, intersectPoint);
        if ( intersectType == QLineF::BoundedIntersection)
        {
            return QLineF::BoundedIntersection;
        }
        p1 = p2;
    }
	return QLineF::NoIntersection;
}

/**
 * Update position the edge
 */
void GuiEdge::updatePosition (bool original_run)
{
	GuiNode* pre = addGui (pred());
	GuiNode* suc = addGui (succ());

	edge_start_point_priv = mapFromItem( pre, pre->width()/2, pre->height()/2);
    edge_end_point_priv = mapFromItem( suc, suc->width()/2, suc->height()/2);//!!!rarely it not work

	if (pre == suc)//mesh edge
	{
		QPointF heigh (0, 2*pre->height());

		QPointF middle (pre->pos().x() - 10, pre->pos().y() + pre->height()/2);
		QPointF middleDirUp = middle + heigh;
		QPointF middleDirDown = middle - heigh;

		edge_start_dir_priv = edge_start_point_priv + heigh;
		edge_end_dir_priv = edge_end_point_priv - heigh;

		QPolygonF polygon = suc->polygon();
		polygon.translate (suc->pos());
		getIntersection (QLineF (edge_start_point_priv, edge_start_dir_priv), polygon, &edge_start_point_priv);
		getIntersection (QLineF (edge_end_point_priv, edge_end_dir_priv), polygon, &edge_end_point_priv);

		QPainterPath path;
		path.moveTo (edge_start_point_priv);
		path.cubicTo (edge_start_dir_priv, middleDirUp, middle);
		path.cubicTo (middleDirDown, edge_end_dir_priv, edge_end_point_priv);
		edge_curve_priv = path;
	}
	else
	{
		edge_valid_priv = true;
		
		QPolygonF headPolygon = suc->polygon();
		headPolygon.translate (suc->pos());
		QPolygonF tailPolygon = pre->polygon();
		tailPolygon.translate (pre->pos());


		if (suc->real())
			edge_valid_priv = edge_valid_priv && getIntersection (QLineF (edge_start_point_priv, edge_end_point_priv), headPolygon, &edge_end_point_priv) == QLineF::BoundedIntersection;
		if (pre->real()) 
			edge_valid_priv = edge_valid_priv && getIntersection (QLineF (edge_start_point_priv, edge_end_point_priv), tailPolygon, &edge_start_point_priv) == QLineF::BoundedIntersection;

		QPointF delta = edge_start_point_priv - edge_end_point_priv;
		delta.setX(0);

		if (pre->real()) 
			edge_start_dir_priv = (edge_start_point_priv + edge_end_point_priv)/2;
		else
		{
			if (pre->firstPred() != 0)
				edge_start_dir_priv = (suc->coor() - pre->firstPred()->pred()->coor())/8 + pre->coor();
			else
				edge_start_dir_priv = edge_start_point_priv - delta/2;
		}

		if (suc->real())
			edge_end_dir_priv = (edge_start_point_priv + edge_end_point_priv)/2;
		else
			if (suc->firstSucc() != 0)
				edge_end_dir_priv = (pre->coor() - suc->firstSucc()->succ()->coor())/8 + suc->coor();
			else
				edge_end_dir_priv = edge_end_point_priv + delta/2;

		QPainterPath path;
		path.moveTo (edge_start_point_priv);
		path.cubicTo (edge_start_dir_priv, edge_end_dir_priv, edge_end_point_priv);

		if (edge_valid_priv) edge_curve_priv = path;
	}

    edge_top_left_priv.setX( min< qreal>( edge_start_point_priv.x(), edge_end_point_priv.x()));
    edge_top_left_priv.setY( min< qreal>( edge_start_point_priv.y(), edge_end_point_priv.y()));
    edge_bottom_right_priv.setX( max< qreal>( edge_start_point_priv.x(), edge_end_point_priv.x()));
    edge_bottom_right_priv.setY( max< qreal>( edge_start_point_priv.y(), edge_end_point_priv.y())); 

	if (original_run)
	{
		if (Edge::succ() && !addAux (Edge::succ())->real() && Edge::succ()->firstSucc())
			addGui (Edge::succ()->firstSucc())->updatePosition (false);//avoidance sharp corners
		
		if (Edge::pred() && !addAux (Edge::pred())->real() && Edge::pred()->firstPred())
			addGui (Edge::pred()->firstPred())->updatePosition (false);//avoidance sharp corners
		
		GuiEdge* i = this;
		for (i; !i->startEdge(); i->pred()->firstPred() && (i = addGui (i->pred()->firstPred())));
		i->updateLabels();
	}
    prepareGeometryChange();
}

/**
 * Update text distribution between edges
 */
void GuiEdge::updateLabels()
{
	float sum_len = 0;
	QPoint end_sequence;

	bool first_run = full_label.length() == 0;

	for (GuiEdge* i = this;;
		i->succ()->firstSucc() && (i = addGui (i->succ()->firstSucc())))
	{
		sum_len += i->length();
		if (first_run) full_label += i->edgeLabel();
		if (i->succ() && i->succ()->real())
		{
			end_sequence = addAux(i->succ())->coor();
			break;
		}
	}

	float n_letters = 0;
	float last_round_up = 0;
	int full_len = full_label.length();
	
	QPoint delta = end_sequence - edge_start_point_priv.toPoint();
	reverse = -delta.x() > delta.y();

	QString buf = full_label;

	for (GuiEdge* i = this;;
		i->succ()->firstSucc() && (i = addGui (i->succ()->firstSucc())))
	{
		i->reverse = reverse;
		n_letters = full_len*i->length()/sum_len + last_round_up;	//give an appropriate substring
		last_round_up = n_letters - (int)n_letters;

		if (i->succ() && i->succ()->real())//take the last character
			n_letters ++;

		if (reverse)
		{
			i->setEdgeLabel (buf.right (n_letters));
			buf.remove (buf.length() + 1 - n_letters, n_letters);
		}
		else
		{
			i->setEdgeLabel (buf.left (n_letters));
			buf.remove (0, n_letters);
		}
		if (i->succ() && i->succ()->real()) break;

	}
}

/**
 * Return lenght of the edge
 */
float GuiEdge::length() const
{
	return edge_valid_priv ? edge_curve_priv.length() : 0;
}

/**
 * Return if this edge first in a sequence
 */
bool GuiEdge::startEdge() const
{//		   pred exists and, it real					or it is a root
	return Edge::pred() && (addAux (Edge::pred())->real() || addAux (Edge::pred())->isRoot());
}

/**
 * Return bounding rectangle
 */
QRectF GuiEdge::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(edge_top_left_priv,
                   QSizeF( edge_bottom_right_priv.x() - edge_top_left_priv.x(),
                           edge_bottom_right_priv.y() - edge_top_left_priv.y()))
           .normalized()
           .adjusted(-adjust, -adjust, adjust, adjust);
}

/**
 * Shape
 */
QPainterPath GuiEdge::shape() const
{
//    QPainterPath path( edge_start_point_priv);
    QPainterPathStroker stroker;
//    path.lineTo( edge_end_point_priv.x(), edge_end_point_priv.y());
    stroker.setWidth( 10);
    return stroker.createStroke( edge_curve_priv);
}

/**
 * Paint arrow
 */
void drawLineHead (QPainter * painter, QPointF end, double angle, double size, bool figure)
{
    QPointF lineP1 = end + QPointF( sin( angle + Pi / 3) * size, cos( angle + Pi / 3) * size);
    QPointF lineP2 = end + QPointF( sin( angle + Pi - Pi / 3) * size, cos( angle + Pi - Pi / 3) * size);
	QPointF centre = (lineP1 + lineP2 + end)/3;

	if (figure)
	{
		QPainterPath arrow;
		arrow.moveTo (end);
		arrow.quadTo (centre, lineP1);
		arrow.lineTo (lineP2);
		arrow.quadTo (centre, end);

		painter->drawPath (arrow);
	}
	else
	{
		QPolygonF lineHead;
		lineHead.clear();
		lineHead << end << lineP1 << lineP2;
	
		painter->drawPolygon (lineHead);
	}
}

/**
 * Paint edge
 */
void GuiEdge::paint( QPainter * painter,
                         const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	if ( succ() == 0 || pred() == 0)//!!! What is the ugliness!
	{
		out ("ERROR: the deleted edge is tried to paint!");
		return;
	}
	if (!edge_valid_priv) return;

	edgeApplStyle (painter, option);

    qreal arrowSize = 10;
//    painter->setPen( QPen( Qt::darkRed, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	GuiNode* suc = addGui ( succ());
	GuiNode* pre = addGui ( pred());
	
	if (isVirtualRootEdge() && !addGui (graph)->showVnodes()) return;//Do not show virtual root edges
	if( suc->real())
    {
		QPointF dir = (7*edge_end_dir_priv + edge_start_point_priv)/8 - edge_end_point_priv;//!!! Mnemonic rule, it must be changed
		drawLineHead (painter, edge_end_point_priv, -atan2 (dir.y(), dir.x()), 10, false);
    }

	if (edgeLabel().size() && addGui (graph)->showEdgeLabels())
	{
		drawText (painter);
	}

	painter->setBrush( Qt::transparent);//!!! change it to black, and you will see, what heppend. I can't explain this
	painter->drawPath (edge_curve_priv);
}

/**
 * Draws a label near the edge
 */
void GuiEdge::drawText (QPainter * painter) const
{
	int len = edgeLabel().length();
	float k = 0.8f;
	float start = 0.1f;
	if (Edge::pred() && !addAux (Edge::pred())->real())//press letters to the virtual nodes
	{
		k += start;
		start = 0;
	}
	if (Edge::succ() && !addAux (Edge::succ())->real())//press letters to the virtual nodes
	{
		k = 1 - start;
	}
	if (reverse)
	{
		k = -k;
		start = 1 - start;
	}
	QPointF delta = edge_end_point_priv - edge_start_point_priv;
	float letter_size = 1.3*edge_curve_priv.length()/len;
	if (letter_size > 25) letter_size = 25;
	if (letter_size > 8 && !cycle())
	{

		for (int i = 0; i < len; ++i)
		{
			float slope = edge_curve_priv.slopeAtPercent (start + k*float(i)/len);
			float ang = atan(slope);
			QPointF pos = edge_curve_priv.pointAtPercent(start + k*float(i)/len);
			pos += QPointF (cos (Pi/2 + ang), sin(Pi/2 + ang))*letter_size;
			QFont curf = painter->font();
			curf.setPixelSize (letter_size);
			painter->setFont (curf);
			painter->drawText (pos, QString(edgeLabel().at(i)));
		}
	}
}

/**
 * Handle mouse press event
 */
void GuiEdge::mousePressEvent( QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent( event);
}

/**
 * Handle mouse release event
 */
void GuiEdge::mouseReleaseEvent( QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent( event);
}

/**
 * Handle mouse double click event
 */
void GuiEdge::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseDoubleClickEvent( event);
	insertNode( event->pos());
}

/**
 * Insert node
 */
GuiNode* GuiEdge::insertNode ( QPointF p)	//!!! I think it's superannuated
{
	GuiNode* node = addGui ( graph->insertNodeOnEdge( this));
	
	node->setReal( false);
	node->update();
	node->setPos( p.x(), p.y());
	return node;
}
/**
 * Write edge by xml writer
 */
void GuiEdge::writeByXmlWriter( xmlTextWriterPtr writer)
{
	EdgeAux::writeByXmlWriter( writer);
 /* wrong	xmlTextWriterWriteAttribute( writer, BAD_CAST "label", BAD_CAST edgeLabel().toAscii().data());*/
	xmlTextWriterWriteAttribute( writer, BAD_CAST "label", BAD_CAST edgeLabel().toLatin1().data());
	xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "prob", "%d", prob());
	if (0 != edgeStName().compare ("default", Qt::CaseInsensitive)){
	/* wrong	xmlTextWriterWriteAttribute( writer, BAD_CAST "style", BAD_CAST edgeStName().toAscii().data());*/
		xmlTextWriterWriteAttribute( writer, BAD_CAST "style", BAD_CAST edgeStName().toLatin1().data());
	}
}

/**
 *  Read from xml
 */
void GuiEdge::readByXml( xmlNode * cur_node)
{
	EdgeAux::readByXml( cur_node);
	for ( xmlAttr * props = cur_node->properties; props; props = props->next)
	{
		if ( xmlStrEqual( props->name, xmlCharStrdup( "prob")))
		{
			setProb( strtoul( ( const char *)( props->children->content), NULL, 0) );
		} else if ( xmlStrEqual( props->name, xmlCharStrdup( "style")))
		{
			setEdgeStyle( ( char *)( props->children->content));
		} else if ( xmlStrEqual( props->name, xmlCharStrdup( "label")))
		{
			setEdgeLabel( ( char *)( props->children->content));
		}
	}
}
