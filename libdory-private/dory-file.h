/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   dory-file.h: Dory file model.
 
   Copyright (C) 1999, 2000, 2001 Eazel, Inc.
  
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
  
   Author: Darin Adler <darin@bentspoon.com>
*/

#ifndef DORY_FILE_H
#define DORY_FILE_H

#include <gtk/gtk.h>
#include <gio/gio.h>
#include <pango/pango.h>
#include <libdory-private/dory-file-attributes.h>
#include <libdory-private/dory-icon-info.h>
#include <libdory-private/dory-search-engine.h>

/* DoryFile is an object used to represent a single element of a
 * DoryDirectory. It's lightweight and relies on DoryDirectory
 * to do most of the work.
 */

/* DoryFile is defined both here and in dory-directory.h. */
#ifndef DORY_FILE_DEFINED
#define DORY_FILE_DEFINED
typedef struct DoryFile DoryFile;
#endif

#define DORY_TYPE_FILE dory_file_get_type()
#define DORY_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_FILE, DoryFile))
#define DORY_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_FILE, DoryFileClass))
#define DORY_IS_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_FILE))
#define DORY_IS_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_FILE))
#define DORY_FILE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_FILE, DoryFileClass))

typedef enum {
	DORY_FILE_SORT_NONE,
	DORY_FILE_SORT_BY_DISPLAY_NAME,
	DORY_FILE_SORT_BY_SIZE,
	DORY_FILE_SORT_BY_TYPE,
    DORY_FILE_SORT_BY_DETAILED_TYPE,
	DORY_FILE_SORT_BY_MTIME,
    DORY_FILE_SORT_BY_ATIME,
	DORY_FILE_SORT_BY_TRASHED_TIME,
	DORY_FILE_SORT_BY_BTIME,
	DORY_FILE_SORT_BY_SEARCH_RESULT_COUNT,
	DORY_FILE_SORT_BY_EXTENSION
} DoryFileSortType;
typedef enum {
	DORY_REQUEST_NOT_STARTED,
	DORY_REQUEST_IN_PROGRESS,
	DORY_REQUEST_DONE
} DoryRequestStatus;

typedef enum {
	DORY_FILE_ICON_FLAGS_NONE = 0,
	DORY_FILE_ICON_FLAGS_USE_THUMBNAILS = (1<<0),
	DORY_FILE_ICON_FLAGS_IGNORE_VISITING = (1<<1),
	DORY_FILE_ICON_FLAGS_EMBEDDING_TEXT = (1<<2),
	DORY_FILE_ICON_FLAGS_FOR_DRAG_ACCEPT = (1<<3),
	DORY_FILE_ICON_FLAGS_FOR_OPEN_FOLDER = (1<<4),
	/* whether the thumbnail size must match the display icon size */
	DORY_FILE_ICON_FLAGS_FORCE_THUMBNAIL_SIZE = (1<<5),
	/* uses the icon of the mount if present */
	DORY_FILE_ICON_FLAGS_USE_MOUNT_ICON = (1<<6),
	/* render the mount icon as an emblem over the regular one */
	DORY_FILE_ICON_FLAGS_USE_MOUNT_ICON_AS_EMBLEM = (1<<7),
    DORY_FILE_ICON_FLAGS_PIN_HEIGHT_FOR_DESKTOP = (1<<8)
} DoryFileIconFlags;	

typedef enum {
    DORY_DATE_TYPE_MODIFIED,
    DORY_DATE_TYPE_CHANGED,
    DORY_DATE_TYPE_ACCESSED,
    DORY_DATE_TYPE_PERMISSIONS_CHANGED,
    DORY_DATE_TYPE_TRASHED,
    DORY_DATE_TYPE_CREATED
} DoryDateType;

