/**
 * File: GUI/GuiNode.h - Implementational header for GuiNode of GUI in MiptVis tool.
 * Copyright (C) 2009  MiptVis, Ivan Mashintsev
 */
#ifndef GUI_NODE_H
#define GUI_NODE_H
/* wrong: #include <QtGui/QGraphicsTextItem>*/
#include <QtWidgets/QGraphicsTextItem>
#include <QtGui/QtGui>
/* wrong: #include <QtGui/QApplication>*/
#include <QtWidgets/QApplication>
#include <QtCore/QLineF>
#include "../Layout/layout_iface.h"
#include "gui_style_sheet.h"
class GuiEdge;
class GuiNode;
class GuiGraph;
class GuiEdge;
class GuiPoint;
class GuiTextEdit;
class QDockWidget;
class QPushButton;
class QVBoxLayout;
class QWidget;

/**
 * Representation of various node attributes
 */
class NodeProperties
{
	char * node_label_priv;
	char * shape_priv;
	char * text_priv;

	StId node_style_priv;
	StyleSheet *node_style_sheet_priv;

public:
	NodeProperties (StyleSheet* sheet_style);
    /** Data reading */
	inline char * nodeLabel() const
    { 
        return node_label_priv;
    }
	inline char * shape() const
    { 
        return shape_priv;
    }
	inline char * textPriv() const
    { 
        return text_priv;
    }
	inline StId nodeStyle () const
    { 
        return node_style_priv;
    }
	inline const QString& nodeStName() const
    { 
        return node_style_sheet_priv->getStName (node_style_priv);
    }
	inline void nodeApplStyle (QPainter * painter, const QStyleOptionGraphicsItem * option)
    { 
        node_style_sheet_priv->applayStyle (node_style_priv, painter, option);
    }
    /** Data writing */
	inline void setNodeLabel( char * label) 
    {
        node_label_priv = label;
    }
	inline void setShape( char * shape) 
    { 
        shape_priv = shape;
    }
	inline void setTextPriv( char * textPriv) 
    { 
        text_priv = textPriv;
    }
	inline void setNodeStyle (char * stName) 
    { 
        node_style_priv = node_style_sheet_priv->getId (stName);
    }
};

/**
 * Class GuiNode.
 */
class GuiNode:public QGraphicsTextItem, public NodeAux, public NodeProperties
{
    Q_OBJECT

private:
    QPolygonF polygon_priv;
    qreal adjust_priv;
    friend class GuiGraph;
    friend class GraphAux;

protected:
	virtual void writeByXmlWriter( xmlTextWriterPtr writer);
	virtual void readByXml ( xmlNode * cur_node);
	virtual void contextMenuEvent( QGraphicsSceneContextMenuEvent *event);

public:
	GuiTextEdit *text_edit;
	QDockWidget *text_dock;

    QString node_text;
    enum { Type = QGraphicsItem::UserType + 1};
    GuiNode( QString * text, GuiGraph * graph_p, int _id, StyleSheet *sheet_style,
        QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
	virtual ~GuiNode();
    inline QPolygonF polygon() const
    {
        return polygon_priv;
    };
    inline void setAdjust( qreal adjust)
    {
        adjust_priv = adjust;
    };
	inline QString getNodeText()
    {
        return node_text;
    };
    QRectF boundingRect() const;

    int type() const
    {
        return Type;
    }
protected:
    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
    QVariant itemChange( GraphicsItemChange change, const QVariant &value);
    void focusInEvent ( QFocusEvent * event);
    void focusOutEvent ( QFocusEvent * event);
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * mouseEvent);
    void mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent);
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * event);
    void commitPos( int x, int y);
    void superScribe ( QColor color, QString text);

public slots:
    void setNodeText( const QString & str);
	void saveText(); //??
	void textChange(); //??
	void emitDelete();
	void changeMode();

signals:
	void deleteGuiNode( int number);
	void createNodeTextDock( int number);
};
#endif
