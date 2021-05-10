//
//	DumpParser.cpp
//

#include "Layout/Layout.h"
#include "DumpParser.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

/// Helper to parse numbers in sting
/*
 *
 */
unsigned int ParseDumpNumbs(const QString &qStr, list<unsigned int> &lst) {
	int pos = 0;
	QRegExp qRegExp;

	qRegExp.setMinimal(false);
	qRegExp.setPatternSyntax(QRegExp::RegExp2);
	qRegExp.setPattern("\\d+");

	while ((pos = qRegExp.indexIn(qStr, pos)) != -1) {
		pos += qRegExp.matchedLength();
		lst.push_back(qRegExp.cap(0).toUInt());
	}

	return (unsigned int)lst.size();
}

int ParseDump(LGraph * plGraph, const QString & filename) {
        // Opening and mapping dump file.
        QFile dump(filename);

	if (!dump.open(QIODevice::ReadOnly | QIODevice::Text)) {
		printf("%s\n", dump.errorString().toAscii().constData());
		return 1;
	}

	uchar * ptrMap = dump.map(0, dump.size(), QFile::NoOptions);

	if (dump.error()) {
		printf("%s\n", dump.errorString().toAscii().constData());
		dump.close();
		return 1;
	}
	
	QString map((const char*)ptrMap);

	dump.unmap(ptrMap);
	dump.close();

	int pos = 0;
	LNode *lNodeCur;
        QRegExp rgBBlk; // GerExp for parse Basic BLocK (BBlck)

	rgBBlk.setMinimal(true);
	rgBBlk.setPatternSyntax(QRegExp::RegExp2);
	rgBBlk.setPattern("BBLOCK([^0-9]+)(\\d+)(.*)preds:(.*)succs:(.*)exec:(.*)prob:(.*)context:(.*)attr:(.*)(BBLOCK|Root Context)(.*)");

//	The first iteration: building dependent.
	while((pos = rgBBlk.indexIn(map, pos)) != -1) {
		pos += rgBBlk.matchedLength() - 6;
		lNodeCur = plGraph->AddNode();
		lNodeCur->SetContent(rgBBlk.cap(9));

		ParseDumpNumbs(rgBBlk.cap(4), lNodeCur->m_preds_lst);
		ParseDumpNumbs(rgBBlk.cap(5), lNodeCur->m_succs_lst);
		ParseDumpNumbs(rgBBlk.cap(6), lNodeCur->m_exec_lst);
	}

	list<pNode> *lstNodes = plGraph->nodes_list();

	list<pNode>::iterator itNodesTmp;
	list<pNode>::iterator itNodesTmpEnd = lstNodes->end();;

	list<pNode>::iterator itNodes = plGraph->nodes_list()->begin();
	list<pNode>::iterator itNodesEnd = plGraph->nodes_list()->end();
	
	list<unsigned int>::iterator itAccessor;
	list<unsigned int>::iterator itAccessorEnd;

//	The Second iteration: links Nodes with Edges to Graph
	while (itNodes != itNodesEnd) {
//		linking preds
		itAccessor = (pLNode(*itNodes))->m_preds_lst.begin();
		itAccessorEnd = (pLNode(*itNodes))->m_preds_lst.end();

		for (;itAccessor != itAccessorEnd; itAccessor++) {
			for (itNodesTmp = lstNodes->begin(); itNodesTmp != itNodesTmpEnd; itNodesTmp++) {
				if ((pLNode(*itNodesTmp))->id() == *itAccessor) {
					plGraph->AddEdge(*itNodesTmp, *itNodes);
					break;
				}
			}

			if (itNodesTmp == itNodesTmpEnd) {
				printf("Not Found Predssor\n");
				return 2;
			}
		}

//		linking succs
		itAccessor = (pLNode(*itNodes))->m_succs_lst.begin();
		itAccessorEnd = (pLNode(*itNodes))->m_succs_lst.end();

		for (;itAccessor != itAccessorEnd; itAccessor++) {
			for (itNodesTmp = lstNodes->begin(); itNodesTmp != itNodesTmpEnd; itNodesTmp++) {
				if ((pLNode(*itNodesTmp))->id() == *itAccessor) {
					plGraph->AddEdge(*itNodes, *itNodesTmp);
					break;
				}
			}

			if (itNodesTmp == itNodesTmpEnd) {
				printf("Not Found Successor\n");
				return 2;
			}
		}

//		next iteration
		itNodes++;
	}

        plGraph->Dump();
	return 0;
}