typedef enum {
    DORY_FILE_TOOLTIP_FLAGS_NONE = 0,
    DORY_FILE_TOOLTIP_FLAGS_FILE_TYPE =  (1<<0),
    DORY_FILE_TOOLTIP_FLAGS_MOD_DATE = (1<<1),
    DORY_FILE_TOOLTIP_FLAGS_ACCESS_DATE = (1<<2),
    DORY_FILE_TOOLTIP_FLAGS_PATH = (1<<3),
    DORY_FILE_TOOLTIP_FLAGS_CREATED_DATE = (1<<4)
} DoryFileTooltipFlags;

typedef enum {
    DORY_FILE_LOAD_DEFERRED_ATTRS_NO,
    DORY_FILE_LOAD_DEFERRED_ATTRS_YES,
    DORY_FILE_LOAD_DEFERRED_ATTRS_PRELOAD
} DoryFileLoadDeferredAttrs;

/* Emblems sometimes displayed for DoryFiles. Do not localize. */ 
#define DORY_FILE_EMBLEM_NAME_SYMBOLIC_LINK "symbolic-link"
#define DORY_FILE_EMBLEM_NAME_CANT_READ "unreadable"
#define DORY_FILE_EMBLEM_NAME_CANT_WRITE "readonly"
#define DORY_FILE_EMBLEM_NAME_TRASH "trash"
#define DORY_FILE_EMBLEM_NAME_NOTE "note"
#define DORY_FILE_EMBLEM_NAME_FAVORITE "xapp-favorite"

typedef void (*DoryFileCallback)          (DoryFile  *file,
				               gpointer       callback_data);
typedef void (*DoryFileListCallback)      (GList         *file_list,
				               gpointer       callback_data);
typedef void (*DoryFileOperationCallback) (DoryFile  *file,
					       GFile         *result_location,
					       GError        *error,
					       gpointer       callback_data);
typedef int (*DoryWidthMeasureCallback)   (const char    *string,
					       void	     *context);
typedef char * (*DoryTruncateCallback)    (const char    *string,
					       int	      width,
					       void	     *context);

#define DORY_FILE_ATTRIBUTES_FOR_ICON (DORY_FILE_ATTRIBUTE_INFO | DORY_FILE_ATTRIBUTE_LINK_INFO | DORY_FILE_ATTRIBUTE_THUMBNAIL)
#define DORY_FILE_DEFERRED_ATTRIBUTES (DORY_FILE_ATTRIBUTE_THUMBNAIL | DORY_FILE_ATTRIBUTE_EXTENSION_INFO)

typedef void DoryFileListHandle;

/* GObject requirements. */
GType                   dory_file_get_type                          (void);

/* Getting at a single file. */
DoryFile *          dory_file_get                               (GFile                          *location);
DoryFile *          dory_file_get_by_uri                        (const char                     *uri);

/* Get a file only if the dory version already exists */
DoryFile *          dory_file_get_existing                      (GFile                          *location);
DoryFile *          dory_file_get_existing_by_uri               (const char                     *uri);

/* Covers for g_object_ref and g_object_unref that provide two conveniences:
 * 1) Using these is type safe.
 * 2) You are allowed to call these with NULL,
 */
DoryFile *          dory_file_ref                               (DoryFile                   *file);
void                    dory_file_unref                             (DoryFile                   *file);

/* Monitor the file. */
void                    dory_file_monitor_add                       (DoryFile                   *file,
									 gconstpointer                   client,
									 DoryFileAttributes          attributes);
void                    dory_file_monitor_remove                    (DoryFile                   *file,
									 gconstpointer                   client);

/* Waiting for data that's read asynchronously.
 * This interface currently works only for metadata, but could be expanded
 * to other attributes as well.
 */
void                    dory_file_call_when_ready                   (DoryFile                   *file,
									 DoryFileAttributes          attributes,
									 DoryFileCallback            callback,
									 gpointer                        callback_data);
void                    dory_file_cancel_call_when_ready            (DoryFile                   *file,
									 DoryFileCallback            callback,
									 gpointer                        callback_data);
