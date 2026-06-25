/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* fm-empty-view.c - implementation of empty view of directory.

   Copyright (C) 2006 Free Software Foundation, Inc.
   
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

   Authors: Christian Neumair <chris@gnome-de.org>
*/

#include <config.h>

#include "dory-empty-view.h"

#include "dory-view.h"
#include "dory-view-factory.h"

#include <string.h>
#include <libdory-private/dory-file-utilities.h>
#include <eel/eel-vfs-extensions.h>

struct DoryEmptyViewDetails {
	int number_of_files;
};

static GList *dory_empty_view_get_selection                   (DoryView   *view);
static GList *dory_empty_view_get_selection_for_file_transfer (DoryView   *view);
static void   dory_empty_view_scroll_to_file                  (DoryView      *view,
								   const char        *uri);

G_DEFINE_TYPE (DoryEmptyView, dory_empty_view, DORY_TYPE_VIEW)

static void
dory_empty_view_add_file (DoryView *view, DoryFile *file, DoryDirectory *directory)
{
	static GTimer *timer = NULL;
	static gdouble cumu = 0, elaps;
	DORY_EMPTY_VIEW (view)->details->number_of_files++;
	GdkPixbuf *icon;

	if (!timer) timer = g_timer_new ();

	g_timer_start (timer);
	icon = dory_file_get_icon_pixbuf (file, dory_get_icon_size_for_zoom_level (DORY_ZOOM_LEVEL_STANDARD), TRUE, 0, DORY_FILE_ICON_FLAGS_NONE);

	elaps = g_timer_elapsed (timer, NULL);
	g_timer_stop (timer);

	g_object_unref (icon);
	
	cumu += elaps;
	g_message ("entire loading: %.3f, cumulative %.3f", elaps, cumu);
}


static void
dory_empty_view_begin_loading (DoryView *view)
{
}

static void
dory_empty_view_clear (DoryView *view)
{
}


static void
dory_empty_view_file_changed (DoryView *view, DoryFile *file, DoryDirectory *directory)
{
}

static GList *
dory_empty_view_get_selection (DoryView *view)
{
	return NULL;
}


static GList *
dory_empty_view_get_selection_for_file_transfer (DoryView *view)
{
	return NULL;
}

static guint
dory_empty_view_get_item_count (DoryView *view)
{
	return DORY_EMPTY_VIEW (view)->details->number_of_files;
}

static gboolean
dory_empty_view_is_empty (DoryView *view)
{
	return DORY_EMPTY_VIEW (view)->details->number_of_files == 0;
}

static void
dory_empty_view_end_file_changes (DoryView *view)
{
}

static void
dory_empty_view_remove_file (DoryView *view, DoryFile *file, DoryDirectory *directory)
{
	DORY_EMPTY_VIEW (view)->details->number_of_files--;
	g_assert (DORY_EMPTY_VIEW (view)->details->number_of_files >= 0);
}

static void
dory_empty_view_set_selection (DoryView *view, GList *selection)
{
	dory_view_notify_selection_changed (view);
}

static void
dory_empty_view_select_all (DoryView *view)
{
}

static void
dory_empty_view_reveal_selection (DoryView *view)
{
}

static void
dory_empty_view_merge_menus (DoryView *view)
{
	DORY_VIEW_CLASS (dory_empty_view_parent_class)->merge_menus (view);
}

static void
dory_empty_view_update_menus (DoryView *view)
{
	DORY_VIEW_CLASS (dory_empty_view_parent_class)->update_menus (view);
}

/* Reset sort criteria and zoom level to match defaults */
static void
dory_empty_view_reset_to_defaults (DoryView *view)
{
}

static void
dory_empty_view_bump_zoom_level (DoryView *view, int zoom_increment)
{
}

static DoryZoomLevel
dory_empty_view_get_zoom_level (DoryView *view)
{
	return DORY_ZOOM_LEVEL_STANDARD;
}

static void
dory_empty_view_zoom_to_level (DoryView *view,
			    DoryZoomLevel zoom_level)
{
}

static void
dory_empty_view_restore_default_zoom_level (DoryView *view)
{
}

static gboolean 
dory_empty_view_can_zoom_in (DoryView *view) 
{
	return FALSE;
}

static gboolean 
dory_empty_view_can_zoom_out (DoryView *view) 
{
	return FALSE;
}

static void
dory_empty_view_start_renaming_file (DoryView *view,
				  DoryFile *file,
				  gboolean select_all)
{
}

static void
dory_empty_view_click_policy_changed (DoryView *directory_view)
{
}


static int
dory_empty_view_compare_files (DoryView *view, DoryFile *file1, DoryFile *file2)
{
	if (file1 < file2) {
		return -1;
	}

	if (file1 > file2) {
		return +1;
	}

	return 0;
}

static gboolean
dory_empty_view_using_manual_layout (DoryView *view)
{
	return FALSE;
}

