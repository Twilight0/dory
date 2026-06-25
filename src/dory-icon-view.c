/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* fm-icon-view.c - implementation of icon view of directory.

   Copyright (C) 2000, 2001 Eazel, Inc.

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

   Authors: John Sullivan <sullivan@eazel.com>
*/

#include <config.h>

#include "dory-icon-view.h"

#include "dory-actions.h"
#include "dory-icon-view-container.h"
#include "dory-icon-view-grid-container.h"
#include "dory-error-reporting.h"
#include "dory-view-dnd.h"
#include "dory-view-factory.h"
#include "dory-window.h"
#include "dory-desktop-window.h"
#include "dory-desktop-manager.h"
#include "dory-application.h"

#include <stdlib.h>
#include <eel/eel-vfs-extensions.h>
#include <errno.h>
#include <fcntl.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <gio/gio.h>
#include <libdory-private/dory-clipboard-monitor.h>
#include <libdory-private/dory-directory.h>
#include <libdory-private/dory-dnd.h>
#include <libdory-private/dory-file-dnd.h>
#include <libdory-private/dory-file-utilities.h>
#include <libdory-private/dory-ui-utilities.h>
#include <libdory-private/dory-global-preferences.h>
#include <libdory-private/dory-icon-container.h>
#include <libdory-private/dory-icon-dnd.h>
#include <libdory-private/dory-icon.h>
#include <libdory-private/dory-link.h>
#include <libdory-private/dory-metadata.h>
#include <libdory-private/dory-clipboard.h>
#include <libdory-private/dory-desktop-icon-file.h>
#include <libdory-private/dory-desktop-utils.h>
#include <libdory-private/dory-desktop-directory.h>

#define DEBUG_FLAG DORY_DEBUG_ICON_VIEW
#include <libdory-private/dory-debug.h>

#include <locale.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define POPUP_PATH_ICON_APPEARANCE		"/selection/Icon Appearance Items"

enum
{
	PROP_COMPACT = 1,
	PROP_SUPPORTS_AUTO_LAYOUT,
	PROP_IS_DESKTOP,
	PROP_SUPPORTS_KEEP_ALIGNED,
	PROP_SUPPORTS_LABELS_BESIDE_ICONS,
	NUM_PROPERTIES
};

static GParamSpec *properties[NUM_PROPERTIES] = { NULL, };

typedef struct {
	const DoryFileSortType sort_type;
	const char *metadata_text;
	const char *action;
	const char *menu_label;
	const char *menu_hint;
} SortCriterion;

typedef enum {
	MENU_ITEM_TYPE_STANDARD,
	MENU_ITEM_TYPE_CHECK,
	MENU_ITEM_TYPE_RADIO,
	MENU_ITEM_TYPE_TREE
} MenuItemType;

struct DoryIconViewDetails
{
	GList *icons_not_positioned;

	guint react_to_icon_change_idle_id;

	const SortCriterion *sort;
	gboolean sort_reversed;

	GtkActionGroup *icon_action_group;
	guint icon_merge_id;

	gboolean compact;

	gulong clipboard_handler_id;

	GtkWidget *icon_container;

	gboolean supports_auto_layout;
	gboolean is_desktop;
	gboolean supports_keep_aligned;
	gboolean supports_labels_beside_icons;
};


/* Note that the first item in this list is the default sort,
 * and that the items show up in the menu in the order they
 * appear in this list.
 */
static const SortCriterion sort_criteria[] = {
	{
		DORY_FILE_SORT_BY_DISPLAY_NAME,
		"name",
		"Sort by Name",
		N_("by _Name"),
		N_("Keep icons sorted by name in rows")
	},
	{
		DORY_FILE_SORT_BY_SIZE,
		"size",
		"Sort by Size",
		N_("by _Size"),
		N_("Keep icons sorted by size in rows")
	},
	{
		DORY_FILE_SORT_BY_TYPE,
		"type",
		"Sort by Type",
		N_("by _Type"),
		N_("Keep icons sorted by type in rows")
	},
    {
        DORY_FILE_SORT_BY_DETAILED_TYPE,
        "detailed_type",
        "Sort by Detailed Type",
        N_("by _Detailed Type"),
        N_("Keep icons sorted by detailed type in rows")
    },
	{
		DORY_FILE_SORT_BY_MTIME,
		"modification date",
		"Sort by Modification Date",
		N_("by Modification _Date"),
		N_("Keep icons sorted by modification date in rows")
	},
  {
    	DORY_FILE_SORT_BY_TRASHED_TIME,
    	"trashed",
    	"Sort by Trash Time",
    	N_("by T_rash Time"),
    	N_("Keep icons sorted by trash time in rows")
  },
  {
    	DORY_FILE_SORT_BY_EXTENSION,
    	"extension",
    	"Sort by Extension",
    	N_("by _Extension"),
    	N_("Keep icons sorted by extension in rows")
  }
};

static void                 dory_icon_view_set_directory_sort_by        (DoryIconView           *icon_view,
									     DoryFile         *file,
									     const char           *sort_by);
static void                 dory_icon_view_set_zoom_level               (DoryIconView           *view,
									     DoryZoomLevel     new_level,
									     gboolean              always_emit);
static void                 dory_icon_view_update_click_mode            (DoryIconView           *icon_view);
static void                 dory_icon_view_update_click_to_rename_mode  (DoryIconView           *icon_view);
static gboolean             dory_icon_view_is_desktop      (DoryIconView           *icon_view);
static void                 dory_icon_view_reveal_selection       (DoryView               *view);
static const SortCriterion *get_sort_criterion_by_sort_type           (DoryFileSortType  sort_type);
static void                 switch_to_manual_layout                   (DoryIconView     *view);
static void                 update_layout_menus                       (DoryIconView     *view);
static DoryFileSortType get_default_sort_order                    (DoryFile         *file,
								       gboolean             *reversed);
static void                 dory_icon_view_clear_full                 (DoryView *view,
                                                                       gboolean  destroying);
static const SortCriterion *get_sort_criterion_by_metadata_text (const char *metadata_text);
static void		    dory_icon_view_remove_file (DoryView *view, DoryFile *file, DoryDirectory *directory);

G_DEFINE_TYPE (DoryIconView, dory_icon_view, DORY_TYPE_VIEW);

static void
dory_icon_view_destroy (GtkWidget *object)
{
	DoryIconView *icon_view;

	icon_view = DORY_ICON_VIEW (object);

	dory_icon_view_clear_full (DORY_VIEW (object), TRUE);

        if (icon_view->details->react_to_icon_change_idle_id != 0) {
                g_source_remove (icon_view->details->react_to_icon_change_idle_id);
		icon_view->details->react_to_icon_change_idle_id = 0;
        }

	if (icon_view->details->clipboard_handler_id != 0) {
		g_signal_handler_disconnect (dory_clipboard_monitor_get (),
					     icon_view->details->clipboard_handler_id);
		icon_view->details->clipboard_handler_id = 0;
	}

	if (icon_view->details->icons_not_positioned) {
		dory_file_list_free (icon_view->details->icons_not_positioned);
		icon_view->details->icons_not_positioned = NULL;
	}

	GTK_WIDGET_CLASS (dory_icon_view_parent_class)->destroy (object);
}

static void
sync_directory_monitor_number (DoryIconView *view, DoryFile *file)
{
    DoryDirectory *directory;
    DoryDesktopWindow *desktop_window;
    gint monitor;

    if (!view->details->is_desktop) {
        return;
    }

    desktop_window = DORY_DESKTOP_WINDOW (dory_view_get_dory_window (DORY_VIEW (view)));

    monitor = dory_desktop_window_get_monitor (desktop_window);

    directory = dory_view_get_model (DORY_VIEW (view));

    DORY_DESKTOP_DIRECTORY (directory)->display_number = monitor;
}


static DoryIconContainer *
get_icon_container (DoryIconView *icon_view)
{
	return DORY_ICON_CONTAINER (icon_view->details->icon_container);
}

DoryIconContainer *
dory_icon_view_get_icon_container (DoryIconView *icon_view)
{
	return get_icon_container (icon_view);
}

static gboolean
dory_icon_view_supports_manual_layout (DoryIconView *view)
{
	g_return_val_if_fail (DORY_IS_ICON_VIEW (view), FALSE);

	return !dory_icon_view_is_compact (view);
}

static void
real_set_sort_criterion (DoryIconView *icon_view,
                         const SortCriterion *sort,
                         gboolean clear,
			 gboolean set_metadata)
{
	DoryFile *file;

	file = dory_view_get_directory_as_file (DORY_VIEW (icon_view));

    sync_directory_monitor_number (icon_view, file);

	if (clear) {
		dory_file_set_metadata (file,
                                DORY_METADATA_KEY_ICON_VIEW_SORT_BY,
                                NULL,
                                NULL);
		dory_file_set_metadata (file,
                                DORY_METADATA_KEY_ICON_VIEW_SORT_REVERSED,
                                NULL,
                                NULL);
        icon_view->details->sort =
            get_sort_criterion_by_sort_type (get_default_sort_order (file, &icon_view->details->sort_reversed));
    } else if (set_metadata) {
		/* Store the new sort setting. */
        dory_icon_view_set_directory_sort_by (icon_view,
                                              file,
                                              sort->metadata_text);
    }

	/* Update the layout menus to match the new sort setting. */
	update_layout_menus (icon_view);
}

static void
set_sort_criterion (DoryIconView *icon_view,
		    const SortCriterion *sort,
		    gboolean set_metadata)
{
	if (sort == NULL ||
	    icon_view->details->sort == sort) {
		return;
	}

	icon_view->details->sort = sort;

        real_set_sort_criterion (icon_view, sort, FALSE, set_metadata);
}

static void
clear_sort_criterion (DoryIconView *icon_view)
{
	real_set_sort_criterion (icon_view, NULL, TRUE, TRUE);
}

static void
dory_icon_view_clean_up (DoryIconView *icon_view)
{
	DoryIconContainer *icon_container;
	gboolean saved_sort_reversed;

	icon_container = get_icon_container (icon_view);

	/* Hardwire Clean Up to always be by name, in forward order */
	saved_sort_reversed = icon_view->details->sort_reversed;

	dory_icon_view_set_sort_reversed (icon_view, FALSE, FALSE);
	set_sort_criterion (icon_view, &sort_criteria[0], FALSE);

	dory_icon_container_sort (icon_container);
	dory_icon_container_freeze_icon_positions (icon_container);

	dory_icon_view_set_sort_reversed (icon_view, saved_sort_reversed, FALSE);
}

static void
action_clean_up_callback (GtkAction *action, gpointer callback_data)
{
	dory_icon_view_clean_up (DORY_ICON_VIEW (callback_data));
}

static gboolean
dory_icon_view_using_auto_layout (DoryIconView *icon_view)
{
	return dory_icon_container_is_auto_layout
		(get_icon_container (icon_view));
}

static void
action_sort_radio_callback (GtkAction *action,
			    GtkRadioAction *current,
			    DoryIconView *view)
{
	DoryFileSortType sort_type;

	sort_type = gtk_radio_action_get_current_value (current);

	/* Note that id might be a toggle item.
	 * Ignore non-sort ids so that they don't cause sorting.
	 */
	if (sort_type == DORY_FILE_SORT_NONE) {
		switch_to_manual_layout (view);
	} else {
		dory_icon_view_set_sort_criterion_by_sort_type (view, sort_type);
	}
}

static void
list_covers (DoryIconData *data, gpointer callback_data)
{
	GSList **file_list;

	file_list = callback_data;

	*file_list = g_slist_prepend (*file_list, data);
}

static void
unref_cover (DoryIconData *data, gpointer callback_data)
{
	dory_file_unref (DORY_FILE (data));
}

static void
dory_icon_view_clear_full (DoryView *view, gboolean destroying)
{
	DoryIconContainer *icon_container;
	GSList *file_list;

	g_return_if_fail (DORY_IS_ICON_VIEW (view));

	icon_container = get_icon_container (DORY_ICON_VIEW (view));
	if (!icon_container)
		return;

	/* Clear away the existing icons. */
	file_list = NULL;
	dory_icon_container_for_each (icon_container, list_covers, &file_list);
	dory_icon_container_clear (icon_container);

    if (!destroying) {
        dory_icon_container_update_scroll_region (icon_container);
    }

	g_slist_foreach (file_list, (GFunc)unref_cover, NULL);
	g_slist_free (file_list);
}

static void
dory_icon_view_clear (DoryView *view)
{
    dory_icon_view_clear_full (view, FALSE);
}

static gboolean
should_show_file_on_screen (DoryView *view, DoryFile *file)
{
	if (!dory_view_should_show_file (view, file)) {
		return FALSE;
	}

	return TRUE;
}

