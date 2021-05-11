/**
 * File: gcc_parser.cpp - Implementation of parser for GCC compiler
 * Copyright (C) 2009 MIPT-VIS
 */

#include "parser.h"
#include "QtCore/QString"
#include "QtCore/QRegExp"

int  Icc_parser::bbNum(std::string &in)
{
	QRegExp regexp;
	QString str(in.c_str());

	regexp.setPattern( icc_patt_bb);
	if ( regexp.indexIn( str) != -1)
		return regexp.cap( 2).toInt();
	else
		return -1;
}

string  Icc_parser::isFnName(std::string &in)
{
	QRegExp regexp;
	QString str(in.c_str());

	regexp.setPattern( icc_patt_fnname);
	if ( regexp.indexIn( str, 0) != -1)
		return regexp.cap( 1).toAscii().constData();
	else
		return "";
}

bool Icc_parser::getPred( string &in, BBlock &bb)
{
	QRegExp regex;
	QString str( in.c_str());
	int pos;
	
	regex.setPattern( icc_patt_predstart);
	pos = regex.indexIn( str);
	if( pos == -1)
		return false;

	int cnt = ( int)strlen( "preds: ");
	regex.setPattern( icc_patt_edges);
	while ( 1 )
	{
		pos = regex.indexIn( str, cnt);
		if ( pos == -1)
		{
			bb.setPredsInstd();
			break;
		}
		bb.addPred( regex.cap( 1).toInt());
		cnt += regex.matchedLength();
	}
	return true;
}


bool Icc_parser::getSucc( string &in, BBlock &bb)
{
	QRegExp regex;
	QString str( in.c_str());
	int pos;
	
	regex.setPattern( icc_patt_succend);
	pos = regex.indexIn( str);
	if( pos == -1)
		return false;

	int cnt = ( int)strlen( "succs: ");
	regex.setPattern( icc_patt_edges);
	while ( 1 )
	{
		pos = regex.indexIn( str, cnt);
		if ( pos == -1)
		{
			bb.setSuccsInstd();
			break;
		}
		bb.addSucc( regex.cap( 1).toInt());
		cnt += regex.matchedLength();
	}
	return true;
}

bool Icc_parser::isContext(std::string &in)
{
	QRegExp regexp;
	QString str(in.c_str());

	regexp.setPattern( icc_patt_context);
	return regexp.indexIn( str) != -1;
}

bool Icc_parser::isRootContext(std::string &in)
{
	QRegExp regexp;
	QString str(in.c_str());

	regexp.setPattern( icc_patt_root_context);
	return regexp.indexIn( str) != -1;
}

bool Icc_parser::parseFromStream(std::istream &is)
{
	int strnum = 0;
	bool isContextFound = false;
	BBlock * tbb = 0;
	string fnname;

	BBlock * zeroBlock = 0;
	int zeroBlockStr;

	//dump_info.addFunction();

	while ( !is.eof())
	{
		string current;
		int tmp;

		getline( is, current);
		strnum++;

		if ( ( tmp = bbNum( current)) != -1)
		{
			if ( tmp == 0) //Handling zero blocks for detecting function names
			{
				zeroBlock = new BBlock( strnum);
				zeroBlockStr = strnum;
				tbb = zeroBlock;
			}
			else
				tbb = dump_info.addBBlock( tmp, strnum, fnname);

			isContextFound = false;
			
			while ( !getPred( current, *tbb))
			{
				getline( is, current);
				strnum++;
			}

			while ( !getSucc( current, *tbb))
			{
				getline( is, current);
				strnum++;
			}
			
			continue;
		}

		if ( isRootContext( current))
			isContextFound = false;
		
		if ( isContextFound)
		{
            if ( !tbb)
                throw exNotFound( -1);

			string t;
			if ( ( t = isFnName(current))!= "" && zeroBlock)
			{
				fnname = t;
				dump_info.addFunction( fnname);
				tbb = dump_info.addBBlock( 0, zeroBlockStr, fnname, tbb);
				delete zeroBlock;
				zeroBlock = 0;
			}
			tbb->addText( current, strnum);
		}

		isContextFound |= isContext( current);
	}

	return true;
}
