/**
 * File: gcc_parser.cpp - Definition of Graph class for Parser project
 * Copyright (C) 2009 MIPT-VIS
 */

#include "../Graph/graph_iface.h"

#ifndef PARSER_GRAPH_H_
#define PARSER_GRAPH_H_


class ParserNode : public Node
{
	char * text_priv;
	char * color_priv;

public:
	ParserNode( Graph * graph_p, int _id) : Node( graph_p, _id)
	{
		text_priv = 0;
		color_priv = 0;
	}

	void setText( char *text)
	{
		text_priv = text;
	}

	char * getText() const
	{
		return text_priv;
	}

	void setColor( char *color)
	{
		color_priv = color;
	}

	char * getColor() const
	{
		return color_priv;
	}

	void writeByXmlWriter( xmlTextWriterPtr writer);

};

class ParserGraph :	public Graph
{
	ParserNode * createNode()
	{
		return new ParserNode( (Graph *)this, incNodeId());
	}

public:
	ParserGraph() : Graph()
	{};

	void writeToXml( const char *filename);
};


#endif //PARSER_GRAPH_H_