static void
dory_icon_view_remove_file (DoryView *view, DoryFile *file, DoryDirectory *directory)
{
	DoryIconView *icon_view;

	/* This used to assert that 'directory == dory_view_get_model (view)', but that
	 * resulted in a lot of crash reports (bug #352592). I don't see how that trace happens.
	 * It seems that somehow we get a files_changed event sent to the view from a directory
	 * that isn't the model, but the code disables the monitor and signal callback handlers when
	 * changing directories. Maybe we can get some more information when this happens.
	 * Further discussion in bug #368178.
	 */
	if (directory != dory_view_get_model (view)) {
		char *file_uri, *dir_uri, *model_uri;
		file_uri = dory_file_get_uri (file);
		dir_uri = dory_directory_get_uri (directory);
		model_uri = dory_directory_get_uri (dory_view_get_model (view));
		g_warning ("dory_icon_view_remove_file() - directory not icon view model, shouldn't happen.\n"
			   "file: %p:%s, dir: %p:%s, model: %p:%s, view loading: %d\n"
			   "If you see this, please add this info to http://bugzilla.gnome.org/show_bug.cgi?id=368178",
			   file, file_uri, directory, dir_uri, dory_view_get_model (view), model_uri, dory_view_get_loading (view));
		g_free (file_uri);
		g_free (dir_uri);
		g_free (model_uri);
	}

	icon_view = DORY_ICON_VIEW (view);

	if (dory_icon_container_remove (get_icon_container (icon_view),
					    DORY_ICON_CONTAINER_ICON_DATA (file))) {
		dory_file_unref (file);
	}
}

static void
dory_icon_view_add_file (DoryView *view, DoryFile *file, DoryDirectory *directory)
{
	DoryIconView *icon_view;
	DoryIconContainer *icon_container;

	g_assert (directory == dory_view_get_model (view));

	icon_view = DORY_ICON_VIEW (view);

    if (icon_view->details->is_desktop &&
        !should_show_file_on_screen (view, file)) {
        return;
    }

    icon_container = get_icon_container (icon_view);

    if (dory_file_has_thumbnail_access_problem (file)) {
        dory_application_set_cache_flag (dory_application_get_singleton ());
        dory_window_slot_check_bad_cache_bar (dory_view_get_dory_window_slot (view));
    }

	/* Reset scroll region for the first icon added when loading a directory. */
	if (dory_view_get_loading (view) && dory_icon_container_is_empty (icon_container)) {
		dory_icon_container_reset_scroll_region (icon_container);
	}

	if (dory_icon_container_add (icon_container,
					 DORY_ICON_CONTAINER_ICON_DATA (file))) {
		dory_file_ref (file);
	}
}

static void
dory_icon_view_file_changed (DoryView *view, DoryFile *file, DoryDirectory *directory)
{
	DoryIconView *icon_view;

	g_assert (directory == dory_view_get_model (view));

	g_return_if_fail (view != NULL);
	icon_view = DORY_ICON_VIEW (view);

	if (!icon_view->details->is_desktop) {
		dory_icon_container_request_update
			(get_icon_container (icon_view),
			 DORY_ICON_CONTAINER_ICON_DATA (file));
		return;
	}

	if (!should_show_file_on_screen (view, file)) {
		dory_icon_view_remove_file (view, file, directory);
	} else {

		dory_icon_container_request_update
			(get_icon_container (icon_view),
			 DORY_ICON_CONTAINER_ICON_DATA (file));
	}
}

static gboolean
dory_icon_view_supports_auto_layout (DoryIconView *view)
{
	g_return_val_if_fail (DORY_IS_ICON_VIEW (view), FALSE);

	return view->details->supports_auto_layout;
}

static gboolean
dory_icon_view_is_desktop (DoryIconView *view)
{
	g_return_val_if_fail (DORY_IS_ICON_VIEW (view), FALSE);

	return view->details->is_desktop;
}

static gboolean
dory_icon_view_supports_keep_aligned (DoryIconView *view)
{
	g_return_val_if_fail (DORY_IS_ICON_VIEW (view), FALSE);

	return view->details->supports_keep_aligned;
}

static gboolean
dory_icon_view_supports_labels_beside_icons (DoryIconView *view)
{
	g_return_val_if_fail (DORY_IS_ICON_VIEW (view), FALSE);

	return view->details->supports_labels_beside_icons;
}

static void
update_layout_menus (DoryIconView *view)
{
	gboolean is_auto_layout;
	GtkAction *action;
	const char *action_name;
	DoryFile *file;

	if (view->details->icon_action_group == NULL) {
		return;
	}

	is_auto_layout = dory_icon_view_using_auto_layout (view);
	file = dory_view_get_directory_as_file (DORY_VIEW (view));

	if (dory_icon_view_supports_auto_layout (view)) {
		/* Mark sort criterion. */
		action_name = is_auto_layout ? view->details->sort->action : DORY_ACTION_MANUAL_LAYOUT;
		action = gtk_action_group_get_action (view->details->icon_action_group,
						      action_name);
		gtk_toggle_action_set_active (GTK_TOGGLE_ACTION (action), TRUE);

        action = gtk_action_group_get_action (view->details->icon_action_group,
                                              DORY_ACTION_REVERSED_ORDER);
		gtk_toggle_action_set_active (GTK_TOGGLE_ACTION (action),
					      view->details->sort_reversed);
		gtk_action_set_sensitive (action, is_auto_layout);

		action = gtk_action_group_get_action (view->details->icon_action_group,
		                                      DORY_ACTION_SORT_TRASH_TIME);

		if (file != NULL && dory_file_is_in_trash (file)) {
			gtk_action_set_visible (action, TRUE);
		} else {
			gtk_action_set_visible (action, FALSE);
		}
	}

	action = gtk_action_group_get_action (view->details->icon_action_group,
					      DORY_ACTION_MANUAL_LAYOUT);
	gtk_action_set_visible (action,
				dory_icon_view_supports_manual_layout (view));

	/* Clean Up is only relevant for manual layout */
	action = gtk_action_group_get_action (view->details->icon_action_group,
					      DORY_ACTION_CLEAN_UP);
	gtk_action_set_sensitive (action, !is_auto_layout);

	if (dory_icon_view_is_desktop (view)) {
		gtk_action_set_label (action, _("_Organize Desktop by Name"));
	}

	action = gtk_action_group_get_action (view->details->icon_action_group,
					      DORY_ACTION_KEEP_ALIGNED);
	gtk_action_set_visible (action,
				dory_icon_view_supports_keep_aligned (view));
	gtk_toggle_action_set_active (GTK_TOGGLE_ACTION (action),
				      dory_icon_container_is_keep_aligned (get_icon_container (view)));
	gtk_action_set_sensitive (action, !is_auto_layout);
}


gchar *
dory_icon_view_get_directory_sort_by (DoryIconView *icon_view,
					  DoryFile *file)
{
	const SortCriterion *default_sort_criterion;

	if (!dory_icon_view_supports_auto_layout (icon_view)) {
		return g_strdup ("name");
	}

	default_sort_criterion = get_sort_criterion_by_sort_type (get_default_sort_order (file, NULL));
	g_return_val_if_fail (default_sort_criterion != NULL, NULL);

    sync_directory_monitor_number (icon_view, file);

    return dory_file_get_metadata (file,
                                   DORY_METADATA_KEY_ICON_VIEW_SORT_BY,
                                   default_sort_criterion->metadata_text);
}

static DoryFileSortType
get_default_sort_order (DoryFile *file, gboolean *reversed)
{
	DoryFileSortType retval, default_sort_order;
	gboolean default_sort_in_reverse_order;

	default_sort_order = g_settings_get_enum (dory_preferences,
						  DORY_PREFERENCES_DEFAULT_SORT_ORDER);
	default_sort_in_reverse_order = g_settings_get_boolean (dory_preferences,
								DORY_PREFERENCES_DEFAULT_SORT_IN_REVERSE_ORDER);

	retval = dory_file_get_default_sort_type (file, reversed);

	if (retval == DORY_FILE_SORT_NONE) {

		if (reversed != NULL) {
			*reversed = default_sort_in_reverse_order;
		}

		retval = CLAMP (default_sort_order, DORY_FILE_SORT_BY_DISPLAY_NAME,
				DORY_FILE_SORT_BY_MTIME);
	}

	return retval;
}

static void
dory_icon_view_set_directory_sort_by (DoryIconView *icon_view,
					  DoryFile *file,
					  const char *sort_by)
{
	const SortCriterion *default_sort_criterion;

	if (!dory_icon_view_supports_auto_layout (icon_view)) {
		return;
	}

	default_sort_criterion = get_sort_criterion_by_sort_type (get_default_sort_order (file, NULL));
	g_return_if_fail (default_sort_criterion != NULL);

    sync_directory_monitor_number (icon_view, file);

    dory_file_set_metadata (file,
                            DORY_METADATA_KEY_ICON_VIEW_SORT_BY,
                            default_sort_criterion->metadata_text,
                            sort_by);
}

gboolean
dory_icon_view_get_directory_sort_reversed (DoryIconView *icon_view,
						DoryFile *file)
{
	gboolean reversed;

	if (!dory_icon_view_supports_auto_layout (icon_view)) {
		return FALSE;
	}

	get_default_sort_order (file, &reversed);

    sync_directory_monitor_number (icon_view, file);

    return dory_file_get_boolean_metadata (file,
                                           DORY_METADATA_KEY_ICON_VIEW_SORT_REVERSED,
                                           reversed);
}

static void
dory_icon_view_set_directory_sort_reversed (DoryIconView *icon_view,
						DoryFile *file,
						gboolean sort_reversed)
{
	gboolean reversed;

	if (!dory_icon_view_supports_auto_layout (icon_view)) {
		return;
	}

	get_default_sort_order (file, &reversed);

    sync_directory_monitor_number (icon_view, file);

    dory_file_set_boolean_metadata (file,
                                    DORY_METADATA_KEY_ICON_VIEW_SORT_REVERSED,
                                    reversed,
                                    sort_reversed);
}

static gboolean
get_default_directory_keep_aligned (void)
{
	return TRUE;
}

static gboolean
dory_icon_view_get_directory_keep_aligned (DoryIconView *icon_view,
					       DoryFile *file)
{
	if (!dory_icon_view_supports_keep_aligned (icon_view)) {
		return FALSE;
	}

    sync_directory_monitor_number (icon_view, file);

    return dory_file_get_boolean_metadata (file,
                                           DORY_METADATA_KEY_ICON_VIEW_KEEP_ALIGNED,
                                           get_default_directory_keep_aligned ());
}

void
dory_icon_view_set_directory_keep_aligned (DoryIconView *icon_view,
					       DoryFile *file,
					       gboolean keep_aligned)
{
	if (!dory_icon_view_supports_keep_aligned (icon_view)) {
		return;
	}

    sync_directory_monitor_number (icon_view, file);

    dory_file_set_boolean_metadata (file,
                                    DORY_METADATA_KEY_ICON_VIEW_KEEP_ALIGNED,
                                    get_default_directory_keep_aligned (),
                                    keep_aligned);
}

static gboolean
dory_icon_view_get_directory_auto_layout (DoryIconView *icon_view,
					      DoryFile *file)
{
	if (!dory_icon_view_supports_auto_layout (icon_view)) {
		return FALSE;
	}

	if (!dory_icon_view_supports_manual_layout (icon_view)) {
		return TRUE;
	}

    sync_directory_monitor_number (icon_view, file);

    return dory_file_get_boolean_metadata (file,
                                           DORY_METADATA_KEY_ICON_VIEW_AUTO_LAYOUT,
                                           TRUE);
}

static void
dory_icon_view_set_directory_auto_layout (DoryIconView *icon_view,
					      DoryFile *file,
					gboolean auto_layout)
{
	if (!dory_icon_view_supports_auto_layout (icon_view) ||
	    !dory_icon_view_supports_manual_layout (icon_view)) {
		return;
	}

    sync_directory_monitor_number (icon_view, file);

    dory_file_set_boolean_metadata (file,
                                    DORY_METADATA_KEY_ICON_VIEW_AUTO_LAYOUT,
                                    TRUE,
                                    auto_layout);
}

void
dory_icon_view_set_directory_horizontal_layout (DoryIconView *icon_view,
                                                DoryFile     *file,
                                                gboolean      horizontal)
{
    sync_directory_monitor_number (icon_view, file);

    dory_file_set_boolean_metadata (file,
                                    DORY_METADATA_KEY_DESKTOP_GRID_HORIZONTAL,
                                    FALSE,
                                    horizontal);
}

