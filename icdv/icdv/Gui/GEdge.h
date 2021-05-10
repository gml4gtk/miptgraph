/**
 * @file: GEdge.h
 */
#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>

#include <list>

class GNode;
class MainScene;

typedef GNode * pGNode;

class GEdge : public QGraphicsItem {
public:
    GEdge(GNode *sourceNode, GNode *destNode);

    GNode *sourceNode() const;
    GNode *destNode() const;

    void adjust();

    enum { Type = UserType + 2 };
    int type() const { return Type; }

    void SetFromDummy(bool from_dummy) {
        is_from_dummy_node = from_dummy;
    }

    void SetPressed (bool is_pressed) {
        pressed = is_pressed;
    }

    bool FromDummy() {
        return is_from_dummy_node;
    }

    bool ToDummy() {
        return is_to_dummy_node;
    }

    void SetToDummy(bool to_dummy) {
        is_to_dummy_node = to_dummy;
    }

    void SetDest (QPointF * dest) {
        if (!dest)
            return;
        destPoint.setX(dest->x());
        destPoint.setY(dest->y());
    }

    void SetSource (QPointF * source) {
        if (!source)
            return;
        sourcePoint.setX(source->x());
        sourcePoint.setY(source->y());
    }

    // Be careful with using of this function: it might produce a memory leak
    // when composite param is NULL and m_composite_edges isn't NULL;
    // TODO(Lega): add right memory free for situation above.
    void SetCompositeEdges (const std::list<GEdge *> * composite);

    const std::list<GEdge *> * CompositeEdges() {
        return m_composite_edges;
    }

    bool Composite() {
        return composite;
    }

    void SetComposite(bool is_composite) {
        composite = is_composite;
    }

    void Update(QGraphicsSceneMouseEvent *event);

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    QPainterPath shape() const;

private:
    bool pressed;
    bool composite;
    std::list<GEdge *> * m_composite_edges;
    bool reverse;
    bool is_from_dummy_node;
    bool is_to_dummy_node;
    GNode *source, *dest;
    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
};

#endif