gboolean                dory_file_check_if_ready                    (DoryFile                   *file,
									 DoryFileAttributes          attributes);
void                    dory_file_invalidate_attributes             (DoryFile                   *file,
									 DoryFileAttributes          attributes);
void                    dory_file_invalidate_all_attributes         (DoryFile                   *file);

void                    dory_file_increment_thumbnail_try_count     (DoryFile                   *file);

/* Basic attributes for file objects. */
gboolean                dory_file_contains_text                     (DoryFile                   *file);
char *                  dory_file_get_display_name                  (DoryFile                   *file);
char *                  dory_file_get_edit_name                     (DoryFile                   *file);
char *                  dory_file_get_name                          (DoryFile                   *file);
const char *            dory_file_peek_name                         (DoryFile                   *file);

GFile *                 dory_file_get_location                      (DoryFile                   *file);
char *			 dory_file_get_description			 (DoryFile			 *file);
char *                  dory_file_get_uri                           (DoryFile                   *file);
char *                  dory_file_get_local_uri                     (DoryFile                   *file);
char *                  dory_file_get_path                          (DoryFile                   *file);
char *                  dory_file_get_uri_scheme                    (DoryFile                   *file);
gboolean                dory_file_has_uri_scheme                    (DoryFile *file, const gchar *scheme);
DoryFile *          dory_file_get_parent                        (DoryFile                   *file);
GFile *                 dory_file_get_parent_location               (DoryFile                   *file);
char *                  dory_file_get_parent_uri                    (DoryFile                   *file);
char *                  dory_file_get_parent_uri_for_display        (DoryFile                   *file);
gboolean                dory_file_can_get_size                      (DoryFile                   *file);
goffset                 dory_file_get_size                          (DoryFile                   *file);
time_t                  dory_file_get_mtime                         (DoryFile                   *file);
time_t                  dory_file_get_ctime                         (DoryFile                   *file);
GFileType               dory_file_get_file_type                     (DoryFile                   *file);
char *                  dory_file_get_mime_type                     (DoryFile                   *file);
gboolean                dory_file_is_mime_type                      (DoryFile                   *file,
									 const char                     *mime_type);
gboolean                dory_file_is_launchable                     (DoryFile                   *file);
gboolean                dory_file_is_symbolic_link                  (DoryFile                   *file);
gboolean                dory_file_is_mountpoint                     (DoryFile                   *file);
GMount *                dory_file_get_mount                         (DoryFile                   *file);
void                    dory_file_set_mount                         (DoryFile                   *file,
                                                                     GMount                     *mount);
char *                  dory_file_get_volume_free_space             (DoryFile                   *file);
char *                  dory_file_get_volume_name                   (DoryFile                   *file);
char *                  dory_file_get_symbolic_link_target_path     (DoryFile                   *file);
char *                  dory_file_get_symbolic_link_target_uri      (DoryFile                   *file);
gboolean                dory_file_is_broken_symbolic_link           (DoryFile                   *file);
gboolean                dory_file_is_dory_link                  (DoryFile                   *file);
gboolean                dory_file_is_executable                     (DoryFile                   *file);
gboolean                dory_file_is_directory                      (DoryFile                   *file);
gboolean                dory_file_is_user_special_directory         (DoryFile                   *file,
									 GUserDirectory                 special_directory);
gboolean		dory_file_is_archive			(DoryFile			*file);
gboolean                dory_file_is_in_trash                       (DoryFile                   *file);
gboolean                dory_file_is_in_recent                      (DoryFile                   *file);
gboolean                dory_file_is_in_favorites                   (DoryFile                   *file);
gboolean                dory_file_is_in_search                      (DoryFile                   *file);
gboolean                dory_file_is_unavailable_favorite           (DoryFile                   *file);
gboolean                dory_file_is_in_admin                       (DoryFile                   *file);
gboolean                dory_file_is_in_desktop                     (DoryFile                   *file);
gboolean		dory_file_is_home				(DoryFile                   *file);
gboolean                dory_file_is_desktop_directory              (DoryFile                   *file);
GError *                dory_file_get_file_info_error               (DoryFile                   *file);
gboolean                dory_file_get_directory_item_count          (DoryFile                   *file,
									 guint                          *count,
									 gboolean                       *count_unreadable);
