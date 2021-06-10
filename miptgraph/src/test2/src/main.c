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
 * @file: main.c
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.141592
#endif

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <cairo.h>
#include <cairo-svg.h>

/* this should be the way check for glib version */
#if GLIB_CHECK_VERSION (2,30,0)
#ifdef WIN32
#else
#include <glib-unix.h>
#endif
#endif

/* check for gtk versions */
#if GTK_CHECK_VERSION (4,0,0)
#elif GTK_CHECK_VERSION (3,0,0)
#elif GTK_CHECK_VERSION (3,2,0)
#elif GTK_CHECK_VERSION (3,4,0)
#elif GTK_CHECK_VERSION (3,6,0)
#elif GTK_CHECK_VERSION (3,8,0)
#elif GTK_CHECK_VERSION (3,12,0)
#elif GTK_CHECK_VERSION (3,14,0)
#else
/* assume gtk-2 */
#endif

/* c interface to C++ lib */
#include "cmiptgraph.h"

#include "main.h"
#include "mem.h"
#include "uniqstring.h"
#include "uniqnode.h"

/*
 * a GTK font pattern. These have the syntax
 * fontname [properties] [fontsize]
 *
 * where fontname is the family name,
 * properties is a list of property values separated by spaces,
 * and fontsize is the point size.
 * The properties that you may specify for GTK font patterns are as follows:
 * Slant properties: ‘Italic’ or ‘Oblique’. If omitted, the default (roman) slant is implied.
 * Weight properties: ‘Bold’, ‘Book’, ‘Light’, ‘Medium’, ‘Semi-bold’, or ‘Ultra-light’. If omitted, ‘Medium’ weight is implied.
 * Width properties: ‘Semi-Condensed’ or ‘Condensed’. If omitted, a default width is used.
 * Here are some examples of GTK font patterns:
 * Monospace 12
 * Monospace Bold Italic 12
 */

/* Linux defaults */

/* name of font to use, example "Sans" */
#define DEFAULT_FONTNAME "sans"

/* name of slant to use, example "Italic", "Oblique" or "Roman" */
#define DEFAULT_FONTSLANT "roman"

/* name of weight to use, example "Bold", "Book", "Light", "Medium", "Semi-bold", "Ultra-light" */
#define DEFAULT_FONTWEIGHT "light"

/* name of condensed to use, example "Semi-Condensed", "Condensed" */
#define DEFAULT_FONTCONDENSED "condensed"

/* font size to use, example "10", "18", "20" etc. */
#define DEFAULT_FONTSIZE "12"
#define DEFAULT_FONTSIZE_INT 12

/* edge line thickness double value for example 0.5, 1.0, 1.5 */
#define DEFAULT_EDGE_THICKNESS 1.0

/* pixels in padding for gtk_box_pack try values 0, 1, 5, 20 */
#define PACKPADDING 0

/* window initial (x,y) size in pixels */
#define TOP_LEVEL_WINDOW_XSIZE 700
#define TOP_LEVEL_WINDOW_YSIZE 600

/* max(x,y) of drawing */
static int maxx = 0;
static int maxy = 0;

/* background r/g/b of drawing */
static int bgcr = 0xff;
static int bgcg = 0xff;
static int bgcb = 0xff;

/* debug 0,1,2 */
static int option_gdebug = 0;

/* last open/save dir */
static char *lastopendir = NULL;

/* exposed size of draw area */
static int drawing_area_xsize = 0;
static int drawing_area_ysize = 0;

/* set if there is data to draw */
static int validdata = 0;

/* r/g/b of drawing alt color "grey82" */
static int altr = 0xc1;
static int altg = 0xc1;
static int altb = 0xc1;

/* zoom scaling factor changed by zoom slider */
static double zfactor = 1.0;

/* x offset changed by x slider */
static int vxmin = 0;

/* y offset changed by y slider */
static int vymin = 0;

/* main graph data */
static struct usrgraph *maingraph = NULL;

/* top level window also used in maingtk2.c */
static GtkWidget *mainwindow1 = (GtkWidget *) 0;

/* where to draw */
static GtkWidget *drawingarea1 = (GtkWidget *) 0;

/* sliders */
#if GTK_HAVE_API_VERSION_2 == 1
static GtkObject *adjvscale1 = NULL;
static GtkObject *adjvscale2 = NULL;
static GtkObject *adjhscale1 = NULL;
#endif

#if GTK_HAVE_API_VERSION_3 == 1
#warning "gtk-3 not implemented"
#endif

/* forward decl. */
static void do_clear_all(void);
static void do_layout_all(struct usrgraph *g);
static void dofit(void);
static void top_level_window_main_quit(void);
static void on_top_level_window_open1_activate(GtkMenuItem * menuitem, gpointer user_data);
#if GTK_HAVE_API_VERSION_2 == 1
/* redraw drawing area */
static gboolean on_top_level_window_drawingarea1_expose_event(GtkWidget * widget, GdkEventExpose * event, gpointer user_data);
#endif
static void on_vscale1_changed(GtkAdjustment * adj);
static void on_vscale2_changed(GtkAdjustment * adj);
static void on_hscale1_changed(GtkAdjustment * adj);

