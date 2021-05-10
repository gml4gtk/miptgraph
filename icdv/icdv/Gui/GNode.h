/**
 * @file: GNode.h
 */

#ifndef NODE_H
#define NODE_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QList>

class LNode;
class GEdge;
class GraphWidget;
class QGraphicsSceneMouseEvent;

class GNode : public QGraphicsItem
{
public:
    GNode(QMainWindow * graphWidget, bool dummy,
          int id,
          const QString * source = NULL);

    void addEdge(GEdge *edge);
    QList<GEdge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    void calculateForces();
    bool advance();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    bool IsDummy () {
        return is_dummy;
    };

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    // void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool clicked;
    QString m_source;
    QList<GEdge *> edgeList;
    QPointF newPos;
    QMainWindow * m_graphWidget;

    // true, if it isn't visible node.
    bool is_dummy;
    int id;
};

#endif