void                    dory_file_recompute_deep_counts             (DoryFile                   *file);
DoryRequestStatus   dory_file_get_deep_counts                   (DoryFile                   *file,
									 guint                          *directory_count,
									 guint                          *file_count,
									 guint                          *unreadable_directory_count,
                                     guint                          *hidden_count,
									 goffset                         *total_size,
									 gboolean                        force);
gboolean                dory_file_should_show_thumbnail             (DoryFile                   *file);
void                    dory_file_delete_thumbnail                  (DoryFile                   *file);
gboolean                dory_file_has_loaded_thumbnail              (DoryFile                   *file);
gboolean                dory_file_should_show_directory_item_count  (DoryFile                   *file);
gboolean                dory_file_should_show_type                  (DoryFile                   *file);
GList *                 dory_file_get_keywords                      (DoryFile                   *file);
GList *                 dory_file_get_emblem_icons                  (DoryFile                   *file,
                                                                     DoryFile                   *view_file);
gboolean                dory_file_get_directory_item_mime_types     (DoryFile                   *file,
									 GList                         **mime_list);

void                    dory_file_set_attributes                    (DoryFile                   *file, 
									 GFileInfo                      *attributes,
									 DoryFileOperationCallback   callback,
									 gpointer                        callback_data);
GFilesystemPreviewType  dory_file_get_filesystem_use_preview        (DoryFile *file);

char *                  dory_file_get_filesystem_id                 (DoryFile                   *file);

DoryFile *          dory_file_get_trash_original_file           (DoryFile                   *file);

/* Permissions. */
gboolean                dory_file_can_get_permissions               (DoryFile                   *file);
gboolean                dory_file_can_set_permissions               (DoryFile                   *file);
guint                   dory_file_get_permissions                   (DoryFile                   *file);
gboolean                dory_file_can_get_owner                     (DoryFile                   *file);
gboolean                dory_file_can_set_owner                     (DoryFile                   *file);
gboolean                dory_file_can_get_group                     (DoryFile                   *file);
gboolean                dory_file_can_set_group                     (DoryFile                   *file);
char *                  dory_file_get_owner_name                    (DoryFile                   *file);
char *                  dory_file_get_group_name                    (DoryFile                   *file);
GList *                 dory_get_user_names                         (void);
GList *                 dory_get_all_group_names                    (void);
GList *                 dory_file_get_settable_group_names          (DoryFile                   *file);
gboolean                dory_file_can_get_selinux_context           (DoryFile                   *file);
char *                  dory_file_get_selinux_context               (DoryFile                   *file);

/* "Capabilities". */
gboolean                dory_file_can_read                          (DoryFile                   *file);
gboolean                dory_file_can_write                         (DoryFile                   *file);
gboolean                dory_file_can_execute                       (DoryFile                   *file);
gboolean                dory_file_can_rename                        (DoryFile                   *file);
gboolean                dory_file_can_delete                        (DoryFile                   *file);
gboolean                dory_file_can_trash                         (DoryFile                   *file);

gboolean                dory_file_can_mount                         (DoryFile                   *file);
gboolean                dory_file_can_unmount                       (DoryFile                   *file);
gboolean                dory_file_can_eject                         (DoryFile                   *file);
gboolean                dory_file_can_start                         (DoryFile                   *file);
gboolean                dory_file_can_start_degraded                (DoryFile                   *file);
gboolean                dory_file_can_stop                          (DoryFile                   *file);
GDriveStartStopType     dory_file_get_start_stop_type               (DoryFile                   *file);
gboolean                dory_file_can_poll_for_media                (DoryFile                   *file);
gboolean                dory_file_is_media_check_automatic          (DoryFile                   *file);

