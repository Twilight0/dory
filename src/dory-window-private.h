/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*- */

/*
 *  Dory
 *
 *  Copyright (C) 1999, 2000 Red Hat, Inc.
 *  Copyright (C) 1999, 2000, 2001 Eazel, Inc.
 *
 *  Dory is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  Dory is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Suite 500, MA 02110-1335, USA.
 *
 *  Authors: Elliot Lee <sopwith@redhat.com>
 *           Darin Adler <darin@bentspoon.com>
 *
 */

#ifndef DORY_WINDOW_PRIVATE_H
#define DORY_WINDOW_PRIVATE_H

#include "dory-window.h"
#include "dory-window-slot.h"
#include "dory-window-pane.h"
#include "dory-navigation-state.h"
#include "dory-bookmark-list.h"

#include <libdory-private/dory-directory.h>

/* FIXME bugzilla.gnome.org 42575: Migrate more fields into here. */
struct DoryWindowDetails
{
        GtkWidget *statusbar;
        GtkWidget *menubar;

        GtkWidget *dory_status_bar;

        GtkUIManager *ui_manager;
        GtkActionGroup *main_action_group; /* owned by ui_manager */
        guint help_message_cid;

        /* Menus. */
        guint extensions_menu_merge_id;
        GtkActionGroup *extensions_menu_action_group;

        GtkActionGroup *bookmarks_action_group;
        GtkActionGroup *toolbar_action_group;
        guint bookmarks_merge_id;
        DoryBookmarkList *bookmark_list;

	DoryWindowShowHiddenFilesMode show_hidden_files_mode;

	/* Ensures that we do not react on signals of a
	 * view that is re-used as new view when its loading
	 * is cancelled
	 */
	gboolean temporarily_ignore_view_signals;

        /* available panes, and active pane.
         * Both of them may never be NULL.
         */
        GList *panes;
        DoryWindowPane *active_pane;

        GtkWidget *content_paned;
        DoryNavigationState *nav_state;
        
        /* Side Pane */
        int side_pane_width;
        GtkWidget *sidebar;
        gchar *sidebar_id;

        gboolean show_sidebar;

        /* Toolbar */
        GtkWidget *toolbar;

        /* Toolbar holder */
        GtkWidget *toolbar_holder;

        guint extensions_toolbar_merge_id;
        GtkActionGroup *extensions_toolbar_action_group;

        guint menu_hide_delay_id;

        /* split view */
        GtkWidget *split_view_hpane;

        // A closed pane's location, valid until the remaining pane
        // location changes.
        GFile *secondary_pane_last_location;

        gboolean disable_chrome;

        guint sidebar_width_handler_id;

        guint menu_state_changed_id;

        gboolean menu_skip_release;
        gboolean menu_show_queued;

        gchar *ignore_meta_view_id;
        gint ignore_meta_zoom_level;
        GList *ignore_meta_visible_columns;
        GList *ignore_meta_column_order;
        gchar *ignore_meta_sort_column;
        gint ignore_meta_sort_direction;

        gboolean dynamic_menu_entries_current;
};

/* window geometry */
/* Min values are very small, and a Dory window at this tiny size is *almost*
 * completely unusable. However, if all the extra bits (sidebar, location bar, etc)
 * are turned off, you can see an icon or two at this size. See bug 5946.
 */

#define DORY_WINDOW_MIN_WIDTH		200
#define DORY_WINDOW_MIN_HEIGHT		200
#define DORY_WINDOW_DEFAULT_WIDTH		800
#define DORY_WINDOW_DEFAULT_HEIGHT		550

typedef void (*DoryBookmarkFailedCallback) (DoryWindow *window,
                                                DoryBookmark *bookmark);

void               dory_window_sync_view_type                    (DoryWindow    *window);
void               dory_window_load_extension_menus                  (DoryWindow    *window);
DoryWindowPane *dory_window_get_next_pane                        (DoryWindow *window);
void               dory_menus_append_bookmark_to_menu                (DoryWindow    *window, 
                                                                          DoryBookmark  *bookmark, 
                                                                          const char        *parent_path,
                                                                          const char        *parent_id,
                                                                          guint              index_in_parent,
                                                                          GtkActionGroup    *action_group,
                                                                          guint              merge_id,
                                                                          GCallback          refresh_callback,
                                                                          DoryBookmarkFailedCallback failed_callback);

DoryWindowSlot *dory_window_get_slot_for_view                    (DoryWindow *window,
									  DoryView   *view);

void                 dory_window_set_active_slot                     (DoryWindow    *window,
									  DoryWindowSlot *slot);
void                 dory_window_set_active_pane                     (DoryWindow *window,
                                                                          DoryWindowPane *new_pane);
DoryWindowPane * dory_window_get_active_pane                     (DoryWindow *window);

gboolean dory_window_restore_saved_tabs                              (DoryWindow *window);
void dory_window_save_session_state                                  (DoryWindow *window);


/* sync window GUI with current slot. Used when changing slots,
 * and when updating the slot state.
 */
void dory_window_sync_allow_stop       (DoryWindow *window,
					    DoryWindowSlot *slot);
void dory_window_sync_title            (DoryWindow *window,
					    DoryWindowSlot *slot);
void dory_window_sync_zoom_widgets     (DoryWindow *window);
void dory_window_sync_menu_bar         (DoryWindow *window);
void dory_window_sync_bookmark_action  (DoryWindow *window);
void dory_window_sync_thumbnail_action (DoryWindow *window);

/* window menus */
GtkActionGroup *dory_window_create_toolbar_action_group (DoryWindow *window);
void               dory_window_initialize_actions                    (DoryWindow    *window);
void               dory_window_initialize_menus                      (DoryWindow    *window);
void               dory_window_finalize_menus                        (DoryWindow    *window);

void               dory_window_update_show_hide_ui_elements           (DoryWindow     *window);

/* window toolbar */
void               dory_window_close_pane                            (DoryWindow    *window,
                                                                          DoryWindowPane *pane);
void               dory_window_show_location_entry                   (DoryWindow    *window);

#endif /* DORY_WINDOW_PRIVATE_H */