int main(int argc, char *argv[])
{
	int status = 0;
	char *s = NULL;
	GtkWidget *vbox1;
	GtkWidget *menubar1;
	GtkWidget *menuitem1;
	GtkWidget *menuitem1_menu;
	GtkWidget *open1;	/* open gml file */
	GtkWidget *quit1;	/* quit */
	GtkWidget *hbox1;
	GtkWidget *vscale1;
	GtkWidget *vscale2;
	GtkWidget *hscale1;

	printf("Running with miptgraph library Version %s\n", cmipt_version());

	/* get the home dir */
	s = getenv("HOME");
	if (s) {
		lastopendir = do_malloc(strlen(s) + 1);
		strcpy(lastopendir, s);
	} else {
		/* there is no home dir set in env */
		lastopendir = NULL;
	}

#if !GLIB_CHECK_VERSION (2, 36, 0)
	/* for GDBus */
	g_type_init();
#endif

	/*
	 *    gtk_init (&argc, &argv); in gkt2, gtk3 and gtk_init() in gtk4
	 *
	 * calls the function gtk_init(gint *argc, gchar ***argv) which will be called in all GTK applications. 
	 * This sets up a few things for us such as the default visual and color map and then proceeds to call 
	 * gdk_init(gint *argc, gchar ***argv). This function initializes the library for use, sets up default 
	 * signal handlers, and checks the arguments passed to your application on the command line, 
	 * looking for one of the following:
	 *
	 *    * --gtk-module
	 *    * --g-fatal-warnings
	 *    * --gtk-debug
	 *    * --gtk-no-debug
	 *    * --gdk-debug
	 *    * --gdk-no-debug
	 *    * --display
	 *    * --sync
	 *    * --no-xshm
	 *    * --name
	 *    * --class
	 *
	 * It removes these from the argument list, leaving anything it does not recognize for your application 
	 * to parse or ignore. This creates a set of standard arguments accepted by all GTK applications.
	 *
	 */

	/* do gtk init, gtk will grab the gtk specific options on command line */
#if GTK_HAVE_API_VERSION_4 == 1
#warning "gtk-4 not implemented"
#else
	/* gtk 2, 3 */
	status = gtk_init_check(&argc, &argv);

	if (status == FALSE) {
		/* this does happen with sudo strace ./mooigraph for some reason */
		printf("%s is in console mode and need in graphical mode to run\n", argv[0]);
		fflush(stdout);
		return (0);
	}
#endif

	/* top level outer window */
	mainwindow1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	/* make sure to exit oke. */
	g_signal_connect(G_OBJECT(mainwindow1), "destroy", G_CALLBACK(top_level_window_main_quit), NULL);

	/* needed for the cairo drawing */
	gtk_widget_set_app_paintable(mainwindow1, TRUE);

	/* use package string program name as set by configure in config.h */
	gtk_window_set_title(GTK_WINDOW(mainwindow1), PACKAGE_STRING);

	/* pre-set some size */
	gtk_window_set_default_size(GTK_WINDOW(mainwindow1), TOP_LEVEL_WINDOW_XSIZE, TOP_LEVEL_WINDOW_YSIZE);

	/* vbox1 is a menu bar */
#if GTK_HAVE_API_VERSION_2 == 1
	vbox1 = gtk_vbox_new( /* homogeneous */ FALSE, /* spacing */ 0);
	gtk_widget_show(vbox1);
	gtk_container_add(GTK_CONTAINER(mainwindow1), vbox1);
#endif

	/* --- */

	/* menu bar in vbox1 */
	menubar1 = gtk_menu_bar_new();
	gtk_widget_show(menubar1);
	gtk_box_pack_start( /* box */ GTK_BOX(vbox1), /* child */ menubar1,
			   /* expand */ FALSE, /* fill */ FALSE,	/* padding */
			   PACKPADDING);

	/* --- */

	/* menu items in menu bar in vbox1 */
	menuitem1 = gtk_menu_item_new_with_mnemonic("File");
	gtk_container_add(GTK_CONTAINER(menubar1), menuitem1);
	gtk_widget_show(menuitem1);

	/* --- */

	/* 'file' sub menu in menu items in menu bar in vbox1 */
	menuitem1_menu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem1), menuitem1_menu);

	/* 'open' in 'file' sub menu in menu items in menu bar in vbox1 */
	open1 = gtk_menu_item_new_with_mnemonic("Open GML Graph");
	gtk_container_add(GTK_CONTAINER(menuitem1_menu), open1);
	gtk_widget_show(open1);

	/* run this routine when selected 'open' in 'file' menu */
	g_signal_connect(G_OBJECT(open1), "activate", G_CALLBACK(on_top_level_window_open1_activate), NULL);

	/* 'quit' in 'file' sub menu in menu items in menu bar in vbox1 */
	quit1 = gtk_menu_item_new_with_mnemonic("Quit");
	gtk_container_add(GTK_CONTAINER(menuitem1_menu), quit1);
	gtk_widget_show(quit1);

	/* run this routine when selected 'quit' in 'file' menu */
	g_signal_connect(G_OBJECT(quit1), "activate", G_CALLBACK(top_level_window_main_quit), NULL);

	/*
	 * in hbox1
	 * left zoom slider
	 * drawing area
	 * right y slider
	 * below x slider
	 */

	/* add next area to the vbox1 */