void                    dory_file_mount                             (DoryFile                   *file,
									 GMountOperation                *mount_op,
									 GCancellable                   *cancellable,
									 DoryFileOperationCallback   callback,
									 gpointer                        callback_data);
void                    dory_file_unmount                           (DoryFile                   *file,
									 GMountOperation                *mount_op,
									 GCancellable                   *cancellable,
									 DoryFileOperationCallback   callback,
									 gpointer                        callback_data);
void                    dory_file_eject                             (DoryFile                   *file,
									 GMountOperation                *mount_op,
									 GCancellable                   *cancellable,
									 DoryFileOperationCallback   callback,
									 gpointer                        callback_data);

void                    dory_file_start                             (DoryFile                   *file,
									 GMountOperation                *start_op,
									 GCancellable                   *cancellable,
									 DoryFileOperationCallback   callback,
									 gpointer                        callback_data);
void                    dory_file_stop                              (DoryFile                   *file,
									 GMountOperation                *mount_op,
									 GCancellable                   *cancellable,
									 DoryFileOperationCallback   callback,
									 gpointer                        callback_data);
void                    dory_file_poll_for_media                    (DoryFile                   *file);

/* Basic operations for file objects. */
void                    dory_file_set_owner                         (DoryFile                   *file,
									 const char                     *user_name_or_id,
									 DoryFileOperationCallback   callback,
									 gpointer                        callback_data);
void                    dory_file_set_group                         (DoryFile                   *file,
									 const char                     *group_name_or_id,
									 DoryFileOperationCallback   callback,
									 gpointer                        callback_data);
void                    dory_file_set_permissions                   (DoryFile                   *file,
									 guint32                         permissions,
									 DoryFileOperationCallback   callback,
									 gpointer                        callback_data);
void                    dory_file_rename                            (DoryFile                   *file,
									 const char                     *new_name,
									 DoryFileOperationCallback   callback,
									 gpointer                        callback_data);
void                    dory_file_cancel                            (DoryFile                   *file,
									 DoryFileOperationCallback   callback,
									 gpointer                        callback_data);

/* Return true if this file has already been deleted.
 * This object will be unref'd after sending the files_removed signal,
 * but it could hang around longer if someone ref'd it.
 */
gboolean                dory_file_is_gone                           (DoryFile                   *file);

/* Return true if this file is not confirmed to have ever really
 * existed. This is true when the DoryFile object has been created, but no I/O
 * has yet confirmed the existence of a file by that name.
 */
gboolean                dory_file_is_not_yet_confirmed              (DoryFile                   *file);

/* Simple getting and setting top-level metadata. */
char *                  dory_file_get_metadata                      (DoryFile                   *file,
									 const char                     *key,
									 const char                     *default_metadata);
GList *                 dory_file_get_metadata_list                 (DoryFile                   *file,
									 const char                     *key);
void                    dory_file_set_metadata                      (DoryFile                   *file,
									 const char                     *key,
									 const char                     *default_metadata,
									 const char                     *metadata);
void                    dory_file_set_metadata_list                 (DoryFile                   *file,
									 const char                     *key,
									 GList                          *list);
void                    dory_file_set_desktop_grid_adjusts (DoryFile   *file,
                                                            const char *key,
                                                            int         int_a,
                                                            int         int_b);
void                    dory_file_get_desktop_grid_adjusts (DoryFile   *file,
                                                            const char *key,
                                                            int        *int_a,
                                                            int        *int_b);
/* Covers for common data types. */
gboolean                dory_file_get_boolean_metadata              (DoryFile                   *file,
									 const char                     *key,
									 gboolean                        default_metadata);
void                    dory_file_set_boolean_metadata              (DoryFile                   *file,
									 const char                     *key,
									 gboolean                        default_metadata,
									 gboolean                        metadata);
