/**
 * File: node_group.cpp - Implementation of node group class,
 * part of Layout library in MiptVis tool.
 * Copyright (C) 2009  MIPTVIS
 */

#include "layout_iface.h" 
#include <QtCore/QtAlgorithms>

/* Retun median value of given list */
int getMedian(QList<int> list)
{
	int s = list.size();
	int m = s / 2;
	int ret = 0;
	if(s == 0) ret = 0;
	if(s == 1) ret = list[0];
	if(s == 2) ret = (list[0] + list[1]) / 2;
	if((s > 2) && (s % 2 == 1)) ret = list[m];
	if((s > 2) && (s % 2 == 0))
	{
		int l = list[m - 1] - list[0];
		int r = list[s - 1] - list[m];
		if(l + r)ret = (list[m - 1] * r + list[m] * l) / (l + r);
		else ret = 0;
	}
	return ret;
}

NodeGroup::NodeGroup(NodeAux* node)
{
	node_list.append(node);
	width_priv = node->width() + offset;
	pos = node->x();
	prev_priv = NULL;
	next_priv = NULL;
}

void NodeGroup::setNext(NodeGroup* group)
{
	next_priv = group;
};

void NodeGroup::setPrev(NodeGroup* group)
{
	prev_priv = group;
};

NodeGroup* NodeGroup::prev()
{
	return prev_priv;
}

NodeGroup* NodeGroup::next()
{
	return next_priv;
}

int NodeGroup::left()
{
	return node_list.first()->x() - node_list.first()->width() / 2 - offset / 2;
};

int NodeGroup::right()
{
	return node_list.last()->x() + node_list.last()->width() / 2 + offset / 2;
};

int NodeGroup::width()
{
	return width_priv;
}

void NodeGroup::median()
{
	QList<int> adj_pos, adj_pos_real, adj_pos_virt; // Positions of adjacent to this group nodes
	/* Get positions of all adjacent nodes */
	for( QList<NodeAux*>::iterator v = node_list.begin(); v != node_list.end(); v++)
	{
		for( EdgeAux* iter = (*v)->firstSucc(); iter != NULL; iter = iter->nextSucc())
		{
			if(iter->backward())
			{
				if((*v)->real() && (iter->succ()->real() || (iter->succ()->rang() == 0)))adj_pos_real.append(iter->succ()->x());
				if(!(*v)->real())adj_pos_virt.append(iter->succ()->x());
			}
		}
		for( EdgeAux* iter = (*v)->firstPred(); iter != NULL; iter = iter->nextPred())
		{
			if(iter->ahead())
			{
				if((*v)->real() && (iter->pred()->real() || (iter->pred()->rang() == 0)))adj_pos_real.append(iter->pred()->x());
				if(!(*v)->real())adj_pos_virt.append(iter->pred()->x());
			}
		}
	}
	/* We consider only real nodes if possible */
	adj_pos.append(adj_pos_real);
	if(adj_pos.empty())adj_pos.append(adj_pos_virt);
	/* Remove repeat elements */
	qSort(adj_pos);
	QList<int>::iterator prev = adj_pos.begin();
	for(QList<int>::iterator iter = ++adj_pos.begin(); iter < adj_pos.end();)
	{
		if(*iter == *prev)iter = adj_pos.erase(iter);
		else prev = iter++;
	}
	/* Calculate median position */
	if(!adj_pos.empty())pos = getMedian(adj_pos);
};

void NodeGroup::append()
{
	node_list += (next()->node_list);
	if(next()->next() != NULL)next()->next()->setPrev(this);
	width_priv += next()->width();
	NodeGroup* next_tmp = next();
	setNext(next()->next());
	next_tmp->setNext(NULL);
	next_tmp->setPrev(NULL);
	next_tmp->node_list.clear();
	median();
	update();
};

void NodeGroup::prepend()
{
	prev()->append();
};

void NodeGroup::updatePos()
{
	/* Calculate private position inside this group  */
	int x = pos - width_priv / 2 + offset / 2;
	for( QList<NodeAux*>::iterator iter = node_list.begin(); iter != node_list.end(); iter++)
	{
		(*iter)->msetX(x + (*iter)->width() / 2);
		x += offset + (*iter)->width();
	}
	/* Calculate median position between nodes of this group */
	QList<int> pos_real, pos_virt, pos_list;
	for(QList<NodeAux*>::iterator iter = node_list.begin(); iter != node_list.end(); iter++)
	{
		if((*iter)->real())pos_real.append((*iter)->x());
		if(!(*iter)->real())pos_virt.append((*iter)->x());
	}
	/* Consider only real nodes if possible */
	pos_list.append(pos_real);
	if(pos_list.empty())pos_list.append(pos_virt);
	qSort(pos_list);
	int shift = pos - getMedian(pos_list);
	/* Update positions */
	for(QList<NodeAux*>::iterator iter = node_list.begin(); iter != node_list.end(); iter++)
	{
		(*iter)->msetX((*iter)->x() + shift);
	}
};

void NodeGroup::update()
{
	if(!empty())
	{
		updatePos();
		if(prev() != NULL) if(left() <= prev()->right()) prepend();
		if(next() != NULL) if(right() >= next()->left()) append();
	}
};

bool NodeGroup::empty()
{
	return node_list.isEmpty();
};