#if GTK_HAVE_API_VERSION_2 == 1
	hbox1 = gtk_hbox_new( /* homogeneous */ FALSE, /* spacing */ 0);
	gtk_box_pack_start( /* box */ GTK_BOX(vbox1), /* child */ hbox1,
			   /* expand */ TRUE, /* fill */ TRUE,	/* padding */
			   PACKPADDING);
	gtk_widget_show(hbox1);
#endif

	/* vertical slider in hbox1 for the zoom factor 50% is 1:1 */
	adjvscale1 = gtk_adjustment_new(50, 0, 100, 0, 0, 0);

#if GTK_HAVE_API_VERSION_2 == 1
	vscale1 = gtk_vscale_new(GTK_ADJUSTMENT(adjvscale1));
	g_signal_connect(G_OBJECT(adjvscale1), "value_changed", GTK_SIGNAL_FUNC(on_vscale1_changed), NULL);
	gtk_box_pack_start( /* box */ GTK_BOX(hbox1), /* child */ vscale1,
			   /* expand */ FALSE, /* fill */ TRUE,	/* padding */
			   PACKPADDING);
	gtk_scale_set_draw_value(GTK_SCALE(vscale1), FALSE);
	gtk_widget_show(vscale1);
#endif

	drawingarea1 = gtk_drawing_area_new();
	gtk_box_pack_start( /* box */ GTK_BOX(hbox1), /* child */ drawingarea1,
			   /* expand */ TRUE, /* fill */ TRUE,	/* padding */
			   PACKPADDING);
	gtk_widget_show(drawingarea1);

#if GTK_HAVE_API_VERSION_2 == 1
	g_signal_connect(G_OBJECT(drawingarea1), "expose_event", G_CALLBACK(on_top_level_window_drawingarea1_expose_event), NULL);
#endif

	/* vertical slider in hbox1 for the y position range 0...100% of full image size */
	adjvscale2 = gtk_adjustment_new(0, 0, 100, 0, 0, 0);

#if GTK_HAVE_API_VERSION_2 == 1
	vscale2 = gtk_vscale_new(GTK_ADJUSTMENT(adjvscale2));
	g_signal_connect(G_OBJECT(adjvscale2), "value_changed", GTK_SIGNAL_FUNC(on_vscale2_changed), NULL);
	gtk_box_pack_start( /* box */ GTK_BOX(hbox1), /* child */ vscale2,
			   /* expand */ FALSE, /* fill */ TRUE,	/* padding */
			   PACKPADDING);
	gtk_scale_set_draw_value(GTK_SCALE(vscale2), FALSE);
	gtk_widget_show(vscale2);
#endif

	/* horizontal scroller 0..100% of drawing size */
	adjhscale1 = gtk_adjustment_new(0, 0, 100, 0, 0, 0);

#if GTK_HAVE_API_VERSION_2 == 1
	hscale1 = gtk_hscale_new(GTK_ADJUSTMENT(adjhscale1));
	g_signal_connect(G_OBJECT(adjhscale1), "value_changed", GTK_SIGNAL_FUNC(on_hscale1_changed), NULL);
	gtk_box_pack_start( /* box */ GTK_BOX(vbox1), /* child */ hscale1,
			   /* expand */ FALSE, /* fill */ TRUE,	/* padding */
			   PACKPADDING);
	gtk_scale_set_draw_value(GTK_SCALE(hscale1), FALSE);
	gtk_widget_show(hscale1);
#endif

	/* put on screen */
	gtk_widget_show(mainwindow1);

	/* run the gui */
	gtk_main();

	/* */
	do_clear_all();

	lastopendir = do_free(lastopendir);

	return (0);
}

/* exit */
static void top_level_window_main_quit(void)
{

	/* */
	do_clear_all();

	lastopendir = do_free(lastopendir);

	/* run the gtk internal routine to stop gtk_main() which is a for(){} loop */
	gtk_main_quit();

	return;
}

