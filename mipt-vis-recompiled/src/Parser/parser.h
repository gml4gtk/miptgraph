/**
 * File: parser.h - Parser class definition.
 * Copyright (C) 2009  MIPTVIS
 */
#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <map>
#include <string>
#include <list>
#include <algorithm>
#include <iostream>
#include <QtCore/QString>

//#include "../Graph/graph_iface.h"
//#include "../GUI/gui_iface.h"
#include "graph_parser.h"
#include "../Utils/utils_iface.h"


/**
 * Implementation of Basic Block
 */
typedef list < int> Lint;

class BBlock
{
	Lint preds, succs;             //Predcessors & successors
	bool instd_preds, instd_succs; //Preds & succs installed?
	int def, impl;                 //String names of definition and implementation, first-start, second-end
	string text;                   //Implementaton of basic block
	bool instd_text;               //Implementation installed?

public:
	BBlock( int line);
	BBlock() {}
	bool isFinished();
	void setPredsInstd() { instd_preds = true; } //Set pedcessors status to "Installed"
	void setSuccsInstd() { instd_succs = true; } //Same for successors
	void addPred( int pred) { preds.push_back( pred); setPredsInstd(); }
	void addSucc( int succ) { succs.push_back( succ); setSuccsInstd(); }
	void addText( string & txt, int line);
	const char * getText();
	Lint::const_iterator getFirstSucc() { return succs.begin(); }
	Lint::const_iterator getEndSucc() { return succs.end(); }
};


/**
 * Implementation of function
 */
typedef map < int, BBlock> BBlocks;

class Function
{
	BBlocks blocks;
	int lastAdded;

	BBlocks::const_iterator getFirstBBlock() { return blocks.begin(); }
	BBlocks::const_iterator getEndBBlock() { return blocks.end(); }

public:
	Function() { lastAdded = -1; }
	BBlock * addBBlock( int number, int line, BBlock * bb = 0);
	BBlock * getBBlock( int number);
	ParserGraph * getGraph();
};

/**
 * Imnplementaton of DumpInfo
 */
typedef map< string, Function> Functions;

class DumpInfo
{
	Functions funcs;

public:
	/*
	Function * addFunction( string & name = string("main"));
	Function * getFunction( string & name = string("main"));
	BBlock * addBBlock( int number, int line, string & name = string("main"), BBlock * bb = 0);
	BBlock * getBBlock( int num, string & name = string("main"));
	*/
	Function * addFunction( string & name );
	Function * getFunction( string & name );
	BBlock * addBBlock( int number, int line, string & name , BBlock * bb = 0);
	BBlock * getBBlock( int num, string & name );
	ParserGraph * getGraph( const char *fname = NULL);
	list < string> & getFunctionList();
};


/**
 * Exception types for errors handling
 */
class exSomething
{
public:
	virtual void print_error( ostream &os)
	{
		os << "General error!" << endl;
	}
};

class exNotFound: public exSomething
{
	int  number;
	string fnName;

public:
	exNotFound( int num) { number = num; }
	exNotFound( string & name) { fnName = name; number = -1; }

	void print_error( ostream &os)
	{
		if ( number == -1)
			os << "Function " << fnName;
		else 
			os << "Basic block " << number;

		os << " not found!" << endl;
	}

};

class exAlreadyExists: public exSomething
{
	int number;
	string fnName;

public:
	exAlreadyExists( int num) { number = num; }
	exAlreadyExists( string & name) { fnName = name; number = -1; }

	void print_error( ostream &os)
	{
		if ( number == -1)
			os << "Function " << fnName;
		else 
			os << "Basic block " << number;

		os << " already exists!" << endl;
	}

};

class exIncorrectSequence: public exSomething
{
	int current, next;

public:
	exIncorrectSequence( int notClosed, int added)
	{
		current = notClosed;
		next = added;
	}

	void print_error( ostream &os)
	{
		os << "Basic block " << current << " not finished, but basic block "\
			<< next << " opened!" << endl;
	}
};


/**
 * RegExp patterns for GCC
 */

const QString gcc_patt_fn = "^(;;\\sFunction\\s)(.+) .*";
const QString gcc_patt_defbb = "^(Basic\\sblock\\s)(\\d+).*";
const QString gcc_patt_implbb = "^(;;\\sStart\\sof\\sbasic\\sblock\\s)(?:\\(.+\\)\\s->\\s)?(\\d+).*";
const QString gcc_patt_implbbend = "^(;;\\sEnd\\sof\\sbasic\\sblock\\s)(\\d+).*";
const QString gcc_patt_predstart = "^Predecessors:\\s+((ENTRY.*)|$)";
const QString gcc_patt_succend = "^Successors:\\s+((EXIT.*)|$)";
const QString gcc_patt_edges = "(\\d+)(?:\\s+\\[\\S+\\])?(?:\\s+\\(\\S+\\))?"; //"^(\\d+)(\\[.+\\])?([^\\d]*)";

/**
 * RegExp patterns for ICC
 */

const QString icc_patt_bb = "^(BBLOCK\\s)(\\d+).*";
const QString icc_patt_predstart = "^preds:.*";
const QString icc_patt_succend = "^succs:.*";
const QString icc_patt_edges = "(\\d+)([^\\d]*)";
const QString icc_patt_context = "^context:.*";
const QString icc_patt_root_context = "^Root\\sContext.*";
const QString icc_patt_fnname = "^\\s*\\d+\\s+\\d+.*\\sentry.*\\s([a-zA-Z_][a-zA-Z0-9_]*)((\\(.*\\);)|$)";


enum CompilerType  
{
	unknown,
	GCC,
	ICC
};

/**
 * Main class of Parser tool
 */
class Parser
{
public:
	Parser(){}
	virtual ~Parser(){}
    bool parseFile( string filename);
	static CompilerType getCompilerType( const char * file);
	list < string> getFunctionList() { return dump_info.getFunctionList(); }
	ParserGraph * getGraph( const char *fname = NULL) { return dump_info.getGraph( fname); }

protected:
    virtual bool parseFromStream( istream & is) = 0;
	DumpInfo dump_info;
};


class Gcc_parser: public Parser
{
private:
	string fnName( string &in);
	int defbbNum( string &in);
	int implbbNum( string &in);
	int implbbEnd( string &in);
	void getPred( string &in, BBlock &bb);
	void getSucc( string &in, BBlock &bb);
	
public:
	bool parseFromStream( istream & is);
	list < string> & getFunctionList() { return dump_info.getFunctionList(); }
	Gcc_parser() { }
	~Gcc_parser() {	}
};


class Icc_parser: public Parser
{
private:
	int bbNum( string &in);
	bool getPred( string &in, BBlock &bb);
	bool getSucc( string &in, BBlock &bb);
	bool isContext( string &in);
	bool isRootContext( string &in);
	string isFnName( string &in);
	
public:
	bool parseFromStream( istream & is);
	Icc_parser() { }
	~Icc_parser() {	}
};

#endif //PARSER_H