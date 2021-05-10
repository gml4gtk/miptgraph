/**
 * @file: GNode.cpp
 */

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>

#include <stdlib.h>

#include "../Gui/dumpview.h"
#include "../Gui/GEdge.h"
#include "../Gui/GNode.h"

GNode::GNode(QMainWindow * graphWidget, bool dummy,
             int m_id,
             const QString * source)
{
    m_graphWidget = graphWidget;
    is_dummy = dummy;
    clicked = false;
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    id = m_id;
    m_source.clear();
    if (source)
        m_source += (*source);
}

void GNode::addEdge(GEdge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<GEdge *> GNode::edges() const
{
    return edgeList;
}

bool GNode::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

QRectF GNode::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10 - adjust, -10 - adjust,
                  30 + adjust, 30 + adjust);
}

QPainterPath GNode::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void GNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20);

    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        if (is_dummy) {
            gradient.setColorAt(1, QColor(Qt::red).light(120));
            gradient.setColorAt(0, QColor(Qt::darkRed).light(120));
        } else {
            gradient.setColorAt(1, QColor(Qt::yellow).light(120));
            gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
        }
    } else {
        if (is_dummy) {
            gradient.setColorAt(0, Qt::red);
            gradient.setColorAt(1, Qt::darkRed);
        } else {
            gradient.setColorAt(0, Qt::yellow);
            gradient.setColorAt(1, Qt::darkYellow);
        }
    }
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(Qt::black);
    painter->drawText(QRectF(-7, -7, 20, 20), Qt::AlignCenter, QString::number(id));
}

void GNode::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        DumpView * nodeWindow = new DumpView(id ,m_source, (QWidget *)(m_graphWidget));
        nodeWindow->SetText(m_source);
        nodeWindow->show();
    }
}

void GNode::calculateForces()
 { return;
     if (!scene() || scene()->mouseGrabberItem() == this) {
         newPos = pos();
         return;
     }

     // Sum up all forces pushing this item away
     qreal xvel = 0;
     qreal yvel = 0;
     foreach (QGraphicsItem *item, scene()->items()) {
         GNode *node = qgraphicsitem_cast<GNode *>(item);
         if (!node)
             continue;

         QPointF vec = mapToItem(node, 0, 0);
         qreal dx = vec.x();
         qreal dy = vec.y();
         double l = 2.0 * (dx * dx + dy * dy);
         if (l > 0) {
             xvel += (dx * 150.0) / l;
             yvel += (dy * 150.0) / l;
         }
     }

     // Now subtract all forces pulling items together
     double weight = (edgeList.size() + 1) * 10;
     foreach (GEdge *edge, edgeList) {
         QPointF vec;
         if (edge->sourceNode() == this)
             vec = mapToItem(edge->destNode(), 0, 0);
         else
             vec = mapToItem(edge->sourceNode(), 0, 0);
         xvel -= vec.x() / weight;
         yvel -= vec.y() / weight;
     }

     if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
         xvel = yvel = 0;

     QRectF sceneRect = scene()->sceneRect();
     newPos = pos() + QPointF(xvel, yvel);
     newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
     newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
 }

QVariant GNode::itemChange(GraphicsItemChange change, const QVariant &value) {
    switch (change) {
        case ItemPositionHasChanged:
            foreach (GEdge *edge, edgeList) {
                if (edge->sourceNode() == this)
                    edge->SetSource(&(this->pos()));
                else
                    edge->SetDest(&(this->pos()));
                edge->adjust();
            }
            break;
        default:
            break;
    };

    return QGraphicsItem::itemChange(change, value);
}

