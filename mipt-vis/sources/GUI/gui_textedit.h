/**
 * File: GUI/GuiTextEdit.h - Implementational header for TextEdit of GUI in MiptVis tool.
 * Copyright (C) 2009  MiptVis, Le Manh Ha
 */
#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QtGui/QTextBrowser>
#include "gui_impl.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

/**
 * Class GuiTextEdit
 */
class GuiTextEdit : public QTextBrowser
{
    Q_OBJECT

public:
    GuiTextEdit();
	QTextCharFormat text_format_with_anchor;
	QTextCharFormat text_format_without_anchor;
protected:
	void mousePressEvent( QMouseEvent * mouseEvent);
signals:
	void nodeToBeCentreOn( int nodeNumber);
};

#endif
