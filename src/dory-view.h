/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/* dory-view.h
 *
 * Copyright (C) 1999, 2000  Free Software Foundaton
 * Copyright (C) 2000, 2001  Eazel, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
 * Boston, MA 02110-1335, USA.
 *
 * Authors: Ettore Perazzoli
 * 	    Darin Adler <darin@bentspoon.com>
 * 	    John Sullivan <sullivan@eazel.com>
 *          Pavel Cisler <pavel@eazel.com>
 */

#ifndef DORY_VIEW_H
#define DORY_VIEW_H

#include <gtk/gtk.h>
#include <gio/gio.h>

#define DORY_FILTER_NO_MATCH G_MAXINT

#include <libdory-private/dory-directory.h>
#include <libdory-private/dory-file.h>
#include <libdory-private/dory-icon-container.h>
#include <libdory-private/dory-link.h>

typedef struct DoryView DoryView;
typedef struct DoryViewClass DoryViewClass;

#include "dory-window.h"
#include "dory-window-slot.h"

#define DORY_TYPE_VIEW dory_view_get_type()
#define DORY_VIEW(obj)\
	(G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_VIEW, DoryView))
#define DORY_VIEW_CLASS(klass)\
	(G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_VIEW, DoryViewClass))
#define DORY_IS_VIEW(obj)\
	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_VIEW))
#define DORY_IS_VIEW_CLASS(klass)\
	(G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_VIEW))
#define DORY_VIEW_GET_CLASS(obj)\
	(G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_VIEW, DoryViewClass))

typedef struct DoryViewDetails DoryViewDetails;

struct DoryView {
	GtkScrolledWindow parent;

	DoryViewDetails *details;
};

struct DoryViewClass {
	GtkScrolledWindowClass parent_class;

	/* The 'clear' signal is emitted to empty the view of its contents.
	 * It must be replaced by each subclass.
	 */
	void 	(* clear) 		 (DoryView *view);
	
	/* The 'begin_file_changes' signal is emitted before a set of files
	 * are added to the view. It can be replaced by a subclass to do any 
	 * necessary preparation for a set of new files. The default
	 * implementation does nothing.
	 */
	void 	(* begin_file_changes) (DoryView *view);
	
	/* The 'add_file' signal is emitted to add one file to the view.
	 * It must be replaced by each subclass.
	 */
	void    (* add_file) 		 (DoryView *view, 
					  DoryFile *file,
					  DoryDirectory *directory);
	void    (* remove_file)		 (DoryView *view, 
					  DoryFile *file,
					  DoryDirectory *directory);

	/* The 'file_changed' signal is emitted to signal a change in a file,
	 * including the file being removed.
	 * It must be replaced by each subclass.
	 */
	void 	(* file_changed)         (DoryView *view, 
					  DoryFile *file,
					  DoryDirectory *directory);

	/* The 'end_file_changes' signal is emitted after a set of files
	 * are added to the view. It can be replaced by a subclass to do any 
	 * necessary cleanup (typically, cleanup for code in begin_file_changes).
	 * The default implementation does nothing.
	 */
	void 	(* end_file_changes)    (DoryView *view);
	
	/* The 'begin_loading' signal is emitted before any of the contents
	 * of a directory are added to the view. It can be replaced by a 
	 * subclass to do any necessary preparation to start dealing with a
	 * new directory. The default implementation does nothing.
	 */
	void 	(* begin_loading) 	 (DoryView *view);

	/* The 'end_loading' signal is emitted after all of the contents
	 * of a directory are added to the view. It can be replaced by a 
	 * subclass to do any necessary clean-up. The default implementation 
	 * does nothing.
	 *
	 * If all_files_seen is true, the handler may assume that
	 * no load error ocurred, and all files of the underlying
	 * directory were loaded.
	 *
	 * Otherwise, end_loading was emitted due to cancellation,
	 * which usually means that not all files are available.
	 */
	void 	(* end_loading) 	 (DoryView *view,
					  gboolean all_files_seen);

	/* The 'load_error' signal is emitted when the directory model
	 * reports an error in the process of monitoring the directory's
	 * contents.  The load error indicates that the process of 
	 * loading the contents has ended, but the directory is still
	 * being monitored. The default implementation handles common
	 * load failures like ACCESS_DENIED.
	 */
	void    (* load_error)           (DoryView *view,
					  GError *error);

