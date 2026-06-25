/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   dory-directory-private.h: Dory directory model.
 
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

#include <gio/gio.h>
#include <eel/eel-vfs-extensions.h>
#include <libdory-private/dory-directory.h>
#include <libdory-private/dory-file-queue.h>
#include <libdory-private/dory-file.h>
#include <libdory-private/dory-monitor.h>
#include <libdory-extension/dory-info-provider.h>

typedef struct LinkInfoReadState LinkInfoReadState;
typedef struct FileMonitors FileMonitors;
typedef struct DirectoryLoadState DirectoryLoadState;
typedef struct DirectoryCountState DirectoryCountState;
typedef struct DeepCountState DeepCountState;
typedef struct GetInfoState GetInfoState;
typedef struct NewFilesState NewFilesState;
typedef struct MimeListState MimeListState;
typedef struct ThumbnailState ThumbnailState;
typedef struct MountState MountState;
typedef struct FilesystemInfoState FilesystemInfoState;
typedef struct FavoriteCheckState FavoriteCheckState;

typedef enum {
	REQUEST_LINK_INFO,
	REQUEST_DEEP_COUNT,
	REQUEST_DIRECTORY_COUNT,
	REQUEST_FILE_INFO,
	REQUEST_FILE_LIST, /* always FALSE if file != NULL */
	REQUEST_MIME_LIST,
	REQUEST_EXTENSION_INFO,
	REQUEST_THUMBNAIL,
	REQUEST_MOUNT,
	REQUEST_FILESYSTEM_INFO,
    REQUEST_FAVORITE_CHECK,
	REQUEST_TYPE_LAST
} RequestType;

/* A request for information about one or more files. */
typedef guint32 Request;
typedef gint32 RequestCounter[REQUEST_TYPE_LAST];

#define REQUEST_WANTS_TYPE(request, type) ((request) & (1<<(type)))
#define REQUEST_SET_TYPE(request, type) (request) |= (1<<(type))

struct DoryDirectoryDetails
{
	/* The location. */
	GFile *location;

	/* The file objects. */
	DoryFile *as_file;
	GList *file_list;
	GHashTable *file_hash;

	/* Queues of files needing some I/O done. */
	DoryFileQueue *high_priority_queue;
	DoryFileQueue *low_priority_queue;
	DoryFileQueue *extension_queue;

	/* These lists are going to be pretty short.  If we think they
	 * are going to get big, we can use hash tables instead.
	 */
	GList *call_when_ready_list;
	RequestCounter call_when_ready_counters;
	GList *monitor_list;
	RequestCounter monitor_counters;
	guint call_ready_idle_id;

	DoryMonitor *monitor;
	gulong 		 mime_db_monitor;

	gboolean in_async_service_loop;
	gboolean state_changed;

	gboolean file_list_monitored;
	gboolean directory_loaded;
	gboolean directory_loaded_sent_notification;
	DirectoryLoadState *directory_load_in_progress;

	GList *pending_file_info; /* list of GnomeVFSFileInfo's that are pending */
	int confirmed_file_count;
        guint dequeue_pending_idle_id;

	GList *new_files_in_progress; /* list of NewFilesState * */

	/* List of GFile's that received CHANGE events while new files were being added in
	 * that same folder. We will process this CHANGE events after new_files_in_progress
	 * list is finished. See Bug 703179 for a case when this happens. */
	GList *new_files_in_progress_changes;

	DirectoryCountState *count_in_progress;

	DoryFile *deep_count_file;
	DeepCountState *deep_count_in_progress;

	MimeListState *mime_list_in_progress;

	DoryFile *get_info_file;
	GetInfoState *get_info_in_progress;

    DoryFile *favorite_check_file;
    FavoriteCheckState *favorite_check_in_progress;
    guint favorite_check_idle_id;

	DoryFile *extension_info_file;
	DoryInfoProvider *extension_info_provider;
	DoryOperationHandle *extension_info_in_progress;
	guint extension_info_idle;
    GClosure * extension_info_closure;

	ThumbnailState *thumbnail_state;

	MountState *mount_state;

	FilesystemInfoState *filesystem_info_state;

	LinkInfoReadState *link_info_read_state;

	GList *file_operations_in_progress; /* list of FileOperation * */

    gint max_deferred_file_count;
    gint early_load_file_count;
};

DoryDirectory *dory_directory_get_existing                    (GFile                     *location);

/* async. interface */
void               dory_directory_async_state_changed             (DoryDirectory         *directory);
void               dory_directory_call_when_ready_internal        (DoryDirectory         *directory,
								       DoryFile              *file,
								       DoryFileAttributes     file_attributes,
								       gboolean                   wait_for_file_list,
								       DoryDirectoryCallback  directory_callback,
								       DoryFileCallback       file_callback,
								       gpointer                   callback_data);