int                     dory_file_get_integer_metadata              (DoryFile                   *file,
									 const char                     *key,
									 int                             default_metadata);
void                    dory_file_set_integer_metadata              (DoryFile                   *file,
									 const char                     *key,
									 int                             default_metadata,
									 int                             metadata);

#define UNDEFINED_TIME ((time_t) (-1))

time_t                  dory_file_get_time_metadata                 (DoryFile                  *file,
									 const char                    *key);
void                    dory_file_set_time_metadata                 (DoryFile                  *file,
									 const char                    *key,
									 time_t                         time);


/* Attributes for file objects as user-displayable strings. */
char *                  dory_file_get_string_attribute              (DoryFile                   *file,
									 const char                     *attribute_name);
char *                  dory_file_get_string_attribute_q            (DoryFile                   *file,
									 GQuark                          attribute_q);
char *                  dory_file_get_string_attribute_with_default (DoryFile                   *file,
									 const char                     *attribute_name);
char *                  dory_file_get_string_attribute_with_default_q (DoryFile                  *file,
									 GQuark                          attribute_q);

/* Matching with another URI. */
gboolean                dory_file_matches_uri                       (DoryFile                   *file,
									 const char                     *uri);

/* Is the file local? */
gboolean                dory_file_is_local                          (DoryFile                   *file);

/* Comparing two file objects for sorting */
DoryFileSortType    dory_file_get_default_sort_type             (DoryFile                   *file,
									 gboolean                       *reversed);
const gchar *           dory_file_get_default_sort_attribute        (DoryFile                   *file,
									 gboolean                       *reversed);

int                     dory_file_compare_for_sort                  (DoryFile                   *file_1,
									 DoryFile                   *file_2,
									 DoryFileSortType            sort_type,
									 gboolean			 directories_first,
									 gboolean            favorites_first,
									 gboolean		  	 reversed,
                                     gpointer                       search_dir);
int                     dory_file_compare_for_sort_by_attribute     (DoryFile                   *file_1,
									 DoryFile                   *file_2,
									 const char                     *attribute,
									 gboolean                        directories_first,
									 gboolean                        favorites_first,
									 gboolean                        reversed,
                                     gpointer                        search_dir);
int                     dory_file_compare_for_sort_by_attribute_q   (DoryFile                   *file_1,
									 DoryFile                   *file_2,
									 GQuark                          attribute,
									 gboolean                        directories_first,
									 gboolean                        favorites_first,
									 gboolean                        reversed,
                                     gpointer                        search_dir);
gboolean                dory_file_is_date_sort_attribute_q          (GQuark                          attribute);
gboolean                dory_file_attribute_slow_sort               (const gchar                    *sort_attribute);

int                     dory_file_compare_display_name              (DoryFile                   *file_1,
									 const char                     *pattern);
int                     dory_file_compare_location                  (DoryFile                    *file_1,
                                                                         DoryFile                    *file_2);

/* filtering functions for use by various directory views */
gboolean                dory_file_is_hidden_file                    (DoryFile                   *file);
gboolean                dory_file_should_show                       (DoryFile                   *file,
									 gboolean                        show_hidden,
									 gboolean                        show_foreign);
GList                  *dory_file_list_filter_hidden                (GList                          *files,
									 gboolean                        show_hidden);


/* Get the URI that's used when activating the file.
 * Getting this can require reading the contents of the file.
 */
gboolean                dory_file_is_launcher                       (DoryFile                   *file);
gboolean                dory_file_is_foreign_link                   (DoryFile                   *file);
gboolean                dory_file_is_trusted_link                   (DoryFile                   *file);
gboolean                dory_file_has_activation_uri                (DoryFile                   *file);
char *                  dory_file_get_activation_uri                (DoryFile                   *file);
GFile *                 dory_file_get_activation_location           (DoryFile                   *file);

char *                  dory_file_get_drop_target_uri               (DoryFile                   *file);