	/* Function pointers that don't have corresponding signals */

        /* reset_to_defaults is a function pointer that subclasses must 
         * override to set sort order, zoom level, etc to match default
         * values. 
         */
        void     (* reset_to_defaults)	         (DoryView *view);

	/* get_backing uri is a function pointer for subclasses to
	 * override. Subclasses may replace it with a function that
	 * returns the URI for the location where to create new folders,
	 * files, links and paste the clipboard to.
	 */

	char *	(* get_backing_uri)		(DoryView *view);

	/* get_selection is not a signal; it is just a function pointer for
	 * subclasses to replace (override). Subclasses must replace it
	 * with a function that returns a newly-allocated GList of
	 * DoryFile pointers.
	 */
	GList *	(* get_selection) 	 	(DoryView *view);

    /* peek_selection is not a signal; it is just a function pointer for
     * subclasses to replace (override). Subclasses must replace it
     * with a function that returns a pointer to the existing container
     * selection list.
     */
    GList * (* peek_selection)       (DoryView *view);

    /* get_selection_count is not a signal; it is just a function pointer for
     * subclasses to replace (override). Subclasses must replace it
     * with a function that returns the current selection count.
     */
    gint    (* get_selection_count)       (DoryView *view);

	/* get_selection_for_file_transfer  is a function pointer for
	 * subclasses to replace (override). Subclasses must replace it
	 * with a function that returns a newly-allocated GList of
	 * DoryFile pointers. The difference from get_selection is
	 * that any files in the selection that also has a parent folder
	 * in the selection is not included.
	 */
	GList *	(* get_selection_for_file_transfer)(DoryView *view);
	
        /* select_all is a function pointer that subclasses must override to
         * select all of the items in the view */
        void     (* select_all)	         	(DoryView *view);

        /* set_selection is a function pointer that subclasses must
         * override to select the specified items (and unselect all
         * others). The argument is a list of DoryFiles. */

        void     (* set_selection)	 	(DoryView *view, 
        					 GList *selection);
        					 
        /* invert_selection is a function pointer that subclasses must
         * override to invert selection. */

        void     (* invert_selection)	 	(DoryView *view);        					 

	/* Return an array of locations of selected icons in their view. */
	GArray * (* get_selected_icon_locations) (DoryView *view);

	guint    (* get_item_count)             (DoryView *view);

        /* bump_zoom_level is a function pointer that subclasses must override
         * to change the zoom level of an object. */
        void    (* bump_zoom_level)      	(DoryView *view,
					  	 int zoom_increment);

        /* zoom_to_level is a function pointer that subclasses must override
         * to set the zoom level of an object to the specified level. */
        void    (* zoom_to_level) 		(DoryView *view, 
        				         DoryZoomLevel level);

        DoryZoomLevel (* get_zoom_level)    (DoryView *view);

	/* restore_default_zoom_level is a function pointer that subclasses must override
         * to restore the zoom level of an object to a default setting. */
        void    (* restore_default_zoom_level) (DoryView *view);

    /* return the default zoom level for the current view */
        DoryZoomLevel  (* get_default_zoom_level)   (DoryView *view);
        /* can_zoom_in is a function pointer that subclasses must override to
         * return whether the view is at maximum size (furthest-in zoom level) */
        gboolean (* can_zoom_in)	 	(DoryView *view);

        /* can_zoom_out is a function pointer that subclasses must override to
         * return whether the view is at minimum size (furthest-out zoom level) */
        gboolean (* can_zoom_out)	 	(DoryView *view);
        
        /* reveal_selection is a function pointer that subclasses may
         * override to make sure the selected items are sufficiently
         * apparent to the user (e.g., scrolled into view). By default,
         * this does nothing.
         */
        void     (* reveal_selection)	 	(DoryView *view);

        /* merge_menus is a function pointer that subclasses can override to
         * add their own menu items to the window's menu bar.
         * If overridden, subclasses must call parent class's function.
         */
        void    (* merge_menus)         	(DoryView *view);
        void    (* unmerge_menus)         	(DoryView *view);

        /* update_menus is a function pointer that subclasses can override to
         * update the sensitivity or wording of menu items in the menu bar.
         * It is called (at least) whenever the selection changes. If overridden, 
         * subclasses must call parent class's function.
         */
        void    (* update_menus)         	(DoryView *view);

