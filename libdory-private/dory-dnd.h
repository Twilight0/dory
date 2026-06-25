/* -*- Mode: C; indent-tabs-mode: f; c-basic-offset: 4; tab-width: 4 -*- */

/* dory-dnd.h - Common Drag & drop handling code shared by the icon container
   and the list view.

   Copyright (C) 2000 Eazel, Inc.

   The Gnome Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gnome Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
   Boston, MA 02110-1335, USA.

   Authors: Pavel Cisler <pavel@eazel.com>,
	    Ettore Perazzoli <ettore@gnu.org>
*/

#ifndef DORY_DND_H
#define DORY_DND_H

#include <gtk/gtk.h>

/* Drag & Drop target names. */
#define DORY_ICON_DND_GNOME_ICON_LIST_TYPE	"x-special/gnome-icon-list"
#define DORY_ICON_DND_URI_LIST_TYPE		"text/uri-list"
#define DORY_ICON_DND_NETSCAPE_URL_TYPE	"_NETSCAPE_URL"
#define DORY_ICON_DND_BGIMAGE_TYPE		"property/bgimage"
#define DORY_ICON_DND_ROOTWINDOW_DROP_TYPE	"application/x-rootwindow-drop"
#define DORY_ICON_DND_XDNDDIRECTSAVE_TYPE	"XdndDirectSave0" /* XDS Protocol Type */
#define DORY_ICON_DND_RAW_TYPE	"application/octet-stream"

/* Item of the drag selection list */
typedef struct {
	char *uri;
	gboolean got_icon_position;
	int icon_x, icon_y;
	int icon_width, icon_height;
} DoryDragSelectionItem;

/* Standard Drag & Drop types. */
typedef enum {
	DORY_ICON_DND_GNOME_ICON_LIST,
	DORY_ICON_DND_URI_LIST,
	DORY_ICON_DND_NETSCAPE_URL,
	DORY_ICON_DND_TEXT,
	DORY_ICON_DND_XDNDDIRECTSAVE,
	DORY_ICON_DND_RAW,
	DORY_ICON_DND_ROOTWINDOW_DROP
} DoryIconDndTargetType;

typedef enum {
	DORY_DND_ACTION_FIRST = GDK_ACTION_ASK << 1,
	DORY_DND_ACTION_SET_AS_BACKGROUND = DORY_DND_ACTION_FIRST << 0,
	DORY_DND_ACTION_SET_AS_FOLDER_BACKGROUND = DORY_DND_ACTION_FIRST << 1,
	DORY_DND_ACTION_SET_AS_GLOBAL_BACKGROUND = DORY_DND_ACTION_FIRST << 2
} DoryDndAction;

/* drag&drop-related information. */
typedef struct {
	GtkTargetList *target_list;

    gchar *source_fs;
    gboolean can_delete_source;

	/* Stuff saved at "receive data" time needed later in the drag. */
	gboolean got_drop_data_type;
	DoryIconDndTargetType data_type;
	GtkSelectionData *selection_data;
	char *direct_save_uri;

	/* Start of the drag, in window coordinates. */
	int start_x, start_y;

	/* List of DoryDragSelectionItems, representing items being dragged, or NULL
	 * if data about them has not been received from the source yet.
	 */
	GList *selection_list;

	/* has the drop occured ? */
	gboolean drop_occured;

	/* whether or not need to clean up the previous dnd data */
	gboolean need_to_destroy;

	/* autoscrolling during dragging */
	int auto_scroll_timeout_id;
	gboolean waiting_to_autoscroll;
	gint64 start_auto_scroll_in;

} DoryDragInfo;

typedef void		(* DoryDragEachSelectedItemDataGet)	(const char *url,
                                 const char *path_str,
								 int x, int y, int w, int h, 
								 gpointer data);
typedef void		(* DoryDragEachSelectedItemIterator)	(DoryDragEachSelectedItemDataGet iteratee, 
								 gpointer iterator_context, 
								 gpointer data);

void			    dory_drag_init				(DoryDragInfo		      *drag_info,
									 const GtkTargetEntry		      *drag_types,
									 int				       drag_type_count,
									 gboolean			       add_text_targets);
void			    dory_drag_finalize			(DoryDragInfo		      *drag_info);
DoryDragSelectionItem  *dory_drag_selection_item_new		(void);
void			    dory_drag_destroy_selection_list	(GList				      *selection_list);
GList			   *dory_drag_build_selection_list		(GtkSelectionData		      *data);

char **			    dory_drag_uri_array_from_selection_list (const GList			      *selection_list);
GList *			    dory_drag_uri_list_from_selection_list	(const GList			      *selection_list);

char **			    dory_drag_uri_array_from_list		(const GList			      *uri_list);
GList *			    dory_drag_uri_list_from_array		(const char			     **uris);

gboolean		    dory_drag_items_local			(const char			      *target_uri,
									 const GList			      *selection_list);
gboolean		    dory_drag_uris_local			(const char			      *target_uri,
									 const GList			      *source_uri_list);
gboolean		    dory_drag_items_in_trash		(const GList			      *selection_list);
gboolean		    dory_drag_items_on_desktop		(const GList			      *selection_list);
void                dory_drag_default_drop_action_for_icons (GdkDragContext *context,
                                                             const char     *target_uri_string,
                                                             const GList    *items,
                                                             int            *action,
                                                             gchar         **source_fs,
                                                             gboolean       *can_delete_source);
GdkDragAction		    dory_drag_default_drop_action_for_netscape_url (GdkDragContext			     *context);
GdkDragAction		    dory_drag_default_drop_action_for_uri_list     (GdkDragContext			     *context,
										const char			     *target_uri_string);
gboolean		    dory_drag_drag_data_get			(GtkWidget			      *widget,
									 GdkDragContext			      *context,
									 GtkSelectionData		      *selection_data,
									 guint				       info,
									 guint32			       time,
									 gpointer			       container_context,
									 DoryDragEachSelectedItemIterator  each_selected_item_iterator);
int			    dory_drag_modifier_based_action		(int				       default_action,
									 int				       non_default_action);

GdkDragAction		    dory_drag_drop_action_ask		(GtkWidget			      *widget,
									 GdkDragAction			       possible_actions);

gboolean		    dory_drag_autoscroll_in_scroll_region	(GtkWidget			      *widget);
void			    dory_drag_autoscroll_calculate_delta	(GtkWidget			      *widget,
									 float				      *x_scroll_delta,
									 float				      *y_scroll_delta);
void			    dory_drag_autoscroll_start		(DoryDragInfo		      *drag_info,
									 GtkWidget			      *widget,
									 GSourceFunc			       callback,
									 gpointer			       user_data);
void			    dory_drag_autoscroll_stop		(DoryDragInfo		      *drag_info);

gboolean		    dory_drag_selection_includes_special_link (GList			      *selection_list);

#endif