/* open gml file */
static void on_top_level_window_open1_activate(GtkMenuItem * menuitem, gpointer user_data)
{
	GtkWidget *edialog = (GtkWidget *) 0;
	GtkWidget *pdialog = (GtkWidget *) 0;
	GtkWidget *dialog = (GtkWidget *) 0;
	char *file_chooser_filename = (char *)0;
	char *file_chooser_dir = (char *)0;
	GtkFileChooser *chooser = NULL;
	char *inputfilename = (char *)0;
	char *baseinputfilename = (char *)0;
	char *baseinputfilename2 = (char *)0;
	FILE *f = NULL;
	char *bname = NULL;
	int status = 0;

	if (menuitem) {
	}
	if (user_data) {
	}
#if GTK_HAVE_API_VERSION_2 == 1

	/* see gimp source code howto */
	dialog = gtk_file_chooser_dialog_new("Select GML Graph File", 0,	/* parent_window */
					     GTK_FILE_CHOOSER_ACTION_OPEN,
					     GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);

#endif

#if GTK_HAVE_API_VERSION_3 == 1

	/* see gimp source code howto */
	dialog = gtk_file_chooser_dialog_new("Select GML Graph File", GTK_WINDOW(mainwindow1)	/* parent_window */
					     ,
					     GTK_FILE_CHOOSER_ACTION_OPEN,
					     "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);

#endif

	chooser = GTK_FILE_CHOOSER(dialog);

	/* use same dir if opened in earlier dir */
	if (lastopendir) {
		gtk_file_chooser_set_current_folder(chooser, lastopendir);
	}

	gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(mainwindow1));

	/* run the window to select a input file */
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		/* open button */
		file_chooser_filename = (char *)gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		file_chooser_dir = (char *)gtk_file_chooser_get_current_folder(chooser);
	} else {
		/* cancel button */
		(void)gtk_widget_destroy(dialog);
		return;
	}

	/* */
	(void)gtk_widget_destroy(dialog);

	/* update last-used-dir */
	if (file_chooser_dir) {
		lastopendir = do_free(lastopendir);
		lastopendir = do_malloc(strlen(file_chooser_dir) + 1);
		strcpy(lastopendir, file_chooser_dir);
		/* not dp_free() because gtk allocated */
		g_free(file_chooser_dir);
	}

	/* copy the input filename from gtk */
	if (file_chooser_filename) {
		inputfilename = do_malloc(strlen(file_chooser_filename) + 1);
		strcpy(inputfilename, file_chooser_filename);
		/* not do_free() because this is gtk allocated */
		g_free(file_chooser_filename);
	} else {
		return;
	}

	/* set filename in window */
	bname = g_path_get_basename(inputfilename);
	baseinputfilename2 = do_malloc(strlen(bname) + 1);
	strcpy(baseinputfilename2, bname);
	/* not do_free() because gtk allocated */
	g_free(bname);

	/* open file to parse */
	errno = 0;
	f = fopen(inputfilename, "r");

	if (f == NULL) {
		edialog = gtk_message_dialog_new(GTK_WINDOW(mainwindow1),
						 GTK_DIALOG_DESTROY_WITH_PARENT,
						 GTK_MESSAGE_ERROR,
						 GTK_BUTTONS_CLOSE,
						 "Cannot open file %s for reading (%s)", inputfilename, g_strerror(errno));
		gtk_dialog_run(GTK_DIALOG(edialog));
		gtk_widget_destroy(edialog);
		inputfilename = do_free(inputfilename);
		baseinputfilename2 = do_free(baseinputfilename2);
		/* data is invalid at this point */
		validdata = 0;
		return;
	}

	/* clear all old data */
	do_clear_all();

	/* fresh graph */
	maingraph = do_malloc(sizeof(struct usrgraph));

	/* background r/g/b of drawing */
	bgcr = 0xff;
	bgcg = 0xff;
	bgcb = 0xff;

	/* parse the gml data */
	status = gmlparse(maingraph, f, baseinputfilename2, option_gdebug);

	if (status) {
		/* parse error and line number is in status */
		pdialog = gtk_message_dialog_new(GTK_WINDOW(mainwindow1),
						 GTK_DIALOG_DESTROY_WITH_PARENT,
						 GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
						 "Gml graph syntax error in file %s at line %d", baseinputfilename2, status);
		gtk_dialog_run(GTK_DIALOG(pdialog));
		gtk_widget_destroy(pdialog);
		inputfilename = do_free(inputfilename);
		baseinputfilename2 = do_free(baseinputfilename2);
		fclose(f);
		/* data is invalid at this point */
		validdata = 0;
		do_clear_all();
		/* use package string program name as set by configure in config.h */
		gtk_window_set_title(GTK_WINDOW(mainwindow1), PACKAGE_STRING);
		/* re draw screen */
		gtk_widget_queue_draw(drawingarea1);
		return;
	}

	/* parsed oke. */
	fclose(f);

	bname = g_path_get_basename(inputfilename);
	baseinputfilename = uniqstr(bname);

	/* not dp_free because gtk allocated */
	g_free(bname);

	gtk_window_set_title(GTK_WINDOW(mainwindow1), baseinputfilename);

	/* check for empty graph here */
	if (maingraph->rawnodelist) {

		printf("%s(): calculating layout of file %s\n", __func__, baseinputfilename2);
		fflush(stdout);

		do_layout_all(maingraph);

		/* set sliders to defaults */
		zfactor = 1.0;
		gtk_adjustment_set_value(GTK_ADJUSTMENT(adjvscale1), 50);

		vxmin = 0;
		vymin = 0;
		gtk_adjustment_set_value(GTK_ADJUSTMENT(adjvscale2), 0);
		gtk_adjustment_set_value(GTK_ADJUSTMENT(adjhscale1), 0);

		/* fit drawing in window */
		dofit();

		validdata = 1;
	} else {
		/* update status text */
		validdata = 0;
	}

	/* filename is not saved */
	inputfilename = do_free(inputfilename);
	baseinputfilename2 = do_free(baseinputfilename2);

	/* re draw screen */
	gtk_widget_queue_draw(drawingarea1);

	return;
}