GIcon *                 dory_file_get_gicon                         (DoryFile                   *file,
                                                                     DoryFileIconFlags           flags);
gchar *                 dory_file_get_control_icon_name             (DoryFile                   *file);

DoryIconInfo *      dory_file_get_icon                          (DoryFile                   *file,
									 int                             size,
                                     int                             max_width,
                                     int                             scale,
									 DoryFileIconFlags           flags);
GdkPixbuf *             dory_file_get_icon_pixbuf                   (DoryFile                   *file,
									 int                             size,
									 gboolean                        force_size,
                                     int                             scale,
									 DoryFileIconFlags           flags);

gboolean                dory_file_has_open_window                   (DoryFile                   *file);
void                    dory_file_set_has_open_window               (DoryFile                   *file,
									 gboolean                        has_open_window);

/* Thumbnailing handling */
gboolean                dory_file_is_thumbnailing                   (DoryFile                   *file);

/* Convenience functions for dealing with a list of DoryFile objects that each have a ref.
 * These are just convenient names for functions that work on lists of GtkObject *.
 */
GList *                 dory_file_list_ref                          (GList                          *file_list);
void                    dory_file_list_unref                        (GList                          *file_list);
void                    dory_file_list_free                         (GList                          *file_list);
GList *                 dory_file_list_copy                         (GList                          *file_list);
GList *                 dory_file_list_from_uris                    (GList                          *uri_list);
GList *			dory_file_list_sort_by_display_name		(GList				*file_list);
void                    dory_file_list_call_when_ready              (GList                          *file_list,
									 DoryFileAttributes          attributes,
									 DoryFileListHandle        **handle,
									 DoryFileListCallback        callback,
									 gpointer                        callback_data);
void                    dory_file_list_cancel_call_when_ready       (DoryFileListHandle         *handle);

char *   dory_file_get_owner_as_string            (DoryFile          *file,
                                                          gboolean           include_real_name);
char *   dory_file_get_type_as_string             (DoryFile          *file);
char *   dory_file_get_detailed_type_as_string    (DoryFile          *file);

gchar *  dory_file_construct_tooltip              (DoryFile *file, DoryFileTooltipFlags flags, gpointer search_dir);

gboolean dory_file_has_thumbnail_access_problem   (DoryFile *file);

gint     dory_file_get_monitor_number             (DoryFile *file);
void     dory_file_set_monitor_number             (DoryFile *file, gint monitor);
void     dory_file_get_position                   (DoryFile *file, GdkPoint *point);
void     dory_file_set_position                   (DoryFile *file, gint x, gint y);
gboolean dory_file_get_is_desktop_orphan          (DoryFile *file);
void     dory_file_set_is_desktop_orphan          (DoryFile *file, gboolean is_desktop_orphan);

gboolean dory_file_get_pinning                    (DoryFile *file);
void     dory_file_set_pinning                    (DoryFile *file, gboolean  pin);
gboolean dory_file_get_is_favorite                (DoryFile *file);
void     dory_file_set_is_favorite                (DoryFile *file, gboolean favorite);
void     dory_file_set_load_deferred_attrs        (DoryFile *file,
                                                   DoryFileLoadDeferredAttrs load_deferred_attrs);
DoryFileLoadDeferredAttrs dory_file_get_load_deferred_attrs (DoryFile *file);

gboolean dory_file_add_search_result_data             (DoryFile *file, gpointer search_dir, FileSearchResult *result);
void dory_file_clear_search_result_data           (DoryFile *file, gpointer search_dir);
gboolean dory_file_has_search_result              (DoryFile *file, gpointer search_dir);
gint dory_file_get_search_result_count            (DoryFile *file, gpointer search_dir);
gchar *dory_file_get_search_result_count_as_string (DoryFile *file, gpointer search_dir);
gchar *dory_file_get_search_result_snippet        (DoryFile *file, gpointer search_dir);

/* Debugging */
void                    dory_file_dump                              (DoryFile                   *file);