gboolean
dory_icon_view_get_directory_horizontal_layout (DoryIconView *icon_view,
                                                DoryFile     *file)
{
    sync_directory_monitor_number (icon_view, file);

    return dory_file_get_boolean_metadata (file,
                                           DORY_METADATA_KEY_DESKTOP_GRID_HORIZONTAL,
                                           FALSE);
}

void
dory_icon_view_set_directory_grid_adjusts (DoryIconView *icon_view,
                                           DoryFile     *file,
                                           gint          horizontal,
                                           gint          vertical)
{
    sync_directory_monitor_number (icon_view, file);

    dory_file_set_desktop_grid_adjusts (file,
                                        DORY_METADATA_KEY_DESKTOP_GRID_ADJUST,
                                        horizontal, vertical);
}

void
dory_icon_view_get_directory_grid_adjusts (DoryIconView *icon_view,
                                           DoryFile     *file,
                                           gint         *horizontal,
                                           gint         *vertical)
{
    gint h, v;

    sync_directory_monitor_number (icon_view, file);

    dory_file_get_desktop_grid_adjusts (file,
                                        DORY_METADATA_KEY_DESKTOP_GRID_ADJUST,
                                        &h, &v);

    if (horizontal)
        *horizontal = h;

    if (vertical)
        *vertical = v;
}

gboolean
dory_icon_view_set_sort_reversed (DoryIconView *icon_view,
                                  gboolean      new_value,
                                  gboolean      set_metadata)
{
    if (icon_view->details->sort_reversed == new_value) {
        return FALSE;
    }

    icon_view->details->sort_reversed = new_value;

    if (set_metadata) {
        /* Store the new sort setting. */
        dory_icon_view_set_directory_sort_reversed (icon_view, dory_view_get_directory_as_file (DORY_VIEW (icon_view)), new_value);
    }

    /* Update the layout menus to match the new sort-order setting. */
    update_layout_menus (icon_view);

    return TRUE;
}

void
dory_icon_view_flip_sort_reversed (DoryIconView *icon_view)
{
    dory_icon_view_set_sort_reversed (icon_view, !icon_view->details->sort_reversed, TRUE);
}

static const SortCriterion *
get_sort_criterion_by_metadata_text (const char *metadata_text)
{
	guint i;

	/* Figure out what the new sort setting should be. */
	for (i = 0; i < G_N_ELEMENTS (sort_criteria); i++) {
		if (g_strcmp0 (sort_criteria[i].metadata_text, metadata_text) == 0) {
			return &sort_criteria[i];
		}
	}
	return NULL;
}

static const SortCriterion *
get_sort_criterion_by_sort_type (DoryFileSortType sort_type)
{
	guint i;

	/* Figure out what the new sort setting should be. */
	for (i = 0; i < G_N_ELEMENTS (sort_criteria); i++) {
		if (sort_type == sort_criteria[i].sort_type) {
			return &sort_criteria[i];
		}
	}

	return &sort_criteria[0];
}

#define DEFAULT_ZOOM_LEVEL(icon_view) icon_view->details->compact ? default_compact_zoom_level : default_zoom_level

static DoryZoomLevel
get_default_zoom_level (DoryIconView *icon_view)
{
	DoryZoomLevel default_zoom_level, default_compact_zoom_level;

	default_zoom_level = g_settings_get_enum (dory_icon_view_preferences,
						  DORY_PREFERENCES_ICON_VIEW_DEFAULT_ZOOM_LEVEL);
	default_compact_zoom_level = g_settings_get_enum (dory_compact_view_preferences,
							  DORY_PREFERENCES_COMPACT_VIEW_DEFAULT_ZOOM_LEVEL);

    if (DORY_ICON_VIEW_GET_CLASS (icon_view)->use_grid_container) {
        return DORY_ZOOM_LEVEL_STANDARD;
    }

	return CLAMP (DEFAULT_ZOOM_LEVEL(icon_view), DORY_ZOOM_LEVEL_SMALLEST, DORY_ZOOM_LEVEL_LARGEST);
}

static void
set_labels_beside_icons (DoryIconView *icon_view)
{
	gboolean labels_beside;

	if (dory_icon_view_supports_labels_beside_icons (icon_view)) {
		labels_beside = dory_icon_view_is_compact (icon_view) ||
			g_settings_get_boolean (dory_icon_view_preferences,
						DORY_PREFERENCES_ICON_VIEW_LABELS_BESIDE_ICONS);

		if (labels_beside) {
			dory_icon_container_set_label_position
				(get_icon_container (icon_view),
				 DORY_ICON_LABEL_POSITION_BESIDE);
		} else {
			dory_icon_container_set_label_position
				(get_icon_container (icon_view),
				 DORY_ICON_LABEL_POSITION_UNDER);
		}
	}
}

static void
set_columns_same_width (DoryIconView *icon_view)
{
	gboolean all_columns_same_width;

	if (dory_icon_view_is_compact (icon_view)) {
		all_columns_same_width = g_settings_get_boolean (dory_compact_view_preferences,
								 DORY_PREFERENCES_COMPACT_VIEW_ALL_COLUMNS_SAME_WIDTH);
		dory_icon_container_set_all_columns_same_width (get_icon_container (icon_view), all_columns_same_width);
	}
}

static void
dory_icon_view_begin_loading (DoryView *view)
{
	DoryIconView *icon_view;
	GtkWidget *icon_container;
	DoryFile *file;
	int level;
    int h_adjust, v_adjust;
	char *sort_name, *uri;

	g_return_if_fail (DORY_IS_ICON_VIEW (view));

	icon_view = DORY_ICON_VIEW (view);
	file = dory_view_get_directory_as_file (view);
	uri = dory_file_get_uri (file);
	icon_container = GTK_WIDGET (get_icon_container (icon_view));

    dory_icon_container_set_ok_to_load_deferred_attrs (DORY_ICON_CONTAINER (icon_container), FALSE);

	dory_icon_container_begin_loading (DORY_ICON_CONTAINER (icon_container));

	dory_icon_container_set_allow_moves (DORY_ICON_CONTAINER (icon_container),
						 !eel_uri_is_search (uri));

	g_free (uri);

	/* Set up the zoom level from the metadata. */
	if (dory_view_supports_zooming (DORY_VIEW (icon_view))) {
        if (dory_global_preferences_get_ignore_view_metadata () && !DORY_ICON_VIEW_GET_CLASS (view)->use_grid_container) {
            if (dory_window_get_ignore_meta_zoom_level (dory_view_get_dory_window (view)) == -1) {
                dory_window_set_ignore_meta_zoom_level (dory_view_get_dory_window (view), get_default_zoom_level (icon_view));
            }

            level = dory_window_get_ignore_meta_zoom_level (dory_view_get_dory_window (DORY_VIEW (icon_view)));
        } else {
            sync_directory_monitor_number (icon_view, file);

            if (icon_view->details->compact) {
                level = dory_file_get_integer_metadata (file,
                                                        DORY_METADATA_KEY_COMPACT_VIEW_ZOOM_LEVEL,
                                                        get_default_zoom_level (icon_view));
            } else {
                level = dory_file_get_integer_metadata (file,
                                                        DORY_METADATA_KEY_ICON_VIEW_ZOOM_LEVEL,
                                                        get_default_zoom_level (icon_view));
    		}
        }

		dory_icon_view_set_zoom_level (icon_view, level, TRUE);
	}

	/* Set the sort mode.
	 * It's OK not to resort the icons because the
	 * container doesn't have any icons at this point.
	 */
	sort_name = dory_icon_view_get_directory_sort_by (icon_view, file);
	set_sort_criterion (icon_view, get_sort_criterion_by_metadata_text (sort_name), FALSE);
	g_free (sort_name);

	/* Set the sort direction from the metadata. */
	dory_icon_view_set_sort_reversed (icon_view, dory_icon_view_get_directory_sort_reversed (icon_view, file), FALSE);

    dory_icon_container_set_horizontal_layout (get_icon_container (icon_view),
                                               dory_icon_view_get_directory_horizontal_layout (icon_view, file));

	dory_icon_container_set_keep_aligned (get_icon_container (icon_view),
                    dory_icon_view_get_directory_keep_aligned (icon_view, file));

    dory_icon_view_get_directory_grid_adjusts (DORY_ICON_VIEW (view),
                                               file,
                                               &h_adjust,
                                               &v_adjust);

    dory_icon_container_set_grid_adjusts (get_icon_container (icon_view), h_adjust, v_adjust);

	set_labels_beside_icons (icon_view);
	set_columns_same_width (icon_view);

	/* We must set auto-layout last, because it invokes the layout_changed
	 * callback, which works incorrectly if the other layout criteria are
	 * not already set up properly (see bug 6500, e.g.)
	 */
	dory_icon_container_set_auto_layout
		(get_icon_container (icon_view),
		 dory_icon_view_get_directory_auto_layout (icon_view, file));

	/* e.g. keep aligned may have changed */
	update_layout_menus (icon_view);
}

static void
icon_view_notify_clipboard_info (DoryClipboardMonitor *monitor,
                                 DoryClipboardInfo *info,
                                 DoryIconView *icon_view)
{
	GList *icon_data;

	icon_data = NULL;
	if (info && info->cut) {
		icon_data = info->files;
	}

	dory_icon_container_set_highlighted_for_clipboard (
							       get_icon_container (icon_view), icon_data);
}

static void
dory_icon_view_end_loading (DoryView *view,
			  gboolean all_files_seen)
{
	DoryIconView *icon_view;
	GtkWidget *icon_container;
	DoryClipboardMonitor *monitor;
	DoryClipboardInfo *info;

	icon_view = DORY_ICON_VIEW (view);

	icon_container = GTK_WIDGET (get_icon_container (icon_view));
	dory_icon_container_end_loading (DORY_ICON_CONTAINER (icon_container), all_files_seen);

	monitor = dory_clipboard_monitor_get ();
	info = dory_clipboard_monitor_get_clipboard_info (monitor);
    dory_icon_container_set_ok_to_load_deferred_attrs (DORY_ICON_CONTAINER (icon_container), TRUE);
	icon_view_notify_clipboard_info (monitor, info, icon_view);
}

static DoryZoomLevel
dory_icon_view_get_zoom_level (DoryView *view)
{
	g_return_val_if_fail (DORY_IS_ICON_VIEW (view), DORY_ZOOM_LEVEL_STANDARD);

	return dory_icon_container_get_zoom_level (get_icon_container (DORY_ICON_VIEW (view)));
}

static void
dory_icon_view_set_zoom_level (DoryIconView *view,
				   DoryZoomLevel new_level,
				   gboolean always_emit)
{
	DoryIconContainer *icon_container;

	g_return_if_fail (DORY_IS_ICON_VIEW (view));
	g_return_if_fail (new_level >= DORY_ZOOM_LEVEL_SMALLEST &&
			  new_level <= DORY_ZOOM_LEVEL_LARGEST);

	icon_container = get_icon_container (view);
	if (dory_icon_container_get_zoom_level (icon_container) == new_level) {
		if (always_emit) {
			g_signal_emit_by_name (view, "zoom_level_changed");
		}
		return;
	}

    if (dory_global_preferences_get_ignore_view_metadata () && !DORY_ICON_VIEW_GET_CLASS (view)->use_grid_container) {
        dory_window_set_ignore_meta_zoom_level (dory_view_get_dory_window (DORY_VIEW (view)), new_level);
    } else {
        sync_directory_monitor_number (view, dory_view_get_directory_as_file (DORY_VIEW (view)));

        if (view->details->compact) {
            dory_file_set_integer_metadata (dory_view_get_directory_as_file (DORY_VIEW (view)),
                                            DORY_METADATA_KEY_COMPACT_VIEW_ZOOM_LEVEL,
                                            get_default_zoom_level (view),
                                            new_level);
        } else {
            dory_file_set_integer_metadata (dory_view_get_directory_as_file (DORY_VIEW (view)),
                                            DORY_METADATA_KEY_ICON_VIEW_ZOOM_LEVEL,
                                            get_default_zoom_level (view),
                                            new_level);
        }
    }

	dory_icon_container_set_zoom_level (icon_container, new_level);

	g_signal_emit_by_name (view, "zoom_level_changed");

	if (dory_view_get_active (DORY_VIEW (view))) {
		dory_view_update_menus (DORY_VIEW (view));
	}
}

static void
dory_icon_view_bump_zoom_level (DoryView *view, int zoom_increment)
{
	DoryZoomLevel new_level;

	g_return_if_fail (DORY_IS_ICON_VIEW (view));

	new_level = dory_icon_view_get_zoom_level (view) + zoom_increment;

	if (new_level >= DORY_ZOOM_LEVEL_SMALLEST &&
	    new_level <= DORY_ZOOM_LEVEL_LARGEST) {
		dory_view_zoom_to_level (view, new_level);
	}
}

