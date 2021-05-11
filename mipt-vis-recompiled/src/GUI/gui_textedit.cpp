/**
 * File: GUI/GuiTextEdit.cpp - Implementation of TextEdit in MiptVis
 * Copyright (C) 2009  MiptVis, Le Manh Ha
 */
#include "gui_textedit.h"

/**
 * Constructor of GuiTextEdit class
 */
GuiTextEdit::GuiTextEdit()
{
	setOpenLinks( false);
}

/**
 * mouse press event
 */
void GuiTextEdit::mousePressEvent( QMouseEvent * mouseEvent)
{
	QTextEdit::mousePressEvent( mouseEvent);
	QTextCursor cursor = textCursor();
	cursor.select( QTextCursor::WordUnderCursor);
	QString str = cursor.selectedText();
	if ( !str.isEmpty())
	{
		QByteArray byteArr = str.toAscii();
		int i = 0, num = -1;
		while (( i < byteArr.size())&&(( byteArr.at(i) < '0')||( byteArr.at(i)> '9' ))) i++;
		if ( i<byteArr.size())
		{
			num = 0;
			while (( i < byteArr.size())&&( byteArr.at(i) >= '0')&&( byteArr.at(i)<= '9'))
			{
				num = num * 10 + (byteArr.at(i) - '0');
				i++;
			}
		}
		if ( num>=0) emit nodeToBeCentreOn( num);
	}
}