gboolean           dory_directory_check_if_ready_internal         (DoryDirectory         *directory,
								       DoryFile              *file,
								       DoryFileAttributes     file_attributes);
void               dory_directory_cancel_callback_internal        (DoryDirectory         *directory,
								       DoryFile              *file,
								       DoryDirectoryCallback  directory_callback,
								       DoryFileCallback       file_callback,
								       gpointer                   callback_data);
void               dory_directory_monitor_add_internal            (DoryDirectory         *directory,
								       DoryFile              *file,
								       gconstpointer              client,
								       gboolean                   monitor_hidden_files,
								       DoryFileAttributes     attributes,
								       DoryDirectoryCallback  callback,
								       gpointer                   callback_data);
void               dory_directory_monitor_remove_internal         (DoryDirectory         *directory,
								       DoryFile              *file,
								       gconstpointer              client);
void               dory_directory_get_info_for_new_files          (DoryDirectory         *directory,
								       GList                     *vfs_uris);
DoryFile *     dory_directory_get_existing_corresponding_file (DoryDirectory         *directory);
void               dory_directory_invalidate_count_and_mime_list  (DoryDirectory         *directory);
gboolean           dory_directory_is_file_list_monitored          (DoryDirectory         *directory);
gboolean           dory_directory_is_anyone_monitoring_file_list  (DoryDirectory         *directory);
gboolean           dory_directory_has_active_request_for_file     (DoryDirectory         *directory,
								       DoryFile              *file);
void               dory_directory_remove_file_monitor_link        (DoryDirectory         *directory,
								       GList                     *link);
void               dory_directory_schedule_dequeue_pending        (DoryDirectory         *directory);
void               dory_directory_stop_monitoring_file_list       (DoryDirectory         *directory);
void               dory_directory_cancel                          (DoryDirectory         *directory);
void               dory_async_destroying_file                     (DoryFile              *file);
void               dory_directory_force_reload_internal           (DoryDirectory         *directory,
								       DoryFileAttributes     file_attributes);
void               dory_directory_cancel_loading_file_attributes  (DoryDirectory         *directory,
								       DoryFile              *file,
								       DoryFileAttributes     file_attributes);

/* Calls shared between directory, file, and async. code. */
void               dory_directory_emit_files_added                (DoryDirectory         *directory,
								       GList                     *added_files);
void               dory_directory_emit_files_changed              (DoryDirectory         *directory,
								       GList                     *changed_files);
void               dory_directory_emit_change_signals             (DoryDirectory         *directory,
								       GList                     *changed_files);
void               emit_change_signals_for_all_files		      (DoryDirectory	 *directory);
void               emit_change_signals_for_all_files_in_all_directories (void);
void               dory_directory_emit_done_loading               (DoryDirectory         *directory);
void               dory_directory_emit_load_error                 (DoryDirectory         *directory,
								       GError                    *error);
DoryDirectory *dory_directory_get_internal                    (GFile                     *location,
								       gboolean                   create);
char *             dory_directory_get_name_for_self_as_new_file   (DoryDirectory         *directory);
Request            dory_directory_set_up_request                  (DoryFileAttributes     file_attributes);

/* Interface to the file list. */
DoryFile *     dory_directory_find_file_by_name               (DoryDirectory         *directory,
								       const char                *filename);
DoryFile *     dory_directory_find_file_by_internal_filename  (DoryDirectory         *directory,
								       const char                *internal_filename);

void               dory_directory_remove_file                     (DoryDirectory         *directory,
								       DoryFile              *file);
FileMonitors *     dory_directory_remove_file_monitors            (DoryDirectory         *directory,
								       DoryFile              *file);
void               dory_directory_add_file_monitors               (DoryDirectory         *directory,
								       DoryFile              *file,
								       FileMonitors              *monitors);
void               dory_directory_add_file                        (DoryDirectory         *directory,
								       DoryFile              *file);
GList *            dory_directory_begin_file_name_change          (DoryDirectory         *directory,
								       DoryFile              *file);
void               dory_directory_end_file_name_change            (DoryDirectory         *directory,
								       DoryFile              *file,
								       GList                     *node);
void               dory_directory_moved                           (const char                *from_uri,
								       const char                *to_uri);
/* Interface to the work queue. */

void               dory_directory_add_file_to_work_queue          (DoryDirectory *directory,
								       DoryFile *file);
void               dory_directory_remove_file_from_work_queue     (DoryDirectory *directory,
								       DoryFile *file);


/* debugging functions */
int                dory_directory_number_outstanding              (void);
