/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   dory-window-slot.h: Dory window slot
 
   Copyright (C) 2008 Free Software Foundation, Inc.
  
   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
  
   You should have received a copy of the GNU General Public
   License along with this program; if not, write to the
   Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
   Boston, MA 02110-1335, USA.
  
   Author: Christian Neumair <cneumair@gnome.org>
*/

#ifndef DORY_WINDOW_SLOT_H
#define DORY_WINDOW_SLOT_H

#include "dory-view.h"
#include "dory-window-types.h"
#include "dory-query-editor.h"

#define DORY_TYPE_WINDOW_SLOT	 (dory_window_slot_get_type())
#define DORY_WINDOW_SLOT_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), DORY_TYPE_WINDOW_SLOT, DoryWindowSlotClass))
#define DORY_WINDOW_SLOT(obj)	 (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_WINDOW_SLOT, DoryWindowSlot))
#define DORY_IS_WINDOW_SLOT(obj)      (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_WINDOW_SLOT))
#define DORY_IS_WINDOW_SLOT_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), DORY_TYPE_WINDOW_SLOT))
#define DORY_WINDOW_SLOT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), DORY_TYPE_WINDOW_SLOT, DoryWindowSlotClass))

typedef enum {
	DORY_LOCATION_CHANGE_STANDARD,
	DORY_LOCATION_CHANGE_BACK,
	DORY_LOCATION_CHANGE_FORWARD,
	DORY_LOCATION_CHANGE_RELOAD
} DoryLocationChangeType;

struct DoryWindowSlotClass {
	GtkBoxClass parent_class;

	/* wrapped DoryWindowInfo signals, for overloading */
	void (* active)		(DoryWindowSlot *slot);
	void (* inactive)	(DoryWindowSlot *slot);
	void (* changed_pane)	(DoryWindowSlot *slot);
};

/* Each DoryWindowSlot corresponds to a location in the window
 * for displaying a DoryView, i.e. a tab.
 */
struct DoryWindowSlot {
	GtkBox parent;

	DoryWindowPane *pane;

	/* slot contains
 	 *  1) an event box containing extra_location_widgets
 	 *  2) the view box for the content view
 	 */
	GtkWidget *extra_location_widgets;

	GtkWidget *view_overlay;
	GtkWidget *floating_bar;
    GtkWidget *cache_bar;
    GtkWidget *no_search_results_box;
    GtkWidget *no_results_label;

    GtkWidget *filter_bar;
    GtkWidget *filter_bar_revealer;
    gulong filter_activate_handler_id;

	guint set_status_timeout_id;
	guint loading_timeout_id;

	DoryView *content_view;
	DoryView *new_content_view;

	/* Information about bookmarks */
	DoryBookmark *current_location_bookmark;
	DoryBookmark *last_location_bookmark;

	/* Current location. */
	GFile *location;
	char *title;
	char *status_text;

	DoryFile *viewed_file;
	gboolean viewed_file_seen;
	gboolean viewed_file_in_trash;

	gboolean allow_stop;

	DoryQueryEditor *query_editor;
	GtkWidget *query_editor_revealer;
	gulong qe_changed_id;
	gulong qe_cancel_id;

	/* New location. */
	DoryLocationChangeType location_change_type;
	guint location_change_distance;
	GFile *pending_location;
	char *pending_scroll_to;
	GList *pending_selection;
	DoryFile *determine_view_file;
	GCancellable *mount_cancellable;
	GError *mount_error;
	gboolean tried_mount;
	DoryWindowGoToCallback open_callback;
	gpointer open_callback_user_data;

	gboolean needs_reload;

	GCancellable *find_mount_cancellable;

	gboolean visible;

	/* Back/Forward chain, and history list. 
	 * The data in these lists are DoryBookmark pointers. 
	 */
	GList *back_list, *forward_list;
};

GType   dory_window_slot_get_type (void);

DoryWindowSlot * dory_window_slot_new (DoryWindowPane *pane);

void    dory_window_slot_update_title		   (DoryWindowSlot *slot);
void    dory_window_slot_update_icon		   (DoryWindowSlot *slot);
void    dory_window_slot_set_query_editor_visible	   (DoryWindowSlot *slot,
							    gboolean            visible);

GFile * dory_window_slot_get_location		   (DoryWindowSlot *slot);
char *  dory_window_slot_get_location_uri		   (DoryWindowSlot *slot);

void dory_window_slot_queue_reload (DoryWindowSlot *slot,
                                    gboolean        clear_thumbs);
void dory_window_slot_force_reload (DoryWindowSlot *slot);

/* convenience wrapper without selection and callback/user_data */
#define dory_window_slot_open_location(slot, location, flags)\
	dory_window_slot_open_location_full(slot, location, flags, NULL, NULL, NULL)

void dory_window_slot_open_location_full (DoryWindowSlot *slot,
					      GFile *location,
					      DoryWindowOpenFlags flags,
					      GList *new_selection, /* DoryFile list */
					      DoryWindowGoToCallback callback,
					      gpointer user_data);

void			dory_window_slot_stop_loading	      (DoryWindowSlot	*slot);

void			dory_window_slot_set_content_view	      (DoryWindowSlot	*slot,
								       const char		*id);
const char	       *dory_window_slot_get_content_view_id      (DoryWindowSlot	*slot);
gboolean		dory_window_slot_content_view_matches_iid (DoryWindowSlot	*slot,
								       const char		*iid);

void    dory_window_slot_go_home			   (DoryWindowSlot *slot,
							    DoryWindowOpenFlags flags);
void    dory_window_slot_go_up                         (DoryWindowSlot *slot,
							    DoryWindowOpenFlags flags);
void    dory_window_slot_set_content_view_widget	   (DoryWindowSlot *slot,
							    DoryView       *content_view);
void    dory_window_slot_set_viewed_file		   (DoryWindowSlot *slot,
							    DoryFile      *file);
void    dory_window_slot_set_allow_stop		   (DoryWindowSlot *slot,
							    gboolean	    allow_stop);
void    dory_window_slot_set_status			   (DoryWindowSlot *slot,
							    const char	 *status,
							    const char   *short_status,
                                gboolean      location_loading);

void    dory_window_slot_add_extra_location_widget     (DoryWindowSlot *slot,
							    GtkWidget       *widget);
void    dory_window_slot_remove_extra_location_widgets (DoryWindowSlot *slot);

DoryView * dory_window_slot_get_current_view     (DoryWindowSlot *slot);
char           * dory_window_slot_get_current_uri      (DoryWindowSlot *slot);
DoryWindow * dory_window_slot_get_window           (DoryWindowSlot *slot);
void           dory_window_slot_make_hosting_pane_active (DoryWindowSlot *slot);

gboolean dory_window_slot_should_close_with_mount (DoryWindowSlot *slot,
						       GMount *mount);

void dory_window_slot_clear_forward_list (DoryWindowSlot *slot);
void dory_window_slot_clear_back_list    (DoryWindowSlot *slot);

void dory_window_slot_check_bad_cache_bar (DoryWindowSlot *slot);

void dory_window_slot_set_show_thumbnails (DoryWindowSlot *slot,
                                           gboolean show_thumbnails);

void dory_window_slot_hide_filter_bar (DoryWindowSlot *slot);

#endif /* DORY_WINDOW_SLOT_H */
