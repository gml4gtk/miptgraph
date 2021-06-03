/** \file
 * \brief routines
 *
 * \par
 * Copyright (C)<br>
 * See README.md in the root directory for details.
 *
 * \par
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * \par
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, see
 * http://www.gnu.org/copyleft/gpl.html
 *
 * SPDX-License-Identifier: GPL-3.0+
 * License-Filename: LICENSE
 */

/**
 * @file: uniqnode.c
 */

#include "config.h"

#include <stdio.h>

#include "splay-tree.h"
#include "main.h"
#include "mem.h"
#include "uniqnode.h"

static splay_tree gidtree = NULL;
static splay_tree lidtree = NULL;

struct usrnode *uniqnode_gid(int nr)
{
	splay_tree_node spn = NULL;
	if (gidtree == NULL) {
		return (NULL);
	}
	spn = splay_tree_lookup(gidtree, (splay_tree_key) nr);
	if (spn == NULL) {
		return (NULL);
	}
	return ((struct usrnode *)spn->value);
}

void uniqnode_gid_add(struct usrnode *n)
{
	struct usrnode *nn = NULL;
	nn = uniqnode_gid(n->gmlid);
	if (nn) {
		return;
	}
	if (gidtree == NULL) {
		gidtree = splay_tree_new(splay_tree_compare_ints, NULL, NULL);
	}
	splay_tree_insert(gidtree, (splay_tree_key) n->gmlid, (splay_tree_value) n);
	return;
}

struct usrnode *uniqnode_lid(int nr)
{
	splay_tree_node spn = NULL;
	if (lidtree == NULL) {
		return (NULL);
	}
	spn = splay_tree_lookup(lidtree, (splay_tree_key) nr);
	if (spn == NULL) {
		return (NULL);
	}
	return ((struct usrnode *)spn->value);
}

void uniqnode_lid_add(struct usrnode *n)
{
	struct usrnode *nn = NULL;
	nn = uniqnode_lid(n->lid);
	if (nn) {
		return;
	}
	if (lidtree == NULL) {
		lidtree = splay_tree_new(splay_tree_compare_ints, NULL, NULL);
	}
	splay_tree_insert(lidtree, (splay_tree_key) n->lid, (splay_tree_value) n);
	return;
}

void uniqnode_clear_gid(void)
{
	if (gidtree) {
		gidtree = splay_tree_delete(gidtree);
	}
	return;
}

void uniqnode_clear_lid(void)
{
	if (lidtree) {
		lidtree = splay_tree_delete(lidtree);
	}
	return;
}

/* end. */
