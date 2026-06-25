/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* fm-list-model.h - a GtkTreeModel for file lists. 

   Copyright (C) 2001, 2002 Anders Carlsson

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

   Authors: Anders Carlsson <andersca@gnu.org>
*/

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <libdory-private/dory-file.h>
#include <libdory-private/dory-directory.h>
#include <libdory-extension/dory-column.h>

#ifndef DORY_LIST_MODEL_H
#define DORY_LIST_MODEL_H

#define DORY_TYPE_LIST_MODEL dory_list_model_get_type()
#define DORY_LIST_MODEL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_LIST_MODEL, DoryListModel))
#define DORY_LIST_MODEL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_LIST_MODEL, DoryListModelClass))
#define DORY_IS_LIST_MODEL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_LIST_MODEL))
#define DORY_IS_LIST_MODEL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_LIST_MODEL))
#define DORY_LIST_MODEL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_LIST_MODEL, DoryListModelClass))

enum {
	DORY_LIST_MODEL_FILE_COLUMN,
	DORY_LIST_MODEL_SUBDIRECTORY_COLUMN,
	DORY_LIST_MODEL_SMALLEST_ICON_COLUMN,
	DORY_LIST_MODEL_SMALLER_ICON_COLUMN,
	DORY_LIST_MODEL_SMALL_ICON_COLUMN,
	DORY_LIST_MODEL_STANDARD_ICON_COLUMN,
	DORY_LIST_MODEL_LARGE_ICON_COLUMN,
	DORY_LIST_MODEL_LARGER_ICON_COLUMN,
	DORY_LIST_MODEL_LARGEST_ICON_COLUMN,
	DORY_LIST_MODEL_FILE_NAME_IS_EDITABLE_COLUMN,
    DORY_LIST_MODEL_TEXT_WEIGHT_COLUMN,
    DORY_LIST_MODEL_ICON_SHOWN,
	DORY_LIST_MODEL_NUM_COLUMNS
};

typedef struct DoryListModelDetails DoryListModelDetails;

typedef struct DoryListModel {
	GObject parent_instance;
	DoryListModelDetails *details;
} DoryListModel;

typedef struct {
	GObjectClass parent_class;

	void (* subdirectory_unloaded)(DoryListModel *model,
				       DoryDirectory *subdirectory);
} DoryListModelClass;

GType    dory_list_model_get_type                          (void);
gboolean dory_list_model_add_file                          (DoryListModel          *model,
								DoryFile         *file,
								DoryDirectory    *directory);
void     dory_list_model_file_changed                      (DoryListModel          *model,
								DoryFile         *file,
								DoryDirectory    *directory);
gboolean dory_list_model_is_empty                          (DoryListModel          *model);
guint    dory_list_model_get_length                        (DoryListModel          *model);
void     dory_list_model_remove_file                       (DoryListModel          *model,
								DoryFile         *file,
								DoryDirectory    *directory);
void     dory_list_model_clear                             (DoryListModel          *model);
gboolean dory_list_model_get_tree_iter_from_file           (DoryListModel          *model,
								DoryFile         *file,
								DoryDirectory    *directory,
								GtkTreeIter          *iter);
GList *  dory_list_model_get_all_iters_for_file            (DoryListModel          *model,
								DoryFile         *file);
gboolean dory_list_model_get_first_iter_for_file           (DoryListModel          *model,
								DoryFile         *file,
								GtkTreeIter          *iter);
void     dory_list_model_set_should_sort_directories_first (DoryListModel          *model,
								gboolean              sort_directories_first);
void     dory_list_model_set_should_sort_favorites_first (DoryListModel          *model,
								gboolean              sort_favorites_first);
int      dory_list_model_get_sort_column_id_from_attribute (DoryListModel *model,
								GQuark       attribute);
GQuark   dory_list_model_get_attribute_from_sort_column_id (DoryListModel *model,
								int sort_column_id);
void     dory_list_model_sort_files                        (DoryListModel *model,
								GList **files);

DoryZoomLevel dory_list_model_get_zoom_level_from_column_id (int               column);
int               dory_list_model_get_column_id_from_zoom_level (DoryZoomLevel zoom_level);

DoryFile *    dory_list_model_file_for_path (DoryListModel *model, GtkTreePath *path);
gboolean          dory_list_model_load_subdirectory (DoryListModel *model, GtkTreePath *path, DoryDirectory **directory);
void              dory_list_model_unload_subdirectory (DoryListModel *model, GtkTreeIter *iter);

void              dory_list_model_set_drag_view (DoryListModel *model,
						     GtkTreeView *view,
						     int begin_x, 
						     int begin_y);

GtkTargetList *   dory_list_model_get_drag_target_list (void);

int               dory_list_model_compare_func (DoryListModel *model,
						    DoryFile *file1,
						    DoryFile *file2);


int               dory_list_model_add_column (DoryListModel *model,
						  DoryColumn *column);
int               dory_list_model_get_column_number (DoryListModel *model,
							 const char *column_name);

void              dory_list_model_subdirectory_done_loading (DoryListModel       *model,
								 DoryDirectory *directory);

void              dory_list_model_set_highlight_for_files (DoryListModel *model,
							       GList *files);

void              dory_list_model_set_temporarily_disable_sort (DoryListModel *model, gboolean disable);
gboolean          dory_list_model_get_temporarily_disable_sort (DoryListModel *model);
void              dory_list_model_set_expanding                (DoryListModel *model, DoryDirectory *directory);
void              dory_list_model_set_view_directory           (DoryListModel *model, DoryDirectory *dir);
void              dory_list_model_set_expansion_enabled        (DoryListModel *model, gboolean enabled);

void              dory_list_model_set_filter_active           (DoryListModel *model,
                                                               gboolean       active);
gboolean          dory_list_model_get_filter_active           (DoryListModel *model);

#endif /* DORY_LIST_MODEL_H */