	/* sort_files is a function pointer that subclasses can override
	 * to provide a sorting order to determine which files should be
	 * presented when only a partial list is provided.
	 */
	int     (* compare_files)              (DoryView *view,
						DoryFile    *a,
						DoryFile    *b);

	/* using_manual_layout is a function pointer that subclasses may
	 * override to control whether or not items can be freely positioned
	 * on the user-visible area.
	 * Note that this value is not guaranteed to be constant within the
	 * view's lifecycle. */
	gboolean (* using_manual_layout)     (DoryView *view);

	/* is_read_only is a function pointer that subclasses may
	 * override to control whether or not the user is allowed to
	 * change the contents of the currently viewed directory. The
	 * default implementation checks the permissions of the
	 * directory.
	 */
	gboolean (* is_read_only)	        (DoryView *view);

	/* is_empty is a function pointer that subclasses must
	 * override to report whether the view contains any items.
	 */
	gboolean (* is_empty)                   (DoryView *view);

	gboolean (* can_rename_file)            (DoryView *view,
						 DoryFile *file);
	/* select_all specifies whether the whole filename should be selected
	 * or only its basename (i.e. everything except the extension)
	 * */
	void	 (* start_renaming_file)        (DoryView *view,
					  	 DoryFile *file,
						 gboolean select_all);

	/* convert *point from widget's coordinate system to a coordinate
	 * system used for specifying file operation positions, which is view-specific.
	 *
	 * This is used by the the icon view, which converts the screen position to a zoom
	 * level-independent coordinate system.
	 */
	void (* widget_to_file_operation_position) (DoryView *view,
						    GdkPoint     *position);

	/* Preference change callbacks, overriden by icon and list views. 
	 * Icon and list views respond by synchronizing to the new preference
	 * values and forcing an update if appropriate.
	 */
    void    (* click_policy_changed)       (DoryView *view);
	void	(* click_to_rename_mode_changed)   (DoryView *view);
	void	(* sort_directories_first_changed) (DoryView *view);
	void	(* sort_favorites_first_changed) (DoryView *view);

	/* Get the id string for this view. Its a constant string, not memory managed */
	const char *   (* get_view_id)            (DoryView          *view);

	/* Return the uri of the first visible file */	
	char *         (* get_first_visible_file) (DoryView          *view);
	/* Scroll the view so that the file specified by the uri is at the top
	   of the view */
	void           (* scroll_to_file)	  (DoryView          *view,
						   const char            *uri);

	void    (* update_filter_text)             (DoryView *view,
						   const char  *filter_text);

	void    (* select_first)                   (DoryView *view);

        /* Signals used only for keybindings */
        gboolean (* trash)                         (DoryView *view);
        gboolean (* delete)                        (DoryView *view);

	/* Returns the current sort attribute string, or NULL if unknown/default */
	const gchar * (* get_sort_attribute)       (DoryView *view);
};

/* GObject support */
GType               dory_view_get_type                         (void);

/* Functions callable from the user interface and elsewhere. */
DoryWindow     *dory_view_get_dory_window              (DoryView  *view);
DoryWindowSlot *dory_view_get_dory_window_slot     (DoryView  *view);
char *              dory_view_get_uri                          (DoryView  *view);

void                dory_view_display_selection_info           (DoryView  *view);

GdkAtom	            dory_view_get_copied_files_atom            (DoryView  *view);
gboolean            dory_view_get_active                       (DoryView  *view);

/* Wrappers for signal emitters. These are normally called 
 * only by DoryView itself. They have corresponding signals
 * that observers might want to connect with.
 */
gboolean            dory_view_get_loading                      (DoryView  *view);

/* Hooks for subclasses to call. These are normally called only by 
 * DoryView and its subclasses 
 */
void                dory_view_activate_files                   (DoryView        *view,
								    GList                  *files,
								    DoryWindowOpenFlags flags,
								    gboolean                confirm_multiple);
void                dory_view_activate_file (DoryView *view,
                                             DoryFile *file,
                                             DoryWindowOpenFlags flags);
void                dory_view_preview_files                    (DoryView        *view,
								    GList               *files,
								    GArray              *locations);
void                dory_view_start_batching_selection_changes (DoryView  *view);
void                dory_view_stop_batching_selection_changes  (DoryView  *view);
void                dory_view_notify_selection_changed         (DoryView  *view);
GtkUIManager *      dory_view_get_ui_manager                   (DoryView  *view);
DoryDirectory  *dory_view_get_model                        (DoryView  *view);
DoryFile       *dory_view_get_directory_as_file            (DoryView  *view);
void            dory_view_update_actions_and_extensions        (DoryView *view);