/* get text size of node */
static struct usrxy *do_layout_all_ntextsize(struct usrnode *n)
{
	struct usrxy *ret = NULL;
	ret = do_malloc(sizeof(struct usrxy));
	return (ret);
}

/* copy into layouter */
static void do_layout_all_copyto(struct usrgraph *g)
{
	struct usrxy *sz = NULL;
	struct usrnode *fn = NULL;
	struct usrnode *tn = NULL;
	struct usrnode *n = NULL;
	struct usredge *e = NULL;
	void *nn = NULL;
	void *ee = NULL;
	int nnid = 0;
	int nxsize = 0;
	int nysize = 0;
	if (g == NULL) {
		return;
	}
	if (g->lg == NULL) {
		return;
	}
	if (g->rawnodelist) {
		n = g->rawnodelist;
		while (n) {
			nn = cmipt_newnode(g->lg);
			n->ln = nn;
			nnid = cmipt_nodeid(nn);
			n->lid = nnid;
			uniqnode_lid_add(n);
			sz = do_layout_all_ntextsize(n);
			n->txsize = sz->x;
			n->tysize = sz->y;
			sz = do_free(sz);
			n->xsize = cmipt_nodexsize(nn);
			n->ysize = cmipt_nodeysize(nn);
			nxsize = 0;
			nysize = 0;
			if (n->xsize <= 0) {
				nxsize = 1;
			}
			if (n->ysize <= 0) {
				nysize = 1;
			}
			if (n->txsize > n->xsize) {
				nxsize = n->txsize;
			}
			if (n->tysize > n->ysize) {
				nysize = n->tysize;
			}
			if (nxsize || nysize) {
				cmipt_setnodesize(nn, nxsize, nysize);
			}
			n = n->next;
		}
		if (g->rawedgelist) {
			e = g->rawedgelist;
			while (e) {
				fn = uniqnode_gid(e->fromgmlid);
				tn = uniqnode_gid(e->togmlid);
				if (fn && tn) {
					ee = cmipt_newedge(g->lg, fn->ln, tn->ln, (void *)e);
					e->le = ee;
				}
				e = e->next;
			}
		}
	}
	return;
}

