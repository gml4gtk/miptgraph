/**
 * @file: GEdge.cpp
 */

#include <QPainter>
#include <QPainterPath>
#include <QPainterPathStroker>

#include "GEdge.h"
#include "GNode.h"
#include "mainscene.h"

#include <math.h>
#include <stdio.h>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

GEdge::GEdge(GNode *sourceNode, GNode *destNode) : arrowSize(10)
{
    //setAcceptedMouseButtons(1);
    is_from_dummy_node = sourceNode->IsDummy();
    is_to_dummy_node = destNode->IsDummy();
    source = sourceNode;
    dest = destNode;
    source->addEdge(this);
    dest->addEdge(this);
    adjust();
    pressed = false;
    m_composite_edges = NULL;
}

GNode *GEdge::sourceNode() const
{
    return source;
}

GNode *GEdge::destNode() const
{
    return dest;
}

void GEdge::adjust()
{
    if (!source || !dest)
        return;

    QLineF * line = new QLineF (source->x(), source->y(),
                                dest->x(), dest->y());

    qreal length = line->length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line->dx() * 10) / length, (line->dy() * 10) / length);

        if (is_from_dummy_node)
            sourcePoint = line->p1();
        else
            sourcePoint = line->p1() + edgeOffset;

        if (is_to_dummy_node)
            destPoint = line->p2();
        else
            destPoint = line->p2() - edgeOffset;
    } else {
        sourcePoint = destPoint = line->p1();
    }
}

QRectF GEdge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    if (is_from_dummy_node)
        return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                          destPoint.y() - sourcePoint.y()))
            .normalized()
            .adjusted(0, 0, extra, extra);
    else if (is_to_dummy_node)
        return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                          destPoint.y() - sourcePoint.y()))
            .normalized()
            .adjusted(-extra, -extra, 0, 0);
    else if (is_from_dummy_node && is_to_dummy_node)
        return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                          destPoint.y() - sourcePoint.y()))
            .normalized()
            .adjusted(0, 0, 0, 0);
    else
        return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                          destPoint.y() - sourcePoint.y()))
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

QPainterPath GEdge::shape() const {
    QPainterPath path(sourcePoint);
    QPainterPathStroker stroker;
    path.lineTo(destPoint);
    stroker.setWidth(20);
    return stroker.createStroke(path);
}

void GEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself.
    if (pressed)
        painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    else
        painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    // TODO(Lega):: add here drawing a curve using QPainterPath.quadTo if edge is composite.
    painter->drawLine(line);

    // Draw the arrows.
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,
                                                      cos(angle + Pi / 3) * arrowSize);
    QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                  cos(angle + Pi - Pi / 3) * arrowSize);
    if (pressed)
        painter->setBrush(Qt::red);
    else
        painter->setBrush(Qt::black);
    if (is_to_dummy_node == true && is_from_dummy_node == false)
       painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    if (is_to_dummy_node == false && is_from_dummy_node == false)
        painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
}

void GEdge::Update(QGraphicsSceneMouseEvent *event) {
    pressed = !pressed;
    update();
    if (event)
        QGraphicsItem::mousePressEvent(event);
}

void GEdge::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (m_composite_edges)
        for (std::list<GEdge *>::iterator edge_iter = m_composite_edges->begin();
             edge_iter != m_composite_edges->end();
             edge_iter++)
            (*edge_iter)->Update(event);
    else {
        pressed = !pressed;
        update();
    }
    QGraphicsItem::mousePressEvent(event);
}

void GEdge::SetCompositeEdges (const std::list<GEdge *> * composite) {
    if (m_composite_edges) {
        for (std::list<GEdge *>::iterator iter = m_composite_edges->begin();
        iter != m_composite_edges->end();
        iter++)
            (*iter) = NULL;
        delete m_composite_edges;
    }
    m_composite_edges = const_cast<std::list<GEdge *> *>(composite);
}