void                dory_view_pop_up_background_context_menu   (DoryView  *view,
								    GdkEventButton   *event);
void                dory_view_pop_up_selection_context_menu    (DoryView  *view,
								    GdkEventButton   *event); 
gboolean            dory_view_should_show_file                 (DoryView  *view,
								    DoryFile     *file);
gboolean	    dory_view_should_sort_directories_first    (DoryView  *view);
gboolean	    dory_view_should_sort_favorites_first    (DoryView  *view);
void                dory_view_ignore_hidden_file_preferences   (DoryView  *view);
void                dory_view_set_show_foreign                 (DoryView  *view,
								    gboolean          show_foreign);
gboolean            dory_view_handle_scroll_event              (DoryView  *view,
								    GdkEventScroll   *event);

void                dory_view_freeze_updates                   (DoryView  *view);
void                dory_view_unfreeze_updates                 (DoryView  *view);
gboolean            dory_view_get_is_renaming                  (DoryView  *view);
void                dory_view_set_is_renaming                  (DoryView  *view,
								    gboolean       renaming);

/* Type-to-filter */
void                dory_view_set_filter_text                  (DoryView  *view,
                                                                    const char    *text);
void                dory_view_clear_filter                     (DoryView  *view);
gboolean            dory_view_get_filter_active                (DoryView  *view);
const char         *dory_view_get_filter_text                  (DoryView  *view);
gint                dory_view_get_filter_match                 (DoryView  *view,
                                                                    DoryFile  *file);
gboolean            dory_view_activate_filter                  (DoryView  *view,
                                                                    GdkEventKey   *event);
void                dory_view_add_subdirectory                (DoryView  *view,
								   DoryDirectory*directory);
void                dory_view_remove_subdirectory             (DoryView  *view,
								   DoryDirectory*directory);

gboolean            dory_view_is_editable                     (DoryView *view);

/* DoryView methods */
const char *      dory_view_get_view_id                (DoryView      *view);

/* file operations */
char *            dory_view_get_backing_uri            (DoryView      *view);
void              dory_view_move_copy_items            (DoryView      *view,
							    const GList       *item_uris,
							    GArray            *relative_item_points,
							    const char        *target_uri,
							    int                copy_action,
							    int                x,
							    int                y);
void              dory_view_new_file_with_initial_contents (DoryView *view,
								const char *parent_uri,
								const char *filename,
								const char *initial_contents,
								int length,
								GdkPoint *pos);

/* selection handling */
int               dory_view_get_selection_count        (DoryView      *view);
GList *           dory_view_get_selection              (DoryView      *view);
GList *           dory_view_peek_selection             (DoryView      *view);
gint              dory_view_get_selection_count        (DoryView      *view);
void              dory_view_set_selection              (DoryView      *view,
							    GList             *selection);


void              dory_view_load_location              (DoryView      *view,
							    GFile             *location);
void              dory_view_stop_loading               (DoryView      *view);

char **           dory_view_get_emblem_names_to_exclude (DoryView     *view);
char *            dory_view_get_first_visible_file     (DoryView      *view);
void              dory_view_scroll_to_file             (DoryView      *view,
							    const char        *uri);
char *            dory_view_get_title                  (DoryView      *view);
gboolean          dory_view_supports_zooming           (DoryView      *view);
void              dory_view_bump_zoom_level            (DoryView      *view,
							    int                zoom_increment);
void              dory_view_zoom_to_level              (DoryView      *view,
							    DoryZoomLevel  level);
void              dory_view_restore_default_zoom_level (DoryView      *view);
gboolean          dory_view_can_zoom_in                (DoryView      *view);
gboolean          dory_view_can_zoom_out               (DoryView      *view);
DoryZoomLevel dory_view_get_zoom_level             (DoryView      *view);
void              dory_view_pop_up_location_context_menu (DoryView    *view,
							      GdkEventButton  *event,
							      const char      *location);
void              dory_view_grab_focus                 (DoryView      *view);
void              dory_view_update_menus               (DoryView      *view);
void              dory_view_new_folder                 (DoryView      *view);

#endif /* DORY_VIEW_H */