/* copy from layouter */
static void do_layout_all_copyfrom(struct usrgraph *g)
{
	int i = 0;
	int count = 0;
	struct usrnode *un = NULL;
	struct drawnode *dn = NULL;
	struct drawedge *en = NULL;
	struct drawnode *fn = NULL;
	struct drawnode *tn = NULL;
	void *ret = NULL;
	int fln = 0;
	int tln = 0;
	int isdummy = 0;
	int selfedges = 0;
	int rxpos = 0;
	int rypos = 0;
	int xpos = 0;
	int ypos = 0;
	int xsize = 0;
	int ysize = 0;
	int hor = 0;
	int rev = 0;
	int split = 0;
	int splithead = 0;
	int splittail = 0;
	void *ud = NULL;
	int ecolor = 0;
	char *elabel = NULL;
	struct usredge *ue = NULL;
	if (g == NULL) {
		return;
	}
	if (g->lg == NULL) {
		return;
	}
	/* get number of nodes in graph including dummy nodes */
	count = cmipt_graphnnodes(g->lg);
	if (count > 0) {
		/* rebuild db */
		uniqdrawnode_clear_lid();
		count = cmipt_graphmaxnodeid(g->lg);
		/* copy nodes from layouter */
		for (i = 0; i < count; i++) {
			ret = cmipt_findnode(g->lg, i);
			if (ret == NULL) {
				continue;
			}
			/* */
			isdummy = cmipt_nodeisdummy(ret);
			if (isdummy == 0) {
				/* this is a real node, get other data */
				un = uniqnode_lid(i);
				xsize = un->xsize;
				ysize = un->ysize;
			} else {
				un = NULL;
				xsize = 1;
				ysize = 1;
			}
			rxpos = cmipt_noderxpos(ret);
			rypos = cmipt_noderypos(ret);
			xpos = cmipt_nodexpos(ret);
			ypos = cmipt_nodeypos(ret);
			selfedges = cmipt_nodenselfedges(ret);
			dn = do_malloc(sizeof(struct drawnode));
			dn->lid = i;	/* layouter id */
			dn->xpos = xpos;
			dn->ypos = ypos;
			dn->rxpos = rxpos;
			dn->rypos = rypos;
			dn->dummy = isdummy;
			dn->un = un;	/* NULL if a dummy node */
			dn->selfedges = selfedges;
			if (g->drawnodelist == NULL) {
				g->drawnodelist = dn;
				g->drawnodelistend = dn;
			} else {
				g->drawnodelistend->next = dn;
				g->drawnodelistend = dn;
			}
			uniqdrawnode_lid_add(dn);
			/* update drawing size */
			if (xpos + xsize > maxx) {
				maxx = xpos + xsize;
			}
			if (ypos + ysize > maxy) {
				maxy = ypos + ysize;
			}
			/* */
		}
		/* copied all nodes, now copy edges */
		/* get number of edges in graph without self-edges */
		count = cmipt_graphnedges(g->lg);
		if (count > 0) {
			/* copy edges and some can have been removed in layouter */
			count = cmipt_graphmaxedgeid(g->lg);
			for (i = 0; i < count; i++) {
				ret = cmipt_findedge(g->lg, i);
				if (ret == NULL) {
					/* some edges can be deleted */
					continue;
				}
				/* other edges and new created edges */
				fln = cmipt_edgefromnode(ret);
				tln = cmipt_edgetonode(ret);
				hor = cmipt_edgeishor(ret);
				rev = cmipt_edgeisrev(ret);
				ud = cmipt_edgeusrdata(ret);
				if (ud) {
					ue = (struct usredge *)ud;
					ecolor = ue->fillcolor;
					elabel = ue->elabel;
				} else {
					/* black edge color */
					ecolor = 0;
					elabel = NULL;
				}
				/* is edge a split edge */
				split = cmipt_edgeissplit(ret);
				splithead = cmipt_edgeisheadsplit(ret);
				splittail = cmipt_edgeistailsplit(ret);
				fn = uniqdrawnode_lid(fln);
				tn = uniqdrawnode_lid(tln);
				if (fn && tn) {
					en = do_malloc(sizeof(struct drawedge));
					en->lid = i;
					en->hor = hor;
					en->rev = rev;
					en->split = split;
					en->splithead = splithead;
					en->splittail = splittail;
					en->fn = fn;
					en->tn = tn;
					en->ecolor = ecolor;
					en->elabel = elabel;
					if (g->drawedgelist == NULL) {
						g->drawedgelist = en;
						g->drawedgelistend = en;
					} else {
						g->drawedgelistend->next = en;
						g->drawedgelistend = en;
					}
				}
				/* next edge */
			}
			/* if() */
		}
	}
	return;
}

/* layout it */
static void do_layout_all(struct usrgraph *g)
{
	struct usrnode *n = NULL;
	struct usredge *e = NULL;
	void *gptr = NULL;
	if (g == NULL) {
		return;
	}
	gptr = cmipt_newgraph();
	if (gptr == NULL) {
		return;
	}
	g->lg = gptr;
	maxx = 0;
	maxy = 0;
	if (option_gdebug > 1 || 0) {
		printf("going to layout this graph data:\n");
		if (g->rawnodelist) {
			n = g->rawnodelist;
			while (n) {
				printf("node %s %d %s\n", n->gmlidstr, n->gmlid, n->nlabel);
				n = n->next;
			}
		}
		if (g->rawedgelist) {
			e = g->rawedgelist;
			while (e) {
				printf("edge from %d to %d\n", e->fromgmlid, e->togmlid);
				e = e->next;
			}
		}
	}
	do_layout_all_copyto(g);
	/* run layout */
	cmipt_layout(g->lg, 10, 1, 10, /* debug 1 */ 0, /* use barycenter values */ 0);
	do_layout_all_copyfrom(g);

	return;
}

static void do_clear_all_rawedgelist(void)
{
	struct usredge *e = NULL;
	struct usredge *enext = NULL;
	if (maingraph) {
		if (maingraph->rawedgelist) {
			e = maingraph->rawedgelist;
			while (e) {
				enext = e->next;
				e = do_free(e);
				e = enext;
			}
		}
	}
	return;
}

static void do_clear_all_rawnodelist(void)
{
	struct usrnode *n = NULL;
	struct usrnode *nnext = NULL;
	if (maingraph) {
		if (maingraph->rawnodelist) {
			n = maingraph->rawnodelist;
			while (n) {
				nnext = n->next;
				n = do_free(n);
				n = nnext;
			}
		}
	}
	return;
}

static void do_clear_all_drawedgelist(void)
{
	struct drawedge *e = NULL;
	struct drawedge *enext = NULL;
	if (maingraph) {
		if (maingraph->drawedgelist) {
			e = maingraph->drawedgelist;
			while (e) {
				enext = e->next;
				e = do_free(e);
				e = enext;
			}
		}
	}
	return;
}