static void
dory_icon_view_zoom_to_level (DoryView *view,
			    DoryZoomLevel zoom_level)
{
	DoryIconView *icon_view;

	g_assert (DORY_IS_ICON_VIEW (view));

	icon_view = DORY_ICON_VIEW (view);
	dory_icon_view_set_zoom_level (icon_view, zoom_level, FALSE);
}

static void
dory_icon_view_restore_default_zoom_level (DoryView *view)
{
	DoryIconView *icon_view;

	g_return_if_fail (DORY_IS_ICON_VIEW (view));

	icon_view = DORY_ICON_VIEW (view);
	dory_view_zoom_to_level
		(view, get_default_zoom_level (icon_view));
}

static DoryZoomLevel
dory_icon_view_get_default_zoom_level (DoryView *view)
{
    g_return_val_if_fail (DORY_IS_ICON_VIEW (view), DORY_ZOOM_LEVEL_NULL);

    return get_default_zoom_level(DORY_ICON_VIEW (view));
}

static gboolean
dory_icon_view_can_zoom_in (DoryView *view)
{
	g_return_val_if_fail (DORY_IS_ICON_VIEW (view), FALSE);

	return dory_icon_view_get_zoom_level (view)
		< DORY_ZOOM_LEVEL_LARGEST;
}

static gboolean
dory_icon_view_can_zoom_out (DoryView *view)
{
	g_return_val_if_fail (DORY_IS_ICON_VIEW (view), FALSE);

	return dory_icon_view_get_zoom_level (view)
		> DORY_ZOOM_LEVEL_SMALLEST;
}

static gboolean
dory_icon_view_is_empty (DoryView *view)
{
	g_assert (DORY_IS_ICON_VIEW (view));

	return dory_icon_container_is_empty
		(get_icon_container (DORY_ICON_VIEW (view)));
}

static GList *
dory_icon_view_get_selection (DoryView *view)
{
	GList *list;

	g_return_val_if_fail (DORY_IS_ICON_VIEW (view), NULL);

	list = dory_icon_container_get_selection
		(get_icon_container (DORY_ICON_VIEW (view)));
	dory_file_list_ref (list);
	return list;
}

static GList *
dory_icon_view_peek_selection (DoryView *view)
{
    GList *list;

    g_return_val_if_fail (DORY_IS_ICON_VIEW (view), NULL);

    list = dory_icon_container_peek_selection (get_icon_container (DORY_ICON_VIEW (view)));
    dory_file_list_ref (list);
    return list;
}

static gint
dory_icon_view_get_selection_count (DoryView *view)
{
    g_return_val_if_fail (DORY_IS_ICON_VIEW (view), 0);

    return dory_icon_container_get_selection_count (get_icon_container (DORY_ICON_VIEW (view)));
}

static void
count_item (DoryIconData *icon_data,
	    gpointer callback_data)
{
	guint *count;

	count = callback_data;
	(*count)++;
}

static guint
dory_icon_view_get_item_count (DoryView *view)
{
	guint count;

	g_return_val_if_fail (DORY_IS_ICON_VIEW (view), 0);

	count = 0;

	dory_icon_container_for_each
		(get_icon_container (DORY_ICON_VIEW (view)),
		 count_item, &count);

	return count;
}

void
dory_icon_view_set_sort_criterion_by_sort_type (DoryIconView     *icon_view,
                                                DoryFileSortType  sort_type)
{
	const SortCriterion *sort;

	g_assert (DORY_IS_ICON_VIEW (icon_view));

	sort = get_sort_criterion_by_sort_type (sort_type);
	g_return_if_fail (sort != NULL);

	if (sort == icon_view->details->sort
	    && dory_icon_view_using_auto_layout (icon_view)) {
		return;
	}

	set_sort_criterion (icon_view, sort, TRUE);
	dory_icon_container_sort (get_icon_container (icon_view));
	dory_icon_view_reveal_selection (DORY_VIEW (icon_view));
}


static void
action_reversed_order_callback (GtkAction *action,
				gpointer user_data)
{
	DoryIconView *icon_view;

	icon_view = DORY_ICON_VIEW (user_data);

	if (dory_icon_view_set_sort_reversed (icon_view,
			       gtk_toggle_action_get_active (GTK_TOGGLE_ACTION (action)),
			       TRUE)) {
		dory_icon_container_sort (get_icon_container (icon_view));
		dory_icon_view_reveal_selection (DORY_VIEW (icon_view));
	}
}

static void
action_keep_aligned_callback (GtkAction *action,
			      gpointer user_data)
{
	DoryIconView *icon_view;
	DoryFile *file;
	gboolean keep_aligned;

	icon_view = DORY_ICON_VIEW (user_data);

	keep_aligned = gtk_toggle_action_get_active (GTK_TOGGLE_ACTION (action));

	file = dory_view_get_directory_as_file (DORY_VIEW (icon_view));
	dory_icon_view_set_directory_keep_aligned (icon_view,
						 file,
						 keep_aligned);

	dory_icon_container_set_keep_aligned (get_icon_container (icon_view),
						  keep_aligned);
}

static void
switch_to_manual_layout (DoryIconView *icon_view)
{
	if (!dory_icon_view_using_auto_layout (icon_view)) {
		return;
	}

	icon_view->details->sort = &sort_criteria[0];

	dory_icon_container_set_auto_layout
		(get_icon_container (icon_view), FALSE);
}

static void
layout_changed_callback (DoryIconContainer *container,
			 DoryIconView *icon_view)
{
	DoryFile *file;

	g_assert (DORY_IS_ICON_VIEW (icon_view));
	g_assert (container == get_icon_container (icon_view));

	file = dory_view_get_directory_as_file (DORY_VIEW (icon_view));

	if (file != NULL) {
		dory_icon_view_set_directory_auto_layout
			(icon_view,
			 file,
			 dory_icon_view_using_auto_layout (icon_view));
	}

	update_layout_menus (icon_view);
}

static gboolean
dory_icon_view_can_rename_file (DoryView *view, DoryFile *file)
{
	if (!(dory_icon_view_get_zoom_level (view) > DORY_ZOOM_LEVEL_SMALLEST)) {
		return FALSE;
	}

	return DORY_VIEW_CLASS(dory_icon_view_parent_class)->can_rename_file (view, file);
}

static void
dory_icon_view_start_renaming_file (DoryView *view,
				  DoryFile *file,
				  gboolean select_all)
{
	/* call parent class to make sure the right icon is selected */
	DORY_VIEW_CLASS(dory_icon_view_parent_class)->start_renaming_file (view, file, select_all);

	/* start renaming */
	dory_icon_container_start_renaming_selected_item
		(get_icon_container (DORY_ICON_VIEW (view)), select_all);
}

static const GtkActionEntry icon_view_entries[] = {
  /* name, stock id, label */  { "Arrange Items", NULL, N_("Arran_ge Items") },
  /* name, stock id */         { "Clean Up", NULL,
  /* label, accelerator */       N_("_Organize by Name"), NULL,
  /* tooltip */                  N_("Reposition icons to better fit in the window and avoid overlapping"),
                                 G_CALLBACK (action_clean_up_callback) },
};

static const GtkToggleActionEntry icon_view_toggle_entries[] = {

  /* name, stock id */      { "Reversed Order", NULL,
  /* label, accelerator */    N_("Re_versed Order"), NULL,
  /* tooltip */               N_("Display icons in the opposite order"),
                              G_CALLBACK (action_reversed_order_callback),
                              0 },
  /* name, stock id */      { "Keep Aligned", NULL,
  /* label, accelerator */    N_("_Keep Aligned"), NULL,
  /* tooltip */               N_("Keep icons lined up on a grid"),
                              G_CALLBACK (action_keep_aligned_callback),
                              0 },
};

static const GtkRadioActionEntry arrange_radio_entries[] = {
  { "Manual Layout", NULL,
    N_("_Manually"), NULL,
    N_("Leave icons wherever they are dropped"),
    DORY_FILE_SORT_NONE },
  { "Sort by Name", NULL,
    N_("By _Name"), NULL,
    N_("Keep icons sorted by name in rows"),
    DORY_FILE_SORT_BY_DISPLAY_NAME },
  { "Sort by Size", NULL,
    N_("By _Size"), NULL,
    N_("Keep icons sorted by size in rows"),
    DORY_FILE_SORT_BY_SIZE },
  { "Sort by Type", NULL,
    N_("By _Type"), NULL,
    N_("Keep icons sorted by type in rows"),
    DORY_FILE_SORT_BY_TYPE },
  { "Sort by Detailed Type", NULL,
    N_("By _Detailed Type"), NULL,
    N_("Keep icons sorted by detailed type in rows"),
    DORY_FILE_SORT_BY_DETAILED_TYPE },
  { "Sort by Modification Date", NULL,
    N_("By Modification _Date"), NULL,
    N_("Keep icons sorted by modification date in rows"),
    DORY_FILE_SORT_BY_MTIME },
  { "Sort by Trash Time", NULL,
    N_("By T_rash Time"), NULL,
    N_("Keep icons sorted by trash time in rows"),
    DORY_FILE_SORT_BY_TRASHED_TIME },
  { "Sort by Extension", NULL,
    N_("By _Extension"), NULL,
    N_("Keep icons sorted by extension in rows"),
    DORY_FILE_SORT_BY_EXTENSION },
};

static void
dory_icon_view_merge_menus (DoryView *view)
{
	DoryIconView *icon_view;
	GtkUIManager *ui_manager;
	GtkActionGroup *action_group;
	GtkAction *action;

        g_assert (DORY_IS_ICON_VIEW (view));

	DORY_VIEW_CLASS (dory_icon_view_parent_class)->merge_menus (view);

	icon_view = DORY_ICON_VIEW (view);

	ui_manager = dory_view_get_ui_manager (DORY_VIEW (icon_view));

	action_group = gtk_action_group_new ("IconViewActions");
	gtk_action_group_set_translation_domain (action_group, GETTEXT_PACKAGE);
	icon_view->details->icon_action_group = action_group;
	gtk_action_group_add_actions (action_group,
				      icon_view_entries, G_N_ELEMENTS (icon_view_entries),
				      icon_view);
	gtk_action_group_add_toggle_actions (action_group,
					     icon_view_toggle_entries, G_N_ELEMENTS (icon_view_toggle_entries),
					     icon_view);
	gtk_action_group_add_radio_actions (action_group,
					    arrange_radio_entries,
					    G_N_ELEMENTS (arrange_radio_entries),
					    -1,
					    G_CALLBACK (action_sort_radio_callback),
					    icon_view);

	gtk_ui_manager_insert_action_group (ui_manager, action_group, 0);
	g_object_unref (action_group); /* owned by ui manager */

	icon_view->details->icon_merge_id =
		gtk_ui_manager_add_ui_from_resource (ui_manager, "/org/dory/dory-icon-view-ui.xml", NULL);

	/* Do one-time state-setting here; context-dependent state-setting
	 * is done in update_menus.
	 */
	if (!dory_icon_view_supports_auto_layout (icon_view)) {
		action = gtk_action_group_get_action (action_group,
						      DORY_ACTION_ARRANGE_ITEMS);
		gtk_action_set_visible (action, FALSE);
	}

	update_layout_menus (icon_view);
}

static void
dory_icon_view_unmerge_menus (DoryView *view)
{
	DoryIconView *icon_view;
	GtkUIManager *ui_manager;

	icon_view = DORY_ICON_VIEW (view);

	DORY_VIEW_CLASS (dory_icon_view_parent_class)->unmerge_menus (view);

	ui_manager = dory_view_get_ui_manager (view);
	if (ui_manager != NULL) {
		dory_ui_unmerge_ui (ui_manager,
					&icon_view->details->icon_merge_id,
					&icon_view->details->icon_action_group);
	}
}

static void
dory_icon_view_update_menus (DoryView *view)
{
    DoryIconView *icon_view;
    GtkAction *action;
    gboolean editable;

    icon_view = DORY_ICON_VIEW (view);

	DORY_VIEW_CLASS (dory_icon_view_parent_class)->update_menus(view);

	editable = dory_view_is_editable (view);
	action = gtk_action_group_get_action (icon_view->details->icon_action_group,
					      DORY_ACTION_MANUAL_LAYOUT);
	gtk_action_set_sensitive (action, editable);
}

static void
dory_icon_view_reset_to_defaults (DoryView *view)
{
	DoryIconContainer *icon_container;
	DoryIconView *icon_view;

	icon_view = DORY_ICON_VIEW (view);
	icon_container = get_icon_container (icon_view);

	clear_sort_criterion (icon_view);
	dory_icon_container_set_keep_aligned
		(icon_container, get_default_directory_keep_aligned ());

	dory_icon_container_sort (icon_container);

	update_layout_menus (icon_view);

	dory_icon_view_restore_default_zoom_level (view);

    if (dory_global_preferences_get_ignore_view_metadata ()) {
        DoryWindow *window = dory_view_get_dory_window (view);
        dory_window_set_ignore_meta_zoom_level (window, DORY_ZOOM_LEVEL_NULL);
    }
}