typedef struct DoryFileDetails DoryFileDetails;

struct DoryFile {
	GObject parent_slot;
	DoryFileDetails *details;
};

typedef struct {
	GObjectClass parent_slot;

	/* Subclasses can set this to something other than G_FILE_TYPE_UNKNOWN and
	   it will be used as the default file type. This is useful when creating
	   a "virtual" DoryFile subclass that you can't actually get real
	   information about. For exaple DoryDesktopDirectoryFile. */
	GFileType default_file_type; 
	
	/* Called when the file notices any change. */
	void                  (* changed)                (DoryFile *file);

	/* Called periodically while directory deep count is being computed. */
	void                  (* updated_deep_count_in_progress) (DoryFile *file);

	/* Virtual functions (mainly used for trash directory). */
	void                  (* monitor_add)            (DoryFile           *file,
							  gconstpointer           client,
							  DoryFileAttributes  attributes);
	void                  (* monitor_remove)         (DoryFile           *file,
							  gconstpointer           client);
	void                  (* call_when_ready)        (DoryFile           *file,
							  DoryFileAttributes  attributes,
							  DoryFileCallback    callback,
							  gpointer                callback_data);
	void                  (* cancel_call_when_ready) (DoryFile           *file,
							  DoryFileCallback    callback,
							  gpointer                callback_data);
	gboolean              (* check_if_ready)         (DoryFile           *file,
							  DoryFileAttributes  attributes);
	gboolean              (* get_item_count)         (DoryFile           *file,
							  guint                  *count,
							  gboolean               *count_unreadable);
	DoryRequestStatus (* get_deep_counts)        (DoryFile           *file,
							  guint                  *directory_count,
							  guint                  *file_count,
							  guint                  *unreadable_directory_count,
                              guint                  *hidden_count,
							  goffset       *total_size);
	gboolean              (* get_date)               (DoryFile           *file,
							  DoryDateType        type,
							  time_t                 *date);
	char *                (* get_where_string)       (DoryFile           *file);

	void                  (* set_metadata)           (DoryFile           *file,
                                                      const char         *key,
                                                      const char         *value);
	void                  (* set_metadata_as_list)   (DoryFile           *file,
                                                      const char         *key,
                                                      char              **value);
    gchar *               (* get_metadata)           (DoryFile           *file,
                                                      const char         *key);
    gchar **              (* get_metadata_as_list)   (DoryFile           *file,
                                                      const char         *key);
	void                  (* mount)                  (DoryFile                   *file,
							  GMountOperation                *mount_op,
							  GCancellable                   *cancellable,
							  DoryFileOperationCallback   callback,
							  gpointer                        callback_data);
	void                 (* unmount)                 (DoryFile                   *file,
							  GMountOperation                *mount_op,
							  GCancellable                   *cancellable,
							  DoryFileOperationCallback   callback,
							  gpointer                        callback_data);
	void                 (* eject)                   (DoryFile                   *file,
							  GMountOperation                *mount_op,
							  GCancellable                   *cancellable,
							  DoryFileOperationCallback   callback,
							  gpointer                        callback_data);

	void                  (* start)                  (DoryFile                   *file,
							  GMountOperation                *start_op,
							  GCancellable                   *cancellable,
							  DoryFileOperationCallback   callback,
							  gpointer                        callback_data);
	void                 (* stop)                    (DoryFile                   *file,
							  GMountOperation                *mount_op,
							  GCancellable                   *cancellable,
							  DoryFileOperationCallback   callback,
							  gpointer                        callback_data);

	void                 (* poll_for_media)          (DoryFile                   *file);
} DoryFileClass;

#define DORY_FILE_URI(msg,f)                                    \
    {                                                           \
        gchar *uri = dory_file_get_uri (DORY_FILE (f));         \
        g_message ("%s: %p - %s", msg, f, uri);                 \
        g_free (uri);                                           \
    }                                                           \

#endif /* DORY_FILE_H */