static void do_clear_all_drawnodelist(void)
{
	struct drawnode *n = NULL;
	struct drawnode *nnext = NULL;
	if (maingraph) {
		if (maingraph->drawnodelist) {
			n = maingraph->drawnodelist;
			while (n) {
				nnext = n->next;
				n = do_free(n);
				n = nnext;
			}
		}
	}
	return;
}

/* clear all used mem at exit or file->open */
static void do_clear_all(void)
{

	/* clear node and string database */
	uniqnode_clear_gid();
	uniqnode_clear_lid();
	uniqdrawnode_clear_lid();
	uniqstr_clear();

	/* clear maingraph data */

	// ...

	if (maingraph) {

		maingraph->lg = cmipt_deletegraph(maingraph->lg);

		/* clear input data */
		do_clear_all_rawnodelist();
		maingraph->rawnodelist = NULL;
		maingraph->rawnodelistend = NULL;

		do_clear_all_rawedgelist();
		maingraph->rawedgelist = NULL;
		maingraph->rawedgelistend = NULL;

		/* clear draw data */
		do_clear_all_drawnodelist();
		maingraph->drawnodelist = NULL;
		maingraph->drawnodelistend = NULL;

		do_clear_all_drawedgelist();
		maingraph->drawedgelist = NULL;
		maingraph->drawedgelistend = NULL;

		/* main graph data */
		maingraph = do_free(maingraph);
	}

	return;
}

/* fit drawing */
static void dofit(void)
{
	return;
}

/* left slider zoom factor */
static void on_vscale1_changed(GtkAdjustment * adj)
{
	gdouble val0 = 0.0;
	int val1 = 0;

	if (adj) {
	}

	/* check if there is node data to draw */
	if (validdata == 0) {
		return;
	}

	val0 = gtk_adjustment_get_value(adj);
	val1 = (int)val0;
	zfactor = exp((double)(3 * (val1 - 50)) / (double)50);

	if (option_gdebug > 1 || 0) {
		printf("%s(): zoomslider=%d zoomfactor=%f\n", __func__, val1, zfactor);
		fflush(stdout);
	}

	/* do a re-draw */
	gtk_widget_queue_draw(drawingarea1);

	return;
}

/* right slider y offset 0...100% */
static void on_vscale2_changed(GtkAdjustment * adj)
{
	gdouble val = 0.0;
	if (adj) {
	}

	/* check if there is node data to draw */
	if (validdata == 0) {
		return;
	}

	val = gtk_adjustment_get_value(adj);
	vymin = (int)((val * maxy) / 100);

	if (option_gdebug > 1 || 0) {
		printf("%s(): yslider=%d vymin=%d (maxy=%d)\n", __func__, (int)val, vymin, maxy);
		fflush(stdout);
	}

	/* do a re-draw */
	gtk_widget_queue_draw(drawingarea1);

	return;
}

/* bottom slider x offset 0...100% */
static void on_hscale1_changed(GtkAdjustment * adj)
{
	gdouble val = 0.0;

	/* */
	if (adj) {
	}

	/* check if there is node data to draw */
	if (validdata == 0) {
		return;
	}

	val = gtk_adjustment_get_value(adj);
	vxmin = (int)((val * maxx) / 100);

	if (option_gdebug > 1 || 0) {
		printf("%s(): xslider=%d vxmin=%d (maxx=%d)\n", __func__, (int)val, vxmin, maxx);
		fflush(stdout);
	}

	/* do a re-draw */
	gtk_widget_queue_draw(drawingarea1);

	return;
}

static void on_top_level_window_drawingarea1_expose_event_nodes(cairo_t * crp)
{
	struct drawnode *dn = NULL;
	int xpos = 0;
	int ypos = 0;
	int dummy = 0;
	int se = 0;
	int xsize = 0;
	int ysize = 0;
	int x0 = 0;
	int y0 = 0;
	int ncolor = 0;
	int nbcolor = 0;
	int r = 0;
	int g = 0;
	int b = 0;
	if (maingraph->drawnodelist) {
		dn = maingraph->drawnodelist;
		while (dn) {
			if (dn->un == NULL) {
				/* shouldnothappen */
				dn->dummy = 1;
			}
			/* after node drawing, this amount of self-edges at this node must be in drawing, see dagre */
			se = dn->selfedges;
			if (se) {	/* todo */
			}
			xpos = dn->xpos;
			ypos = dn->ypos;
			dummy = dn->dummy;
			if (dummy) {
				xsize = 1;
				ysize = 1;
				ncolor = 0;
				nbcolor = 0;
			} else {
				xsize = dn->un->xsize;
				ysize = dn->un->ysize;
				ncolor = dn->un->fillcolor;
				nbcolor = dn->un->outlinecolor;
			}

			/* */
			x0 = xpos - vxmin;
			y0 = ypos - vymin;

			/* fillcolor of node white default or color */
			r = ((ncolor & 0x00ff0000) >> 16);
			g = ((ncolor & 0x0000ff00) >> 8);
			b = (ncolor & 0x000000ff);

			cairo_set_source_rgb(crp, r / 255.0, g / 255.0, b / 255.0);
			cairo_rectangle(crp, x0, y0, xsize, ysize);
			cairo_fill(crp);
			cairo_stroke(crp);

			/* bordercolor of node black default or color */
			r = (nbcolor & 0x00ff0000) >> 16;
			g = (nbcolor & 0x0000ff00) >> 8;
			b = (nbcolor & 0x000000ff);

			cairo_set_source_rgb(crp, r / 255.0, g / 255.0, b / 255.0);
			cairo_rectangle(crp, x0, y0, xsize, ysize);
			cairo_stroke(crp);

			dn = dn->next;
		}
	}
	return;
}