static void
dory_icon_view_select_all (DoryView *view)
{
	DoryIconContainer *icon_container;

	g_return_if_fail (DORY_IS_ICON_VIEW (view));

	icon_container = get_icon_container (DORY_ICON_VIEW (view));
        dory_icon_container_select_all (icon_container);
}

static void
dory_icon_view_reveal_selection (DoryView *view)
{
	GList *selection;

	g_return_if_fail (DORY_IS_ICON_VIEW (view));

        selection = dory_view_get_selection (view);

	/* Make sure at least one of the selected items is scrolled into view */
	if (selection != NULL) {
		dory_icon_container_reveal
			(get_icon_container (DORY_ICON_VIEW (view)),
			 selection->data);
	}

        dory_file_list_free (selection);
}

static GArray *
dory_icon_view_get_selected_icon_locations (DoryView *view)
{
	g_return_val_if_fail (DORY_IS_ICON_VIEW (view), NULL);

	return dory_icon_container_get_selected_icon_locations
		(get_icon_container (DORY_ICON_VIEW (view)));
}


static void
dory_icon_view_set_selection (DoryView *view, GList *selection)
{
	g_return_if_fail (DORY_IS_ICON_VIEW (view));

	dory_icon_container_set_selection
		(get_icon_container (DORY_ICON_VIEW (view)), selection);
}

static void
dory_icon_view_invert_selection (DoryView *view)
{
	g_return_if_fail (DORY_IS_ICON_VIEW (view));

	dory_icon_container_invert_selection
		(get_icon_container (DORY_ICON_VIEW (view)));
}

static gboolean
dory_icon_view_using_manual_layout (DoryView *view)
{
	g_return_val_if_fail (DORY_IS_ICON_VIEW (view), FALSE);

	return !dory_icon_view_using_auto_layout (DORY_ICON_VIEW (view));
}

static void
dory_icon_view_widget_to_file_operation_position (DoryView *view,
						GdkPoint *position)
{
	g_assert (DORY_IS_ICON_VIEW (view));

	dory_icon_container_widget_to_file_operation_position
		(get_icon_container (DORY_ICON_VIEW (view)), position);
}

static void
icon_container_activate_callback (DoryIconContainer *container,
				  GList *file_list,
				  DoryIconView *icon_view)
{
	g_assert (DORY_IS_ICON_VIEW (icon_view));
	g_assert (container == get_icon_container (icon_view));

	dory_view_activate_files (DORY_VIEW (icon_view),
				      file_list,
				      0, TRUE);
}

static void
icon_container_activate_previewer_callback (DoryIconContainer *container,
					    GList *file_list,
					    GArray *locations,
					    DoryIconView *icon_view)
{
	g_assert (DORY_IS_ICON_VIEW (icon_view));
	g_assert (container == get_icon_container (icon_view));

	dory_view_preview_files (DORY_VIEW (icon_view),
				     file_list, locations);
}

/* this is called in one of these cases:
 * - we activate with enter holding shift
 * - we activate with space holding shift
 * - we double click an icon holding shift
 * - we middle click an icon
 *
 * If we don't open in new windows by default, the behavior should be
 * - middle click, shift + activate -> open in new tab
 * - shift + double click -> open in new window
 *
 * If we open in new windows by default, the behaviour should be
 * - middle click, or shift + activate, or shift + double-click -> close parent
 */
static void
icon_container_activate_alternate_callback (DoryIconContainer *container,
					    GList *file_list,
					    DoryIconView *icon_view)
{
	GdkEvent *event;
	GdkEventButton *button_event;
	GdkEventKey *key_event;
	gboolean open_in_tab, open_in_window, close_behind;
	DoryWindowOpenFlags flags;

	g_assert (DORY_IS_ICON_VIEW (icon_view));
	g_assert (container == get_icon_container (icon_view));

	flags = 0;
	event = gtk_get_current_event ();
	open_in_tab = FALSE;
	open_in_window = FALSE;
	close_behind = FALSE;

	if (g_settings_get_boolean (dory_preferences,
				    DORY_PREFERENCES_ALWAYS_USE_BROWSER)) {
		if (event->type == GDK_BUTTON_PRESS ||
		    event->type == GDK_BUTTON_RELEASE ||
		    event->type == GDK_2BUTTON_PRESS ||
		    event->type == GDK_3BUTTON_PRESS) {
			button_event = (GdkEventButton *) event;
			open_in_window = ((button_event->state & GDK_SHIFT_MASK) != 0);
			open_in_tab = !open_in_window;
		} else if (event->type == GDK_KEY_PRESS ||
			   event->type == GDK_KEY_RELEASE) {
			key_event = (GdkEventKey *) event;
			open_in_tab = ((key_event->state & GDK_SHIFT_MASK) != 0);
		}
	} else {
		close_behind = TRUE;
	}

	if (open_in_tab) {
		flags |= DORY_WINDOW_OPEN_FLAG_NEW_TAB;
	}

	if (open_in_window) {
		flags |= DORY_WINDOW_OPEN_FLAG_NEW_WINDOW;
	}

	if (close_behind) {
		flags |= DORY_WINDOW_OPEN_FLAG_CLOSE_BEHIND;
	}

	DEBUG ("Activate alternate, open in tab %d, close behind %d, new window %d\n",
	       open_in_tab, close_behind, open_in_window);

	dory_view_activate_files (DORY_VIEW (icon_view),
				      file_list,
				      flags,
				      TRUE);
}

static void
band_select_started_callback (DoryIconContainer *container,
			      DoryIconView *icon_view)
{
	g_assert (DORY_IS_ICON_VIEW (icon_view));
	g_assert (container == get_icon_container (icon_view));

	dory_view_start_batching_selection_changes (DORY_VIEW (icon_view));
}

static void
band_select_ended_callback (DoryIconContainer *container,
			    DoryIconView *icon_view)
{
	g_assert (DORY_IS_ICON_VIEW (icon_view));
	g_assert (container == get_icon_container (icon_view));

	dory_view_stop_batching_selection_changes (DORY_VIEW (icon_view));
}

int
dory_icon_view_compare_files (DoryIconView   *icon_view,
				  DoryFile *a,
				  DoryFile *b)
{
	DoryView *view = DORY_VIEW (icon_view);
	DoryIconContainer *container = dory_icon_view_get_icon_container (icon_view);

	if (container != NULL &&
	    dory_icon_container_get_filter_highlight (container) != NULL) {
		gint pos_a = dory_view_get_filter_match (view, a);
		gint pos_b = dory_view_get_filter_match (view, b);

		if (pos_a != pos_b) {
			return (pos_a < pos_b) ? -1 : 1;
		}
	}

	return dory_file_compare_for_sort
		(a, b, icon_view->details->sort->sort_type,
		 /* Use type-unsafe cast for performance */
		 dory_view_should_sort_directories_first ((DoryView *)icon_view),
		 dory_view_should_sort_favorites_first ((DoryView *)icon_view),
		 icon_view->details->sort_reversed,
         NULL);
}

static int
compare_files (DoryView   *icon_view,
	       DoryFile *a,
	       DoryFile *b)
{
	return dory_icon_view_compare_files ((DoryIconView *)icon_view, a, b);
}

static void
dory_icon_view_screen_changed (GtkWidget *widget,
				   GdkScreen *previous_screen)
{
	DoryView *view;
	GList *files, *l;
	DoryFile *file;
	DoryDirectory *directory;
	DoryIconContainer *icon_container;

	if (GTK_WIDGET_CLASS (dory_icon_view_parent_class)->screen_changed) {
		GTK_WIDGET_CLASS (dory_icon_view_parent_class)->screen_changed (widget, previous_screen);
	}

	view = DORY_VIEW (widget);
	if (DORY_ICON_VIEW (view)->details->is_desktop) {
		icon_container = get_icon_container (DORY_ICON_VIEW (view));

		directory = dory_view_get_model (view);
		files = dory_directory_get_file_list (directory);

		for (l = files; l != NULL; l = l->next) {
			file = l->data;

			if (!should_show_file_on_screen (view, file)) {
				dory_icon_view_remove_file (view, file, directory);
			} else {
				if (dory_icon_container_add (icon_container,
								 DORY_ICON_CONTAINER_ICON_DATA (file))) {
					dory_file_ref (file);
				}
			}
		}

		dory_file_list_unref (files);
		g_list_free (files);
	}
}

static gboolean
dory_icon_view_scroll_event (GtkWidget *widget,
			   GdkEventScroll *scroll_event)
{
	DoryIconView *icon_view;
	GdkEvent *event_copy;
	GdkEventScroll *scroll_event_copy;
	gboolean ret;

	icon_view = DORY_ICON_VIEW (widget);

	if (icon_view->details->compact &&
	    (scroll_event->direction == GDK_SCROLL_UP ||
	     scroll_event->direction == GDK_SCROLL_DOWN ||
	     scroll_event->direction == GDK_SCROLL_SMOOTH)) {
		ret = dory_view_handle_scroll_event (DORY_VIEW (icon_view), scroll_event);
		if (!ret) {
			/* in column-wise layout, re-emit vertical mouse scroll events as horizontal ones,
			 * if they don't bump zoom */
			event_copy = gdk_event_copy ((GdkEvent *) scroll_event);
			scroll_event_copy = (GdkEventScroll *) event_copy;

			/* transform vertical integer smooth scroll events into horizontal events */
			if (scroll_event_copy->direction == GDK_SCROLL_SMOOTH &&
				   scroll_event_copy->delta_x == 0) {
				if (scroll_event_copy->delta_y == 1.0) {
					scroll_event_copy->direction = GDK_SCROLL_DOWN;
				} else if (scroll_event_copy->delta_y == -1.0) {
					scroll_event_copy->direction = GDK_SCROLL_UP;
				}
			}

			if (scroll_event_copy->direction == GDK_SCROLL_UP) {
				scroll_event_copy->direction = GDK_SCROLL_LEFT;
			} else if (scroll_event_copy->direction == GDK_SCROLL_DOWN) {
				scroll_event_copy->direction = GDK_SCROLL_RIGHT;
			}

			ret = GTK_WIDGET_CLASS (dory_icon_view_parent_class)->scroll_event (widget, scroll_event_copy);
			gdk_event_free (event_copy);
		}

		return ret;
	}

	return GTK_WIDGET_CLASS (dory_icon_view_parent_class)->scroll_event (widget, scroll_event);
}

static void
selection_changed_callback (DoryIconContainer *container,
			    DoryIconView *icon_view)
{
	g_assert (DORY_IS_ICON_VIEW (icon_view));
	g_assert (container == get_icon_container (icon_view));

	dory_view_notify_selection_changed (DORY_VIEW (icon_view));
}

static void
icon_container_context_click_selection_callback (DoryIconContainer *container,
						 GdkEventButton *event,
						 DoryIconView *icon_view)
{
	g_assert (DORY_IS_ICON_CONTAINER (container));
	g_assert (DORY_IS_ICON_VIEW (icon_view));

	dory_view_pop_up_selection_context_menu
		(DORY_VIEW (icon_view), event);
}

static void
icon_container_context_click_background_callback (DoryIconContainer *container,
						  GdkEventButton *event,
						  DoryIconView *icon_view)
{
	g_assert (DORY_IS_ICON_CONTAINER (container));
	g_assert (DORY_IS_ICON_VIEW (icon_view));

	dory_view_pop_up_background_context_menu
		(DORY_VIEW (icon_view), event);
}

static gboolean
dory_icon_view_react_to_icon_change_idle_callback (gpointer data)
{
        DoryIconView *icon_view;

        g_assert (DORY_IS_ICON_VIEW (data));

        icon_view = DORY_ICON_VIEW (data);
        icon_view->details->react_to_icon_change_idle_id = 0;

	/* Rebuild the menus since some of them (e.g. Restore Stretched Icons)
	 * may be different now.
	 */
	dory_view_update_menus (DORY_VIEW (icon_view));

        /* Don't call this again (unless rescheduled) */
        return FALSE;
}

