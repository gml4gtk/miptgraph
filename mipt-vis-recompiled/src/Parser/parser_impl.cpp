/**
 * File: parser_impl.cpp - Implementation of parser basic class and helper-classes
 * Copyright (C) 2009 MIPT-VIS
 */


#include "parser.h"
#include <QtWidgets/QWidget>
#include <QtWidgets/QFileDialog>
#include <QtCore/QObject>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QInputDialog>


/**
 * BBlock class methods
 */

BBlock::BBlock( int line) 
{
	def = line;
	instd_preds = instd_succs = false;
	instd_text = false;
	impl = -1;
}

bool BBlock::isFinished()
{
	return instd_preds &&
		   instd_succs;
	     // instd_text :fails on gcc dumps 
}

void BBlock::addText( string & txt, int line)
{
	if ( !text.empty())
		text += '\n';
	text += txt;
	instd_text = true;
	impl = ( impl == -1) ? line : impl;
}

const char * BBlock::getText() 
{
	return text.c_str();
}


/**
 * Function class methods
 */

BBlock * Function::addBBlock( int number, int line, BBlock * bb)
{
	if ( lastAdded != -1 && ! getBBlock( lastAdded)->isFinished())
		throw exIncorrectSequence(lastAdded, number);

	BBlocks::iterator it;

	it = blocks.find( number);
	if ( it != blocks.end())
		throw exAlreadyExists( line);

	BBlock * tmp;
	if ( bb)
		tmp = bb;
	else
		tmp = new BBlock( line);

	blocks.insert( pair< int, BBlock> ( number, *tmp));
	lastAdded = number;
	return &blocks[ number];
}

BBlock * Function::getBBlock( int number)
{
	map < int, BBlock>::iterator it;

	it = blocks.find( number);
	if ( it == blocks.end())
		throw exNotFound( number);
	return &it->second;
}

ParserGraph * Function::getGraph()
{
	ParserGraph * graph = new ParserGraph;
	map< int, ParserNode *> node_list;
	BBlocks::const_iterator i;

	for ( i = getFirstBBlock(); i != getEndBBlock(); i++)
		node_list[ i->first] = (ParserNode *)graph->newNode();


	map< int, ParserNode *>::const_iterator node_index;
	for ( node_index = node_list.begin();
		  node_index != node_list.end();
		  node_index++
		)
	{
		

		Lint::const_iterator to;
		BBlock * bb= getBBlock( node_index->first);

		node_list[node_index->first]->setColor( "Green");
		node_list[node_index->first]->setText( (char*)bb->getText());

		for ( to = bb->getFirstSucc();
			  to != bb->getEndSucc();
			  to++
			)
		{
			if ( *to != -1)
				graph->newEdge(node_list[ node_index->first], node_list[ *to]);
		}
	}

	return graph;
}


/**
 * Dump info class methods
 */
	/*
	Function * addFunction( string & name = string("main"));
	Function * getFunction( string & name = string("main"));
	BBlock * addBBlock( int number, int line, string & name = string("main"), BBlock * bb = 0);
	BBlock * getBBlock( int num, string & name = string("main"));
	*/

Function * DumpInfo::addFunction( string & name)
{
	if ( funcs.find( name) != funcs.end())
		throw exAlreadyExists( name);;
	return &funcs[ name];
}

Function * DumpInfo::getFunction( string & name)
{
	if ( funcs.find( name) == funcs.end())
		throw exNotFound( name);
	return &funcs[ name];
}

BBlock * DumpInfo::getBBlock( int num, string &name)
{
	return getFunction( name)->getBBlock( num);
}

BBlock * DumpInfo::addBBlock( int number, int line, string &name, BBlock * bb)
{
	return getFunction( name)->addBBlock( number, line, bb);
}

ParserGraph * DumpInfo::getGraph( const char *fname)
{
	string tmp = ( fname)? string( fname) : "main";
	return getFunction( tmp)->getGraph();
}

list < string> & DumpInfo::getFunctionList()
{
	Functions::const_iterator it;
	list < string> * ls = new list < string>;

	for( it = funcs.begin(); it != funcs.end(); it++)
		ls->push_back( it->first);

	return *ls;
}

/**
 * Parser class methods
 */

bool Parser::parseFile( string filename)
{
	ifstream is( filename.c_str(), ios::in);
	if ( is.is_open())
		return parseFromStream( is);
	else
		return false;
}

CompilerType Parser::getCompilerType( const char * file)
{
	string current;

	ifstream is( file, ios::in);
	if ( !is.is_open())
		return unknown;

	getline( is, current);
	if ( current.find( "After Building the Control Flow:") == 0)
		return ICC;

	getline( is, current);
	if ( current.find( ";; Function") == 0)
		return GCC;

	return unknown;
}

bool convertDumpToXml( QWidget * parent)
{
	Parser* P;
	ParserGraph * gr;
	CompilerType ct;
	QString file, item;

	Utils::out( "\nParser started");

	file = QFileDialog::getOpenFileName( parent, parent->tr( "Select file to convert"), 
		"", parent->tr( "All files(*.*)")); 
    if ( file.isEmpty()) return false;

	Utils::out( "Parsing file %s", file.toLatin1().constData());

	ct = Parser::getCompilerType( file.toLatin1().constData());
	if ( ct == GCC)
	{
		Utils::out( "Gcc compiler detected");
		P = new Gcc_parser();
	}
	else if ( ct == ICC)
	{
		Utils::out( "Icc compiler detected");
		P = new Icc_parser();
	}
	else
	{
		Utils::err( "Unknown compiler type");
		QMessageBox msgBox;
		msgBox.setText( parent->tr( "Unknown compiler type!"));
		msgBox.setIcon( QMessageBox::Critical);
		msgBox.setModal( true);
		msgBox.exec();

		return false;
	}

	try
	{
		P->parseFile( file.toLatin1().constData());

		QStringList items;
		list < string> functions;

		functions = P->getFunctionList();

		list < string>::const_iterator it;
		for( it = functions.begin(); it != functions.end(); it++)
			items << QString::fromStdString( *it);

		bool ok;
		item = QInputDialog::getItem( parent, parent->tr("Select function"),
			parent->tr("Function:"), items, 0, false, &ok);
		if (ok && !item.isEmpty())
			gr = P->getGraph( item.toLatin1().constData());
		else
			return false;
	}
	catch( exSomething & ex)
	{
		ex.print_error( std::cerr);
		return false;
	}

	Utils::out( "%s successfuly parsed", file.toLatin1().constData());

	file += "_";
	file += item.toLatin1().constData();
	file += ".xml";

	file = QFileDialog::getSaveFileName( parent, parent->tr( "Save XML"),
		file,	parent->tr( "XML files (*.xml)"));

	if ( !file.isEmpty())
	{
		gr->writeToXml( file.toLatin1().constData());
		Utils::out( "Dump %s wrote", file.toLatin1().constData());

		QMessageBox msgBox;
		QString msg = parent->tr( "The dump of function \"")
			+ item
			+ parent->tr("\" has been writen to ")
			+ file + ".";

		msgBox.setText( msg);
		msgBox.setModal( true);
		msgBox.exec();
	}

	Utils::out( "End of parser job\n");

    delete P;
    delete gr;

	return true;
}