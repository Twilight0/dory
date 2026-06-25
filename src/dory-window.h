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
/* dory-window.h: Interface of the main window object */

#ifndef DORY_WINDOW_H
#define DORY_WINDOW_H

#include <gtk/gtk.h>
#include <eel/eel-glib-extensions.h>
#include <libdory-private/dory-bookmark.h>
#include <libdory-private/dory-search-directory.h>

#include "dory-navigation-state.h"
#include "dory-view.h"
#include "dory-window-types.h"

#define DORY_TYPE_WINDOW dory_window_get_type()
#define DORY_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_WINDOW, DoryWindow))
#define DORY_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_WINDOW, DoryWindowClass))
#define DORY_IS_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_WINDOW))
#define DORY_IS_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_WINDOW))
#define DORY_WINDOW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_WINDOW, DoryWindowClass))

typedef enum {
        DORY_WINDOW_SHOW_HIDDEN_FILES_ENABLE,
        DORY_WINDOW_SHOW_HIDDEN_FILES_DISABLE
} DoryWindowShowHiddenFilesMode;

typedef enum {
        DORY_WINDOW_NOT_SHOWN,
        DORY_WINDOW_POSITION_SET,
        DORY_WINDOW_SHOULD_SHOW
} DoryWindowShowState;

typedef enum {
	DORY_WINDOW_OPEN_SLOT_NONE = 0,
	DORY_WINDOW_OPEN_SLOT_APPEND = 1
}  DoryWindowOpenSlotFlags;

enum {
    SORT_NULL = -1,
    SORT_ASCENDING = 0,
    SORT_DESCENDING = 1
};

#define DORY_WINDOW_SIDEBAR_PLACES "places"
#define DORY_WINDOW_SIDEBAR_TREE "tree"

typedef struct DoryWindowDetails DoryWindowDetails;

typedef struct {
        GtkApplicationWindowClass parent_spot;

	/* Function pointers for overriding, without corresponding signals */

        void   (* sync_title) (DoryWindow *window,
			       DoryWindowSlot *slot);
        DoryIconInfo * (* get_icon) (DoryWindow *window,
                                         DoryWindowSlot *slot);

        void   (* prompt_for_location) (DoryWindow *window, const char *initial);
        void   (* close) (DoryWindow *window);

        /* Signals used only for keybindings */
        void   (* go_up)  (DoryWindow *window);
	void   (* reload) (DoryWindow *window);
} DoryWindowClass;

struct DoryWindow {
        GtkApplicationWindow parent_object;
        
        DoryWindowDetails *details;
};

GType            dory_window_get_type             (void);
DoryWindow *     dory_window_new                  (GtkApplication    *application,
                                                   GdkScreen         *screen);
void             dory_window_close                (DoryWindow    *window);

void             dory_window_connect_content_view (DoryWindow    *window,
						       DoryView      *view);
void             dory_window_disconnect_content_view (DoryWindow    *window,
							  DoryView      *view);

void             dory_window_go_to                (DoryWindow    *window,
                                                       GFile             *location);
void             dory_window_go_to_tab            (DoryWindow    *window,
                                                       GFile             *location);
void             dory_window_go_to_full           (DoryWindow    *window,
                                                       GFile             *location,
                                                       DoryWindowGoToCallback callback,
                                                       gpointer           user_data);
void             dory_window_new_tab              (DoryWindow    *window);

GtkUIManager *   dory_window_get_ui_manager       (DoryWindow    *window);
GtkActionGroup * dory_window_get_main_action_group (DoryWindow   *window);
DoryNavigationState * 
                 dory_window_get_navigation_state (DoryWindow    *window);

void                 dory_window_report_load_complete     (DoryWindow *window,
                                                               DoryView *view);

DoryWindowSlot * dory_window_get_extra_slot       (DoryWindow *window);
DoryWindowShowHiddenFilesMode
                     dory_window_get_hidden_files_mode (DoryWindow *window);
void                 dory_window_set_hidden_files_mode (DoryWindow *window,
                                                            DoryWindowShowHiddenFilesMode  mode);
void                 dory_window_report_load_underway  (DoryWindow *window,
                                                            DoryView *view);
void                 dory_window_view_visible          (DoryWindow *window,
                                                            DoryView *view);
GList *              dory_window_get_panes             (DoryWindow *window);
DoryWindowSlot * dory_window_get_active_slot       (DoryWindow *window);
void                 dory_window_push_status           (DoryWindow *window,
                                                            const char *text);
GtkWidget *          dory_window_ensure_location_bar   (DoryWindow *window);
void                 dory_window_sync_location_widgets (DoryWindow *window);
void                 dory_window_sync_search_widgets   (DoryWindow *window);
void                 dory_window_grab_focus            (DoryWindow *window);
void                 dory_window_sync_create_folder_button (DoryWindow *window);
void     dory_window_hide_sidebar         (DoryWindow *window);
void     dory_window_show_sidebar         (DoryWindow *window);
void     dory_window_back_or_forward      (DoryWindow *window,
                                               gboolean        back,
                                               guint           distance,
                                               DoryWindowOpenFlags flags);
void     dory_window_split_view_on        (DoryWindow *window);
void     dory_window_split_view_off       (DoryWindow *window);
gboolean dory_window_split_view_showing   (DoryWindow *window);

gboolean dory_window_disable_chrome_mapping (GValue *value,
                                                 GVariant *variant,
                                                 gpointer user_data);

void     dory_window_set_sidebar_id (DoryWindow *window,
                                    const gchar *id);

const gchar *    dory_window_get_sidebar_id (DoryWindow *window);

void    dory_window_set_show_sidebar (DoryWindow *window,
                                      gboolean show);

gboolean  dory_window_get_show_sidebar (DoryWindow *window);

const gchar *dory_window_get_ignore_meta_view_id (DoryWindow *window);
void         dory_window_set_ignore_meta_view_id (DoryWindow *window, const gchar *id);
gint         dory_window_get_ignore_meta_zoom_level (DoryWindow *window);
void         dory_window_set_ignore_meta_zoom_level (DoryWindow *window, gint level);
GList       *dory_window_get_ignore_meta_visible_columns (DoryWindow *window);
void         dory_window_set_ignore_meta_visible_columns (DoryWindow *window, GList *list);
GList       *dory_window_get_ignore_meta_column_order (DoryWindow *window);
void         dory_window_set_ignore_meta_column_order (DoryWindow *window, GList *list);
const gchar *dory_window_get_ignore_meta_sort_column (DoryWindow *window);
void         dory_window_set_ignore_meta_sort_column (DoryWindow *window, const gchar *column);
gint         dory_window_get_ignore_meta_sort_direction (DoryWindow *window);
void         dory_window_set_ignore_meta_sort_direction (DoryWindow *window, gint direction);

void         dory_window_clear_secondary_pane_location (DoryWindow *window);
DoryWindowOpenFlags dory_event_get_window_open_flags   (void);

void dory_window_slot_added (DoryWindow *window,  DoryWindowSlot *slot);
void dory_window_slot_removed (DoryWindow *window,  DoryWindowSlot *slot);

#endif