static void
icon_position_changed_callback (DoryIconContainer *container,
				DoryFile *file,
				const DoryIconPosition *position,
				DoryIconView *icon_view)
{
	char scale_string[G_ASCII_DTOSTR_BUF_SIZE];

	g_assert (DORY_IS_ICON_VIEW (icon_view));
	g_assert (container == get_icon_container (icon_view));
	g_assert (DORY_IS_FILE (file));

	/* Schedule updating menus for the next idle. Doing it directly here
	 * noticeably slows down icon stretching.  The other work here to
	 * store the icon position and scale does not seem to noticeably
	 * slow down icon stretching. It would be trickier to move to an
	 * idle call, because we'd have to keep track of potentially multiple
	 * sets of file/geometry info.
	 */
	if (dory_view_get_active (DORY_VIEW (icon_view)) &&
	    icon_view->details->react_to_icon_change_idle_id == 0) {
                icon_view->details->react_to_icon_change_idle_id
                        = g_idle_add (dory_icon_view_react_to_icon_change_idle_callback,
				      icon_view);
	}

	/* Store the new position of the icon in the metadata. */
	if (!dory_file_get_is_desktop_orphan (file)) {
		dory_file_set_position (file, position->x, position->y);
        dory_file_set_monitor_number (file, position->monitor);
	}

	g_ascii_dtostr (scale_string, sizeof (scale_string), position->scale);

    sync_directory_monitor_number (icon_view, file);

	dory_file_set_metadata (file, DORY_METADATA_KEY_ICON_SCALE, "1.0", scale_string);
}

/* Attempt to change the filename to the new text.  Notify user if operation fails. */
static void
icon_rename_ended_cb (DoryIconContainer *container,
		      DoryFile *file,
		      const char *new_name,
		      DoryIconView *icon_view)
{
	g_assert (DORY_IS_FILE (file));

	dory_view_set_is_renaming (DORY_VIEW (icon_view), FALSE);

	/* Don't allow a rename with an empty string. Revert to original
	 * without notifying the user.
	 */
	if ((new_name == NULL) || (new_name[0] == '\0')) {
		return;
	}

	dory_rename_file (file, new_name, NULL, NULL);
}

static void
icon_rename_started_cb (DoryIconContainer *container,
			GtkWidget *widget,
			gpointer callback_data)
{
	DoryView *directory_view;

	directory_view = DORY_VIEW (callback_data);
	dory_clipboard_set_up_editable
		(GTK_EDITABLE (widget),
		 dory_view_get_ui_manager (directory_view),
		 FALSE);
}

static char *
get_icon_uri_callback (DoryIconContainer *container,
		       DoryFile *file,
		       DoryIconView *icon_view)
{
	g_assert (DORY_IS_ICON_CONTAINER (container));
	g_assert (DORY_IS_FILE (file));
	g_assert (DORY_IS_ICON_VIEW (icon_view));

	return dory_file_get_local_uri (file);
}

static char *
get_icon_drop_target_uri_callback (DoryIconContainer *container,
		       		   DoryFile *file,
		       		   DoryIconView *icon_view)
{
	g_return_val_if_fail (DORY_IS_ICON_CONTAINER (container), NULL);
	g_return_val_if_fail (DORY_IS_FILE (file), NULL);
	g_return_val_if_fail (DORY_IS_ICON_VIEW (icon_view), NULL);

	return dory_file_get_drop_target_uri (file);
}

/* Preferences changed callbacks */
static void
dory_icon_view_click_policy_changed (DoryView *directory_view)
{
    g_assert (DORY_IS_ICON_VIEW (directory_view));

    dory_icon_view_update_click_mode (DORY_ICON_VIEW (directory_view));
}

static void
dory_icon_view_click_to_rename_mode_changed (DoryView *directory_view)
{
    g_assert (DORY_IS_ICON_VIEW (directory_view));

    dory_icon_view_update_click_to_rename_mode (DORY_ICON_VIEW (directory_view));
}

static void
image_display_policy_changed_callback (gpointer callback_data)
{
	DoryIconView *icon_view;

	icon_view = DORY_ICON_VIEW (callback_data);

	dory_icon_container_request_update_all (get_icon_container (icon_view));
}

static void
text_attribute_names_changed_callback (gpointer callback_data)
{
	DoryIconView *icon_view;

	icon_view = DORY_ICON_VIEW (callback_data);

	dory_icon_container_request_update_all (get_icon_container (icon_view));
}

static void
default_sort_order_changed_callback (gpointer callback_data)
{
	DoryIconView *icon_view;
	DoryFile *file;
	char *sort_name;
	DoryIconContainer *icon_container;

	g_return_if_fail (DORY_IS_ICON_VIEW (callback_data));

	icon_view = DORY_ICON_VIEW (callback_data);

	file = dory_view_get_directory_as_file (DORY_VIEW (icon_view));
	sort_name = dory_icon_view_get_directory_sort_by (icon_view, file);
	set_sort_criterion (icon_view, get_sort_criterion_by_metadata_text (sort_name), FALSE);
	g_free (sort_name);

	icon_container = get_icon_container (icon_view);
	g_return_if_fail (DORY_IS_ICON_CONTAINER (icon_container));

	dory_icon_container_request_update_all (icon_container);
}

static void
default_sort_in_reverse_order_changed_callback (gpointer callback_data)
{
	DoryIconView *icon_view;
	DoryFile *file;
	DoryIconContainer *icon_container;

	g_return_if_fail (DORY_IS_ICON_VIEW (callback_data));

	icon_view = DORY_ICON_VIEW (callback_data);

	file = dory_view_get_directory_as_file (DORY_VIEW (icon_view));
	dory_icon_view_set_sort_reversed (icon_view, dory_icon_view_get_directory_sort_reversed (icon_view, file), FALSE);
	icon_container = get_icon_container (icon_view);
	g_return_if_fail (DORY_IS_ICON_CONTAINER (icon_container));

	dory_icon_container_request_update_all (icon_container);
}

static void
default_zoom_level_changed_callback (gpointer callback_data)
{
	DoryIconView *icon_view;
	DoryFile *file;
	int level;

	g_return_if_fail (DORY_IS_ICON_VIEW (callback_data));

	icon_view = DORY_ICON_VIEW (callback_data);

	if (dory_view_supports_zooming (DORY_VIEW (icon_view))) {
		file = dory_view_get_directory_as_file (DORY_VIEW (icon_view));

        if (dory_global_preferences_get_ignore_view_metadata () &&
            dory_window_get_ignore_meta_zoom_level (dory_view_get_dory_window (DORY_VIEW (icon_view))) > -1) {
            level = dory_window_get_ignore_meta_zoom_level (dory_view_get_dory_window (DORY_VIEW (icon_view)));
        } else {
            sync_directory_monitor_number (icon_view, file);

            if (dory_icon_view_is_compact (icon_view)) {
                level = dory_file_get_integer_metadata (file,
                                                        DORY_METADATA_KEY_COMPACT_VIEW_ZOOM_LEVEL,
                                                        get_default_zoom_level (icon_view));
            } else {
                level = dory_file_get_integer_metadata (file,
                                                        DORY_METADATA_KEY_ICON_VIEW_ZOOM_LEVEL,
                                                        get_default_zoom_level (icon_view));
            }
        }
        dory_view_zoom_to_level (DORY_VIEW (icon_view), level);
    }
}

static void
labels_beside_icons_changed_callback (gpointer callback_data)
{
	DoryIconView *icon_view;

	g_return_if_fail (DORY_IS_ICON_VIEW (callback_data));

	icon_view = DORY_ICON_VIEW (callback_data);

	set_labels_beside_icons (icon_view);
}

static void
all_columns_same_width_changed_callback (gpointer callback_data)
{
	DoryIconView *icon_view;

	g_assert (DORY_IS_ICON_VIEW (callback_data));

	icon_view = DORY_ICON_VIEW (callback_data);

	set_columns_same_width (icon_view);
}


static void
dory_icon_view_sort_directories_first_changed (DoryView *directory_view)
{
	DoryIconView *icon_view;

	icon_view = DORY_ICON_VIEW (directory_view);

	if (dory_icon_view_using_auto_layout (icon_view)) {
		dory_icon_container_sort
			(get_icon_container (icon_view));
	}
}

static void
dory_icon_view_sort_favorites_first_changed (DoryView *directory_view)
{
	DoryIconView *icon_view;

	icon_view = DORY_ICON_VIEW (directory_view);

	if (dory_icon_view_using_auto_layout (icon_view)) {
		dory_icon_container_sort
			(get_icon_container (icon_view));
	}
}

static gboolean
icon_view_can_accept_item (DoryIconContainer *container,
			   DoryFile *target_item,
			   const char *item_uri,
			   DoryView *view)
{
	return dory_drag_can_accept_item (target_item, item_uri);
}

static char *
icon_view_get_container_uri (DoryIconContainer *container,
			     DoryView *view)
{
	return dory_view_get_uri (view);
}

static void
icon_view_move_copy_items (DoryIconContainer *container,
			   const GList *item_uris,
			   GArray *relative_item_points,
			   const char *target_dir,
			   int copy_action,
			   int x, int y,
			   DoryView *view)
{
	dory_clipboard_clear_if_colliding_uris (GTK_WIDGET (view),
						    item_uris,
						    dory_view_get_copied_files_atom (view));
	dory_view_move_copy_items (view, item_uris, relative_item_points, target_dir,
				       copy_action, x, y);
}

static void
dory_icon_view_update_click_mode (DoryIconView *icon_view)
{
	DoryIconContainer	*icon_container;
	int			click_mode;

	icon_container = get_icon_container (icon_view);
	g_assert (icon_container != NULL);

	click_mode = g_settings_get_enum (dory_preferences, DORY_PREFERENCES_CLICK_POLICY);

	dory_icon_container_set_single_click_mode (icon_container,
						       click_mode == DORY_CLICK_POLICY_SINGLE);
}

static void
dory_icon_view_update_click_to_rename_mode (DoryIconView *icon_view)
{
    DoryIconContainer   *icon_container;
    gboolean enabled;

    icon_container = get_icon_container (icon_view);
    g_assert (icon_container != NULL);

    enabled = g_settings_get_boolean (dory_preferences, DORY_PREFERENCES_CLICK_TO_RENAME);

    dory_icon_container_set_click_to_rename_enabled (icon_container,
                                                     enabled);
}

static gboolean
get_stored_layout_timestamp (DoryIconContainer *container,
			     DoryIconData *icon_data,
			     time_t *timestamp,
			     DoryIconView *view)
{
	DoryFile *file;
	DoryDirectory *directory;

	if (icon_data == NULL) {
		directory = dory_view_get_model (DORY_VIEW (view));
		if (directory == NULL) {
			return FALSE;
		}

		file = dory_directory_get_corresponding_file (directory);

        sync_directory_monitor_number (view, file);

        *timestamp = dory_file_get_time_metadata (file, DORY_METADATA_KEY_ICON_VIEW_LAYOUT_TIMESTAMP);

		dory_file_unref (file);
	} else {
        sync_directory_monitor_number (view, DORY_FILE (icon_data));

        *timestamp = dory_file_get_time_metadata (DORY_FILE (icon_data), DORY_METADATA_KEY_ICON_POSITION_TIMESTAMP);
	}

	return TRUE;
}

static gboolean
store_layout_timestamp (DoryIconContainer *container,
			DoryIconData *icon_data,
			const time_t *timestamp,
			DoryIconView *view)
{
	DoryFile *file;
	DoryDirectory *directory;

	if (icon_data == NULL) {
		directory = dory_view_get_model (DORY_VIEW (view));
		if (directory == NULL) {
			return FALSE;
		}

		file = dory_directory_get_corresponding_file (directory);

        sync_directory_monitor_number (view, file);

		dory_file_set_time_metadata (file,
                                     DORY_METADATA_KEY_ICON_VIEW_LAYOUT_TIMESTAMP,
                                     (time_t) *timestamp);
		dory_file_unref (file);
	} else {
        sync_directory_monitor_number (view, DORY_FILE (icon_data));

		dory_file_set_time_metadata (DORY_FILE (icon_data),
                                     DORY_METADATA_KEY_ICON_POSITION_TIMESTAMP,
                                     (time_t) *timestamp);
	}

	return TRUE;
}

static gboolean
focus_in_event_callback (GtkWidget *widget, GdkEventFocus *event, gpointer user_data)
{
	DoryWindowSlot *slot;
	DoryIconView *icon_view = DORY_ICON_VIEW (user_data);

	/* make the corresponding slot (and the pane that contains it) active */
	slot = dory_view_get_dory_window_slot (DORY_VIEW (icon_view));
	dory_window_slot_make_hosting_pane_active (slot);

	return FALSE;
}

