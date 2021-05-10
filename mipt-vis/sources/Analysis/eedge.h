/**
 * File: eedge.h - EEdge class definition, part of
 * Analysis library in MiptVis tool.
 * Copyright (C) 2009  AnalysisTeam
 */
#ifndef EEDGE_H
#define EEDGE_H

enum EdType { TREE, BACK, FORWARD, CROSS, UNTYPED};

class EEdge: public Edge
{
    friend class EGraph;
    EdType edgeType;

public:
	~EEdge();
    /**
     * set edge type using numerations
     */
    void determineEdgeType();
    /**
     * get edge type
     */
    EdType getEdgeType()
    {
        return edgeType;
    }
    /**
     * returns true if the edge is critical and false if it's not.
     */
    bool isEdgeCritical();
	/**
	 * Casts to bring functions of class to the functions of advanced class
	 */
    EEdge* nextEdge()
    {
        return static_cast< EEdge *>( Edge::nextEdge());
    }
};

#endif