static void on_top_level_window_drawingarea1_expose_event_edges(cairo_t * crp)
{
	struct drawedge *de = NULL;
	struct drawnode *fn = NULL;
	struct drawnode *tn = NULL;
	int fdummy = 0;
	int tdummy = 0;
	int fxc = 0;
	int fyc = 0;
	int txc = 0;
	int tyc = 0;
	int xf = 0;
	int yf = 0;
	int xt = 0;
	int yt = 0;
	int ecolor = 0;
	int r = 0;
	int g = 0;
	int b = 0;
	if (maingraph->drawedgelist) {
		de = maingraph->drawedgelist;
		while (de) {
			fn = de->fn;
			tn = de->tn;
			fdummy = fn->dummy;
			tdummy = tn->dummy;
			if (fdummy) {
				fxc = fn->xpos;
				fyc = fn->ypos;
			} else {
				fxc = fn->xpos + fn->un->xsize / 2;;
				fyc = fn->ypos + fn->un->ysize / 2;
			}
			xf = fxc - vxmin;
			yf = fyc - vymin;
			if (tdummy) {
				txc = tn->xpos;
				tyc = tn->ypos;
			} else {
				txc = tn->xpos + tn->un->xsize / 2;;
				tyc = tn->ypos + tn->un->ysize / 2;
			}
			xt = txc - vxmin;
			yt = tyc - vymin;

			/* thickness of edge line */
			cairo_set_line_width(crp, 0.9 /* DEFAULT_EDGE_THICKNESS */ );

			ecolor = de->ecolor;

			/* black or colored line */
			r = (ecolor & 0x00ff0000) >> 16;
			g = (ecolor & 0x0000ff00) >> 8;
			b = (ecolor & 0x000000ff);

			/* draw in line color of edge */
			cairo_set_source_rgb(crp, r / 255.0, g / 255.0, b / 255.0);

			cairo_move_to(crp, xf, yf);

			cairo_line_to(crp, xt, yt);

			cairo_stroke(crp);

			de = de->next;
		}
	}
	return;
}

#if GTK_HAVE_API_VERSION_2 == 1
/* redraw drawing area */
static gboolean on_top_level_window_drawingarea1_expose_event(GtkWidget * widget, GdkEventExpose * event, gpointer user_data)
{
	cairo_t *crdraw = NULL;
	gint w = 0;		/* xsize of drawing area */
	gint h = 0;		/* ysize of drawing area */
	int fit = 0;
	if (widget) {
	}
	if (event == NULL) {
		return (FALSE);
	}
	if (user_data) {
	}

	/* only at first expose */
	if (drawing_area_xsize == 0 && drawing_area_ysize == 0) {
		fit = 1;
	} else {
		fit = 0;
	}

	if (fit) {
	}

	/* get cairo drawing context */
	crdraw = gdk_cairo_create(event->window);

	/* how large drawing area is */
	(void)gdk_drawable_get_size(event->window, &w, &h);
	if (option_gdebug > 1 || 0) {
		printf("%s(): drawing area size is (%d,%d)\n", __func__, w, h);
		fflush(stdout);
	}

	/* save a copy of current size */
	drawing_area_xsize = w;
	drawing_area_ysize = h;

	/* check if there is node data to draw */
	if (validdata == 0) {
		/* white fill drawing area */
		cairo_set_source_rgb(crdraw, altr / 255.0, altg / 255.0, altb / 255.0);
		cairo_rectangle(crdraw, 0, 0, w, h);
		cairo_fill(crdraw);
		cairo_stroke(crdraw);
		cairo_destroy(crdraw);
		/* no data */
		return (FALSE);
	}

	cairo_set_source_rgb(crdraw, bgcr / 255.0, bgcg / 255.0, bgcb / 255.0);
	/* select whole screen to fill with background color */
	cairo_rectangle(crdraw, 0, 0, w, h);
	cairo_fill(crdraw);
	cairo_stroke(crdraw);
	/* use zoom slider drawing scale */
	cairo_scale(crdraw, zfactor, zfactor);
	on_top_level_window_drawingarea1_expose_event_edges(crdraw);
	on_top_level_window_drawingarea1_expose_event_nodes(crdraw);
	/* ready drawing */
	cairo_destroy(crdraw);

	/* do a re-draw */
	gtk_widget_queue_draw(drawingarea1);

	return (FALSE);
}
#endif

/* end. */