static gboolean
button_press_callback (GtkWidget *widget, GdkEventFocus *event, gpointer user_data)
{
    DoryView *view = DORY_VIEW (user_data);
    GdkEventButton *event_button = (GdkEventButton *)event;
    gint selection_count = dory_view_get_selection_count (DORY_VIEW (view));

    if (!dory_view_get_active (view) && selection_count > 0) {
        DoryWindowSlot *slot = dory_view_get_dory_window_slot (view);
        dory_window_slot_make_hosting_pane_active (slot);
        return GDK_EVENT_STOP;
    }

    /* double left click on blank will go to parent folder */
    if (g_settings_get_boolean (dory_preferences, DORY_PREFERENCES_CLICK_DOUBLE_PARENT_FOLDER) &&
        (event_button->button == 1) && (event_button->type == GDK_2BUTTON_PRESS)) {
        if (selection_count == 0) {
            DoryWindowSlot *slot = dory_view_get_dory_window_slot (view);
            dory_window_slot_go_up (slot, 0);
            return GDK_EVENT_STOP;
        }
    }

    return GDK_EVENT_PROPAGATE;
}

static void
dory_icon_view_update_filter_text (DoryView   *view,
                                   const char *filter_text)
{
    DoryIconContainer *container;

    container = dory_icon_view_get_icon_container (DORY_ICON_VIEW (view));
    if (container != NULL) {
        dory_icon_container_set_filter_highlight (container, filter_text);
    }
}

static void
dory_icon_view_select_first (DoryView *view)
{
    DoryIconContainer *container;

    container = dory_icon_view_get_icon_container (DORY_ICON_VIEW (view));
    if (container != NULL) {
        dory_icon_container_select_first (container);
    }
}

static gboolean
icon_container_activate_filter_cb (DoryIconContainer *container,
                                   GdkEvent *event,
                                   DoryIconView *icon_view)
{
    return dory_view_activate_filter (DORY_VIEW (icon_view), (GdkEventKey *) event);
}

static DoryIconContainer *
create_icon_container (DoryIconView *icon_view)
{
	DoryIconContainer *icon_container;

    if (DORY_ICON_VIEW_GET_CLASS (icon_view)->use_grid_container) {
        icon_container = dory_icon_view_grid_container_new (icon_view,
                                                            icon_view->details->is_desktop);
    } else {
        icon_container = dory_icon_view_container_new (icon_view,
                                                       icon_view->details->is_desktop);
    }

	icon_view->details->icon_container = GTK_WIDGET (icon_container);
	g_object_add_weak_pointer (G_OBJECT (icon_container),
				   (gpointer *) &icon_view->details->icon_container);

	gtk_widget_set_can_focus (GTK_WIDGET (icon_container), TRUE);

    g_signal_connect_object (icon_container, "button_press_event",
                 G_CALLBACK (button_press_callback), icon_view, 0);
	g_signal_connect_object (icon_container, "focus_in_event",
				 G_CALLBACK (focus_in_event_callback), icon_view, 0);
	g_signal_connect_object (icon_container, "activate",
				 G_CALLBACK (icon_container_activate_callback), icon_view, 0);
	g_signal_connect_object (icon_container, "activate_alternate",
				 G_CALLBACK (icon_container_activate_alternate_callback), icon_view, 0);
	g_signal_connect_object (icon_container, "activate_previewer",
				 G_CALLBACK (icon_container_activate_previewer_callback), icon_view, 0);
	g_signal_connect_object (icon_container, "band_select_started",
				 G_CALLBACK (band_select_started_callback), icon_view, 0);
	g_signal_connect_object (icon_container, "band_select_ended",
				 G_CALLBACK (band_select_ended_callback), icon_view, 0);
	g_signal_connect_object (icon_container, "context_click_selection",
				 G_CALLBACK (icon_container_context_click_selection_callback), icon_view, 0);
	g_signal_connect_object (icon_container, "context_click_background",
				 G_CALLBACK (icon_container_context_click_background_callback), icon_view, 0);
	g_signal_connect_object (icon_container, "icon_position_changed",
				 G_CALLBACK (icon_position_changed_callback), icon_view, 0);
	g_signal_connect_object (icon_container, "selection_changed",
				 G_CALLBACK (selection_changed_callback), icon_view, 0);
	/* FIXME: many of these should move into fm-icon-container as virtual methods */
	g_signal_connect_object (icon_container, "get_icon_uri",
				 G_CALLBACK (get_icon_uri_callback), icon_view, 0);
	g_signal_connect_object (icon_container, "get_icon_drop_target_uri",
				 G_CALLBACK (get_icon_drop_target_uri_callback), icon_view, 0);
	g_signal_connect_object (icon_container, "move_copy_items",
				 G_CALLBACK (icon_view_move_copy_items), icon_view, 0);
	g_signal_connect_object (icon_container, "get_container_uri",
				 G_CALLBACK (icon_view_get_container_uri), icon_view, 0);
	g_signal_connect_object (icon_container, "can_accept_item",
				 G_CALLBACK (icon_view_can_accept_item), icon_view, 0);
	g_signal_connect_object (icon_container, "layout_changed",
				 G_CALLBACK (layout_changed_callback), icon_view, 0);
	g_signal_connect_object (icon_container, "icon_rename_started",
				 G_CALLBACK (icon_rename_started_cb), icon_view, 0);
	g_signal_connect_object (icon_container, "icon_rename_ended",
				 G_CALLBACK (icon_rename_ended_cb), icon_view, 0);
	g_signal_connect_object (icon_container, "icon_stretch_started",
				 G_CALLBACK (dory_view_update_menus), icon_view,
				 G_CONNECT_SWAPPED);
	g_signal_connect_object (icon_container, "icon_stretch_ended",
				 G_CALLBACK (dory_view_update_menus), icon_view,
				 G_CONNECT_SWAPPED);

	g_signal_connect_object (icon_container, "get_stored_layout_timestamp",
				 G_CALLBACK (get_stored_layout_timestamp), icon_view, 0);
	g_signal_connect_object (icon_container, "store_layout_timestamp",
				 G_CALLBACK (store_layout_timestamp), icon_view, 0);
	g_signal_connect_object (icon_container, "check-filter-event",
				 G_CALLBACK (icon_container_activate_filter_cb), icon_view, 0);

	gtk_container_add (GTK_CONTAINER (icon_view),
			   GTK_WIDGET (icon_container));

	dory_icon_view_update_click_mode (icon_view);
    dory_icon_view_update_click_to_rename_mode (icon_view);

	gtk_widget_show (GTK_WIDGET (icon_container));

	return icon_container;
}

/* Handles an URL received from Mozilla */
static void
icon_view_handle_netscape_url (DoryIconContainer *container, const char *encoded_url,
			       const char *target_uri,
			       GdkDragAction action, int x, int y, DoryIconView *view)
{
	dory_view_handle_netscape_url_drop (DORY_VIEW (view),
						encoded_url, target_uri, action, x, y);
}

static void
icon_view_handle_uri_list (DoryIconContainer *container, const char *item_uris,
			   const char *target_uri,
			   GdkDragAction action, int x, int y, DoryIconView *view)
{
	dory_view_handle_uri_list_drop (DORY_VIEW (view),
					    item_uris, target_uri, action, x, y);
}

static void
icon_view_handle_text (DoryIconContainer *container, const char *text,
		       const char *target_uri,
		       GdkDragAction action, int x, int y, DoryIconView *view)
{
	dory_view_handle_text_drop (DORY_VIEW (view),
					text, target_uri, action, x, y);
}

static void
icon_view_handle_raw (DoryIconContainer *container, const char *raw_data,
		      int length, const char *target_uri, const char *direct_save_uri,
		      GdkDragAction action, int x, int y, DoryIconView *view)
{
	dory_view_handle_raw_drop (DORY_VIEW (view),
				       raw_data, length, target_uri, direct_save_uri, action, x, y);
}

static char *
icon_view_get_first_visible_file (DoryView *view)
{
	DoryFile *file;
	DoryIconView *icon_view;

	icon_view = DORY_ICON_VIEW (view);

	file = DORY_FILE (dory_icon_container_get_first_visible_icon (get_icon_container (icon_view)));

	if (file) {
		return dory_file_get_uri (file);
	}

	return NULL;
}

static void
icon_view_scroll_to_file (DoryView *view,
			  const char *uri)
{
	DoryFile *file;
	DoryIconView *icon_view;

	icon_view = DORY_ICON_VIEW (view);

	if (uri != NULL) {
		/* Only if existing, since we don't want to add the file to
		   the directory if it has been removed since then */
		file = dory_file_get_existing_by_uri (uri);
		if (file != NULL) {
			dory_icon_container_scroll_to_icon (get_icon_container (icon_view),
								DORY_ICON_CONTAINER_ICON_DATA (file));
			dory_file_unref (file);
		}
	}
}

static const char *
dory_icon_view_get_id (DoryView *view)
{
	if (dory_icon_view_is_compact (DORY_ICON_VIEW (view))) {
		return FM_COMPACT_VIEW_ID;
	}

	return DORY_ICON_VIEW_ID;
}

static void
set_compact_view (DoryIconView *icon_view,
                  gboolean      compact)
{
    icon_view->details->compact = compact;

    if (compact) {
        dory_icon_container_set_layout_mode (get_icon_container (icon_view),
                                             gtk_widget_get_direction (GTK_WIDGET(icon_view)) == GTK_TEXT_DIR_RTL ?
                                                                                                     DORY_ICON_LAYOUT_T_B_R_L :
                                                                                                     DORY_ICON_LAYOUT_T_B_L_R);
        dory_icon_container_set_forced_icon_size (get_icon_container (icon_view),
                                                  DORY_COMPACT_FORCED_ICON_SIZE);
    } else {
        dory_icon_container_set_layout_mode (get_icon_container (icon_view),
                                             gtk_widget_get_direction (GTK_WIDGET(icon_view)) == GTK_TEXT_DIR_RTL ?
                                                                                                     DORY_ICON_LAYOUT_R_L_T_B :
                                                                                                     DORY_ICON_LAYOUT_L_R_T_B);
        dory_icon_container_set_forced_icon_size (get_icon_container (icon_view),
                                                  0);
    }
}

