/**
 * File: node_group.h - Node group class definition, part of Layout 
 * library in MiptVis tool
 * Copyright (C) 2009  MIPTVIS
 */

#ifndef NODE_GROUP_H
#define NODE_GROUP_H

#include <QtCore/QList>

class NodeGroup
{
private:
	/* Space between node */
 	static const int offset = 20;

	/* Group position(on X axis) & width */
	int pos;
	int width_priv;

	/* Next and previous node group in rank */
	NodeGroup* next_priv, *prev_priv;

	/* Updates coordinates of nodes in group */
	void updatePos();

public:
	/* Construct node group from one node */
	NodeGroup(NodeAux* node);

	/* List of nodes in group */
	QList<NodeAux*> node_list;

	/* Set and get previous and next group in rank */
	void setNext(NodeGroup* group);
	void setPrev(NodeGroup* group);
	NodeGroup* next();
	NodeGroup* prev();

	/* Left and right borders of group */
	int left();
	int right();

	/* Return width of group */
	int width();

	/* Attach another group to the left or right side of this group */
	void append();
	void prepend();

	/* Calculate median position of group */
	void median();

	/* Update position on group */
	void update();

	/* Check if group is empty */
	bool empty();
};

#endif