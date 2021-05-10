#pragma once
#ifndef STYLE_SHEET_H
#define STYLE_SHEET_H
#include <QtGui/QPainter>
#include <Qt/QVector.h>
#include <Qt/QString.h>
#include <libxml/xmlwriter.h>
#include <Qt/QStyleOption.h>

#include "libXml/tree.h"
//struct xmlNode;

class Style
{
	QString name_priv;
	QPen pen;
	QBrush brush;

public:
	Style (QString name, QPen pen, QBrush brush);
	virtual void applayTo (QPainter * painter, const QStyleOptionGraphicsItem * option = 0) const;

	inline void rename (QString nname)
	{
		name_priv = nname;
	}

	inline const QString& name() const
	{
		return name_priv;
	}
	
	virtual void loadFromXmlNode (xmlNode * a_node);
	virtual void writeByXmlWriter( xmlTextWriterPtr writer);
};

class EdgeStyle
{
};

typedef int/*QList <Style*>::const_iterator*/ StId;

class StyleSheet
{
	QVector <Style*> sts;

public:
	StyleSheet();
	~StyleSheet();

	void loadFromXmlNode (xmlNode * a_node);
	void writeByXmlWriter( xmlTextWriterPtr writer);

	StId getId (const QString& name);
	void applayStyle (StId id, QPainter * painter, const QStyleOptionGraphicsItem * option = 0);
	void applayStyle (const QString& name, QPainter * painter, const QStyleOptionGraphicsItem * option = 0);
	const QString& getStName (StId id);
	StId addStyle (Style* st);
};

#endif // STYLE_SHEET_H