static void
dory_icon_view_set_property (GObject         *object,
			   guint            prop_id,
			   const GValue    *value,
			   GParamSpec      *pspec)
{
	DoryIconView *icon_view;

	icon_view = DORY_ICON_VIEW (object);

	switch (prop_id)  {
	case PROP_COMPACT:
        set_compact_view (icon_view, g_value_get_boolean (value));
		break;
	case PROP_SUPPORTS_AUTO_LAYOUT:
		icon_view->details->supports_auto_layout = g_value_get_boolean (value);
		break;
	case PROP_IS_DESKTOP:
		icon_view->details->is_desktop = g_value_get_boolean (value);
		break;
	case PROP_SUPPORTS_KEEP_ALIGNED:
		icon_view->details->supports_keep_aligned = g_value_get_boolean (value);
		break;
	case PROP_SUPPORTS_LABELS_BESIDE_ICONS:
		icon_view->details->supports_labels_beside_icons = g_value_get_boolean (value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
dory_icon_view_finalize (GObject *object)
{
	DoryIconView *icon_view;

	icon_view = DORY_ICON_VIEW (object);

	g_free (icon_view->details);

	g_signal_handlers_disconnect_by_func (dory_preferences,
					      default_sort_order_changed_callback,
					      icon_view);
	g_signal_handlers_disconnect_by_func (dory_preferences,
					      default_sort_in_reverse_order_changed_callback,
					      icon_view);
	g_signal_handlers_disconnect_by_func (dory_preferences,
					      image_display_policy_changed_callback,
					      icon_view);

	g_signal_handlers_disconnect_by_func (dory_icon_view_preferences,
					      default_zoom_level_changed_callback,
					      icon_view);
	g_signal_handlers_disconnect_by_func (dory_icon_view_preferences,
					      labels_beside_icons_changed_callback,
					      icon_view);
	g_signal_handlers_disconnect_by_func (dory_icon_view_preferences,
					      text_attribute_names_changed_callback,
					      icon_view);

	g_signal_handlers_disconnect_by_func (dory_compact_view_preferences,
					      default_zoom_level_changed_callback,
					      icon_view);
	g_signal_handlers_disconnect_by_func (dory_compact_view_preferences,
					      all_columns_same_width_changed_callback,
					      icon_view);

	G_OBJECT_CLASS (dory_icon_view_parent_class)->finalize (object);
}

static void
dory_icon_view_constructed (GObject *object)
{
    DoryIconView *icon_view;
    DoryIconContainer *icon_container;

    icon_view = DORY_ICON_VIEW (object);

    G_OBJECT_CLASS (dory_icon_view_parent_class)->constructed (G_OBJECT (icon_view));

    g_return_if_fail (gtk_bin_get_child (GTK_BIN (icon_view)) == NULL);

    icon_container = create_icon_container (icon_view);

    /* Set our default layout mode */
    if (!icon_view->details->is_desktop) {
        dory_icon_container_set_layout_mode (icon_container,
                                             gtk_widget_get_direction (GTK_WIDGET(icon_container)) == GTK_TEXT_DIR_RTL ?
                                             DORY_ICON_LAYOUT_R_L_T_B :
                                             DORY_ICON_LAYOUT_L_R_T_B);
    }

    g_signal_connect_swapped (dory_preferences,
                  "changed::" DORY_PREFERENCES_DEFAULT_SORT_ORDER,
                  G_CALLBACK (default_sort_order_changed_callback),
                  icon_view);
    g_signal_connect_swapped (dory_preferences,
                  "changed::" DORY_PREFERENCES_DEFAULT_SORT_IN_REVERSE_ORDER,
                  G_CALLBACK (default_sort_in_reverse_order_changed_callback),
                  icon_view);
    g_signal_connect_swapped (dory_preferences,
                  "changed::" DORY_PREFERENCES_SHOW_IMAGE_FILE_THUMBNAILS,
                  G_CALLBACK (image_display_policy_changed_callback),
                  icon_view);
    g_signal_connect_swapped (dory_icon_view_preferences,
                  "changed::" DORY_PREFERENCES_ICON_VIEW_DEFAULT_ZOOM_LEVEL,
                  G_CALLBACK (default_zoom_level_changed_callback),
                  icon_view);
    g_signal_connect_swapped (dory_icon_view_preferences,
                  "changed::" DORY_PREFERENCES_ICON_VIEW_LABELS_BESIDE_ICONS,
                  G_CALLBACK (labels_beside_icons_changed_callback),
                  icon_view);
    g_signal_connect_swapped (dory_icon_view_preferences,
                  "changed::" DORY_PREFERENCES_ICON_VIEW_CAPTIONS,
                  G_CALLBACK (text_attribute_names_changed_callback),
                  icon_view);

    g_signal_connect_swapped (dory_compact_view_preferences,
                  "changed::" DORY_PREFERENCES_COMPACT_VIEW_DEFAULT_ZOOM_LEVEL,
                  G_CALLBACK (default_zoom_level_changed_callback),
                  icon_view);
    g_signal_connect_swapped (dory_compact_view_preferences,
                  "changed::" DORY_PREFERENCES_COMPACT_VIEW_ALL_COLUMNS_SAME_WIDTH,
                  G_CALLBACK (all_columns_same_width_changed_callback),
                  icon_view);

    g_signal_connect_object (get_icon_container (icon_view), "handle_netscape_url",
                 G_CALLBACK (icon_view_handle_netscape_url), icon_view, 0);
    g_signal_connect_object (get_icon_container (icon_view), "handle_uri_list",
                 G_CALLBACK (icon_view_handle_uri_list), icon_view, 0);
    g_signal_connect_object (get_icon_container (icon_view), "handle_text",
                 G_CALLBACK (icon_view_handle_text), icon_view, 0);
    g_signal_connect_object (get_icon_container (icon_view), "handle_raw",
                 G_CALLBACK (icon_view_handle_raw), icon_view, 0);

    icon_view->details->clipboard_handler_id =
        g_signal_connect (dory_clipboard_monitor_get (),
                          "clipboard_info",
                          G_CALLBACK (icon_view_notify_clipboard_info), icon_view);

    dory_icon_container_set_is_desktop (icon_container, FALSE);
}

static const gchar *
dory_icon_view_get_sort_attribute (DoryView *view)
{
	DoryIconView *icon_view = DORY_ICON_VIEW (view);
	return icon_view->details->sort->metadata_text;
}

static void
dory_icon_view_class_init (DoryIconViewClass *klass)
{
	DoryViewClass *dory_view_class;
	GObjectClass *oclass;

    klass->use_grid_container = FALSE;

	dory_view_class = DORY_VIEW_CLASS (klass);
	oclass = G_OBJECT_CLASS (klass);

	oclass->set_property = dory_icon_view_set_property;
	oclass->finalize = dory_icon_view_finalize;
    oclass->constructed = dory_icon_view_constructed;

	GTK_WIDGET_CLASS (klass)->destroy = dory_icon_view_destroy;
	GTK_WIDGET_CLASS (klass)->screen_changed = dory_icon_view_screen_changed;
	GTK_WIDGET_CLASS (klass)->scroll_event = dory_icon_view_scroll_event;

	dory_view_class->add_file = dory_icon_view_add_file;
	dory_view_class->begin_loading = dory_icon_view_begin_loading;
	dory_view_class->bump_zoom_level = dory_icon_view_bump_zoom_level;
	dory_view_class->can_rename_file = dory_icon_view_can_rename_file;
	dory_view_class->can_zoom_in = dory_icon_view_can_zoom_in;
	dory_view_class->can_zoom_out = dory_icon_view_can_zoom_out;
	dory_view_class->clear = dory_icon_view_clear;
	dory_view_class->end_loading = dory_icon_view_end_loading;
	dory_view_class->file_changed = dory_icon_view_file_changed;
	dory_view_class->get_selected_icon_locations = dory_icon_view_get_selected_icon_locations;
    dory_view_class->get_selection = dory_icon_view_get_selection;
    dory_view_class->peek_selection = dory_icon_view_peek_selection;
	dory_view_class->get_selection_count = dory_icon_view_get_selection_count;
	dory_view_class->get_selection_for_file_transfer = dory_icon_view_get_selection;
	dory_view_class->get_item_count = dory_icon_view_get_item_count;
	dory_view_class->is_empty = dory_icon_view_is_empty;
	dory_view_class->remove_file = dory_icon_view_remove_file;
	dory_view_class->reset_to_defaults = dory_icon_view_reset_to_defaults;
	dory_view_class->restore_default_zoom_level = dory_icon_view_restore_default_zoom_level;
    dory_view_class->get_default_zoom_level = dory_icon_view_get_default_zoom_level;
	dory_view_class->reveal_selection = dory_icon_view_reveal_selection;
	dory_view_class->select_all = dory_icon_view_select_all;
	dory_view_class->set_selection = dory_icon_view_set_selection;
	dory_view_class->invert_selection = dory_icon_view_invert_selection;
	dory_view_class->compare_files = compare_files;
	dory_view_class->update_filter_text = dory_icon_view_update_filter_text;
	dory_view_class->select_first = dory_icon_view_select_first;
	dory_view_class->zoom_to_level = dory_icon_view_zoom_to_level;
	dory_view_class->get_zoom_level = dory_icon_view_get_zoom_level;
        dory_view_class->click_policy_changed = dory_icon_view_click_policy_changed;
        dory_view_class->click_to_rename_mode_changed = dory_icon_view_click_to_rename_mode_changed;
        dory_view_class->merge_menus = dory_icon_view_merge_menus;
        dory_view_class->unmerge_menus = dory_icon_view_unmerge_menus;
        dory_view_class->sort_directories_first_changed = dory_icon_view_sort_directories_first_changed;
        dory_view_class->sort_favorites_first_changed = dory_icon_view_sort_favorites_first_changed;
        dory_view_class->start_renaming_file = dory_icon_view_start_renaming_file;
        dory_view_class->update_menus = dory_icon_view_update_menus;
	dory_view_class->using_manual_layout = dory_icon_view_using_manual_layout;
	dory_view_class->widget_to_file_operation_position = dory_icon_view_widget_to_file_operation_position;
	dory_view_class->get_view_id = dory_icon_view_get_id;
	dory_view_class->get_first_visible_file = icon_view_get_first_visible_file;
	dory_view_class->scroll_to_file = icon_view_scroll_to_file;
	dory_view_class->get_sort_attribute = dory_icon_view_get_sort_attribute;

	properties[PROP_COMPACT] =
		g_param_spec_boolean ("compact",
				      "Compact",
				      "Whether this view provides a compact listing",
				      FALSE,
				      G_PARAM_WRITABLE);
	properties[PROP_SUPPORTS_AUTO_LAYOUT] =
		g_param_spec_boolean ("supports-auto-layout",
				      "Supports auto layout",
				      "Whether this view supports auto layout",
				      TRUE,
				      G_PARAM_WRITABLE |
				      G_PARAM_CONSTRUCT_ONLY);
	properties[PROP_IS_DESKTOP] =
		g_param_spec_boolean ("is-desktop",
				      "Is a desktop view",
				      "Whether this view is on a desktop",
				      FALSE,
				      G_PARAM_WRITABLE |
				      G_PARAM_CONSTRUCT_ONLY);
	properties[PROP_SUPPORTS_KEEP_ALIGNED] =
		g_param_spec_boolean ("supports-keep-aligned",
				      "Supports keep aligned",
				      "Whether this view supports keep aligned",
				      FALSE,
				      G_PARAM_WRITABLE |
				      G_PARAM_CONSTRUCT_ONLY);
	properties[PROP_SUPPORTS_LABELS_BESIDE_ICONS] =
		g_param_spec_boolean ("supports-labels-beside-icons",
				      "Supports labels beside icons",
				      "Whether this view supports labels beside icons",
				      TRUE,
				      G_PARAM_WRITABLE |
				      G_PARAM_CONSTRUCT_ONLY);

	g_object_class_install_properties (oclass, NUM_PROPERTIES, properties);
}

static void
dory_icon_view_init (DoryIconView *icon_view)
{
    icon_view->details = g_new0 (DoryIconViewDetails, 1);
    icon_view->details->sort = &sort_criteria[0];
}

static DoryView *
dory_icon_view_create (DoryWindowSlot *slot)
{
	DoryIconView *view;

	view = g_object_new (DORY_TYPE_ICON_VIEW,
			     "window-slot", slot,
			     NULL);
#if GTK_CHECK_VERSION (3, 20, 0)
	gtk_style_context_add_class (gtk_widget_get_style_context (GTK_WIDGET(view)), GTK_STYLE_CLASS_VIEW);
#endif

    set_compact_view (view, FALSE);

	return DORY_VIEW (view);
}

static DoryView *
dory_compact_view_create (DoryWindowSlot *slot)
{
	DoryIconView *view;

	view = g_object_new (DORY_TYPE_ICON_VIEW,
			     "window-slot", slot,
			     NULL);
#if GTK_CHECK_VERSION (3, 20, 0)
	gtk_style_context_add_class (gtk_widget_get_style_context (GTK_WIDGET(view)), GTK_STYLE_CLASS_VIEW);
#endif

    set_compact_view (view, TRUE);

    return DORY_VIEW (view);
}

static gboolean
dory_icon_view_supports_uri (const char *uri,
			   GFileType file_type,
			   const char *mime_type)
{
	if (file_type == G_FILE_TYPE_DIRECTORY) {
		return TRUE;
	}
	if (g_str_has_prefix (uri, "trash:")) {
		return TRUE;
	}
	if (g_str_has_prefix (uri, EEL_SEARCH_URI)) {
		return TRUE;
	}

	return FALSE;
}

#define TRANSLATE_VIEW_INFO(view_info)					\
	view_info.view_combo_label = _(view_info.view_combo_label);	\
	view_info.view_menu_label_with_mnemonic = _(view_info.view_menu_label_with_mnemonic); \
	view_info.error_label = _(view_info.error_label);		\
	view_info.startup_error_label = _(view_info.startup_error_label); \
	view_info.display_location_label = _(view_info.display_location_label); \


static DoryViewInfo dory_icon_view = {
	(char *)DORY_ICON_VIEW_ID,
	/* translators: this is used in the view selection dropdown
	 * of navigation windows and in the preferences dialog */
	(char *)N_("Icon View"),
	/* translators: this is used in the view menu */
	(char *)N_("_Icons"),
	(char *)N_("The icon view encountered an error."),
	(char *)N_("The icon view encountered an error while starting up."),
	(char *)N_("Display this location with the icon view."),
	dory_icon_view_create,
	dory_icon_view_supports_uri
};

static DoryViewInfo dory_compact_view = {
	(char *)FM_COMPACT_VIEW_ID,
	/* translators: this is used in the view selection dropdown
	 * of navigation windows and in the preferences dialog */
	(char *)N_("Compact View"),
	/* translators: this is used in the view menu */
	(char *)N_("_Compact"),
	(char *)N_("The compact view encountered an error."),
	(char *)N_("The compact view encountered an error while starting up."),
	(char *)N_("Display this location with the compact view."),
	dory_compact_view_create,
	dory_icon_view_supports_uri
};

gboolean
dory_icon_view_is_compact (DoryIconView *view)
{
	return view->details->compact;
}

void
dory_icon_view_register (void)
{
	TRANSLATE_VIEW_INFO (dory_icon_view)
		dory_view_factory_register (&dory_icon_view);
}

void
dory_icon_view_compact_register (void)
{
	TRANSLATE_VIEW_INFO (dory_compact_view)
		dory_view_factory_register (&dory_compact_view);
}