static void
dory_empty_view_end_loading (DoryView *view,
			   gboolean all_files_seen)
{
}

static char *
dory_empty_view_get_first_visible_file (DoryView *view)
{
	return NULL;
}

static void
dory_empty_view_scroll_to_file (DoryView *view,
			      const char *uri)
{
}

static void
dory_empty_view_sort_directories_first_changed (DoryView *view)
{
}

static const char *
dory_empty_view_get_id (DoryView *view)
{
	return DORY_EMPTY_VIEW_ID;
}

static void
dory_empty_view_class_init (DoryEmptyViewClass *class)
{
	DoryViewClass *dory_view_class;

	g_type_class_add_private (class, sizeof (DoryEmptyViewDetails));

	dory_view_class = DORY_VIEW_CLASS (class);

	dory_view_class->add_file = dory_empty_view_add_file;
	dory_view_class->begin_loading = dory_empty_view_begin_loading;
	dory_view_class->bump_zoom_level = dory_empty_view_bump_zoom_level;
	dory_view_class->can_zoom_in = dory_empty_view_can_zoom_in;
	dory_view_class->can_zoom_out = dory_empty_view_can_zoom_out;
        dory_view_class->click_policy_changed = dory_empty_view_click_policy_changed;
	dory_view_class->clear = dory_empty_view_clear;
	dory_view_class->file_changed = dory_empty_view_file_changed;
	dory_view_class->get_selection = dory_empty_view_get_selection;
	dory_view_class->get_selection_for_file_transfer = dory_empty_view_get_selection_for_file_transfer;
	dory_view_class->get_item_count = dory_empty_view_get_item_count;
	dory_view_class->is_empty = dory_empty_view_is_empty;
	dory_view_class->remove_file = dory_empty_view_remove_file;
	dory_view_class->merge_menus = dory_empty_view_merge_menus;
	dory_view_class->update_menus = dory_empty_view_update_menus;
	dory_view_class->reset_to_defaults = dory_empty_view_reset_to_defaults;
	dory_view_class->restore_default_zoom_level = dory_empty_view_restore_default_zoom_level;
	dory_view_class->reveal_selection = dory_empty_view_reveal_selection;
	dory_view_class->select_all = dory_empty_view_select_all;
	dory_view_class->set_selection = dory_empty_view_set_selection;
	dory_view_class->compare_files = dory_empty_view_compare_files;
	dory_view_class->sort_directories_first_changed = dory_empty_view_sort_directories_first_changed;
	dory_view_class->start_renaming_file = dory_empty_view_start_renaming_file;
	dory_view_class->get_zoom_level = dory_empty_view_get_zoom_level;
	dory_view_class->zoom_to_level = dory_empty_view_zoom_to_level;
	dory_view_class->end_file_changes = dory_empty_view_end_file_changes;
	dory_view_class->using_manual_layout = dory_empty_view_using_manual_layout;
	dory_view_class->end_loading = dory_empty_view_end_loading;
	dory_view_class->get_view_id = dory_empty_view_get_id;
	dory_view_class->get_first_visible_file = dory_empty_view_get_first_visible_file;
	dory_view_class->scroll_to_file = dory_empty_view_scroll_to_file;
}

static void
dory_empty_view_init (DoryEmptyView *empty_view)
{
	empty_view->details = G_TYPE_INSTANCE_GET_PRIVATE (empty_view, DORY_TYPE_EMPTY_VIEW,
							   DoryEmptyViewDetails);
}

static DoryView *
dory_empty_view_create (DoryWindowSlot *slot)
{
	DoryEmptyView *view;

	g_assert (DORY_IS_WINDOW_SLOT (slot));

	view = g_object_new (DORY_TYPE_EMPTY_VIEW,
			     "window-slot", slot,
			     NULL);

	return DORY_VIEW (view);
}

static gboolean
dory_empty_view_supports_uri (const char *uri,
				  GFileType file_type,
				  const char *mime_type)
{
	if (file_type == G_FILE_TYPE_DIRECTORY) {
		return TRUE;
	}
	if (strcmp (mime_type, DORY_SAVED_SEARCH_MIMETYPE) == 0){
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

static DoryViewInfo dory_empty_view = {
	DORY_EMPTY_VIEW_ID,
	"Empty",
	"Empty View",
	"_Empty View",
	"The empty view encountered an error.",
	"Display this location with the empty view.",
	dory_empty_view_create,
	dory_empty_view_supports_uri
};

void
dory_empty_view_register (void)
{
	dory_empty_view.id = dory_empty_view.id;
	dory_empty_view.view_combo_label = dory_empty_view.view_combo_label;
	dory_empty_view.view_menu_label_with_mnemonic = dory_empty_view.view_menu_label_with_mnemonic;
	dory_empty_view.error_label = dory_empty_view.error_label;
	dory_empty_view.display_location_label = dory_empty_view.display_location_label;

	dory_view_factory_register (&dory_empty_view);
}
