/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   dory-directory.h: Dory directory model.
 
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

#ifndef DORY_DIRECTORY_H
#define DORY_DIRECTORY_H

#include <gtk/gtk.h>
#include <gio/gio.h>
#include <libdory-private/dory-file-attributes.h>

/* DoryDirectory is a class that manages the model for a directory,
   real or virtual, for Dory, mainly the file-manager component. The directory is
   responsible for managing both real data and cached metadata. On top of
   the file system independence provided by gio, the directory
   object also provides:
  
       1) A synchronization framework, which notifies via signals as the
          set of known files changes.
       2) An abstract interface for getting attributes and performing
          operations on files.
*/

#define DORY_TYPE_DIRECTORY dory_directory_get_type()
#define DORY_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_DIRECTORY, DoryDirectory))
#define DORY_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_DIRECTORY, DoryDirectoryClass))
#define DORY_IS_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_DIRECTORY))
#define DORY_IS_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_DIRECTORY))
#define DORY_DIRECTORY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_DIRECTORY, DoryDirectoryClass))

/* DoryFile is defined both here and in dory-file.h. */
#ifndef DORY_FILE_DEFINED
#define DORY_FILE_DEFINED
typedef struct DoryFile DoryFile;
#endif

typedef struct DoryDirectoryDetails DoryDirectoryDetails;

typedef struct
{
	GObject object;
	DoryDirectoryDetails *details;
} DoryDirectory;

typedef void (*DoryDirectoryCallback) (DoryDirectory *directory,
					   GList             *files,
					   gpointer           callback_data);

typedef struct
{
	GObjectClass parent_class;

	/*** Notification signals for clients to connect to. ***/

	/* The files_added signal is emitted as the directory model 
	 * discovers new files.
	 */
	void     (* files_added)         (DoryDirectory          *directory,
					  GList                      *added_files);

	/* The files_changed signal is emitted as changes occur to
	 * existing files that are noticed by the synchronization framework,
	 * including when an old file has been deleted. When an old file
	 * has been deleted, this is the last chance to forget about these
	 * file objects, which are about to be unref'd. Use a call to
	 * dory_file_is_gone () to test for this case.
	 */
	void     (* files_changed)       (DoryDirectory         *directory,
					  GList                     *changed_files);

	/* The done_loading signal is emitted when a directory load
	 * request completes. This is needed because, at least in the
	 * case where the directory is empty, the caller will receive
	 * no kind of notification at all when a directory load
	 * initiated by `dory_directory_file_monitor_add' completes.
	 */
	void     (* done_loading)        (DoryDirectory         *directory);

	void     (* load_error)          (DoryDirectory         *directory,
					  GError                    *error);

	/*** Virtual functions for subclasses to override. ***/
	gboolean (* contains_file)       (DoryDirectory         *directory,
					  DoryFile              *file);
	void     (* call_when_ready)     (DoryDirectory         *directory,
					  DoryFileAttributes     file_attributes,
					  gboolean                   wait_for_file_list,
					  DoryDirectoryCallback  callback,
					  gpointer                   callback_data);
	void     (* cancel_callback)     (DoryDirectory         *directory,
					  DoryDirectoryCallback  callback,
					  gpointer                   callback_data);
	void     (* file_monitor_add)    (DoryDirectory          *directory,
					  gconstpointer              client,
					  gboolean                   monitor_hidden_files,
					  DoryFileAttributes     monitor_attributes,
					  DoryDirectoryCallback  initial_files_callback,
					  gpointer                   callback_data);
	void     (* file_monitor_remove) (DoryDirectory         *directory,
					  gconstpointer              client);
	void     (* force_reload)        (DoryDirectory         *directory);
	gboolean (* are_all_files_seen)  (DoryDirectory         *directory);
	gboolean (* is_not_empty)        (DoryDirectory         *directory);

	/* get_file_list is a function pointer that subclasses may override to
	 * customize collecting the list of files in a directory.
	 * For example, the DoryDesktopDirectory overrides this so that it can
	 * merge together the list of files in the $HOME/Desktop directory with
	 * the list of standard icons (Computer, Home, Trash) on the desktop.
	 */
	GList *	 (* get_file_list)	 (DoryDirectory *directory);

	/* Should return FALSE if the directory is read-only and doesn't
	 * allow setting of metadata.
	 * An example of this is the search directory.
	 */
	gboolean (* is_editable)         (DoryDirectory *directory);
} DoryDirectoryClass;

/* Basic GObject requirements. */
GType              dory_directory_get_type                 (void);

/* Get a directory given a uri.
 * Creates the appropriate subclass given the uri mappings.
 * Returns a referenced object, not a floating one. Unref when finished.
 * If two windows are viewing the same uri, the directory object is shared.
 */
DoryDirectory *dory_directory_get                      (GFile                     *location);
DoryDirectory *dory_directory_get_by_uri               (const char                *uri);
DoryDirectory *dory_directory_get_for_file             (DoryFile              *file);

/* Covers for g_object_ref and g_object_unref that provide two conveniences:
 * 1) Using these is type safe.
 * 2) You are allowed to call these with NULL,
 */
DoryDirectory *dory_directory_ref                      (DoryDirectory         *directory);
void               dory_directory_unref                    (DoryDirectory         *directory);

/* Access to a URI. */
char *             dory_directory_get_uri                  (DoryDirectory         *directory);
GFile *            dory_directory_get_location             (DoryDirectory         *directory);

/* Is this file still alive and in this directory? */
gboolean           dory_directory_contains_file            (DoryDirectory         *directory,
								DoryFile              *file);

/* Get the uri of the file in the directory, NULL if not found */
char *             dory_directory_get_file_uri             (DoryDirectory         *directory,
								const char                *file_name);

/* Get (and ref) a DoryFile object for this directory. */
DoryFile *     dory_directory_get_corresponding_file   (DoryDirectory         *directory);

/* Waiting for data that's read asynchronously.
 * The file attribute and metadata keys are for files in the directory.
 */
void               dory_directory_call_when_ready          (DoryDirectory         *directory,
								DoryFileAttributes     file_attributes,
								gboolean                   wait_for_all_files,
								DoryDirectoryCallback  callback,
								gpointer                   callback_data);
void               dory_directory_cancel_callback          (DoryDirectory         *directory,
								DoryDirectoryCallback  callback,
								gpointer                   callback_data);


/* Monitor the files in a directory. */
void               dory_directory_file_monitor_add         (DoryDirectory         *directory,
								gconstpointer              client,
								gboolean                   monitor_hidden_files,
								DoryFileAttributes     attributes,
								DoryDirectoryCallback  initial_files_callback,
								gpointer                   callback_data);
void               dory_directory_file_monitor_remove      (DoryDirectory         *directory,
								gconstpointer              client);
void               dory_directory_force_reload             (DoryDirectory         *directory);

/* Get a list of all files currently known in the directory. */
GList *            dory_directory_get_file_list            (DoryDirectory         *directory);

GList *            dory_directory_match_pattern            (DoryDirectory         *directory,
							        const char *glob);


/* Return true if the directory has information about all the files.
 * This will be false until the directory has been read at least once.
 */
gboolean           dory_directory_are_all_files_seen       (DoryDirectory         *directory);

/* Return true if the directory is local. */
gboolean           dory_directory_is_local                 (DoryDirectory         *directory);

gboolean           dory_directory_is_in_trash              (DoryDirectory         *directory);
gboolean           dory_directory_is_in_recent             (DoryDirectory         *directory);
gboolean           dory_directory_is_in_favorites          (DoryDirectory         *directory);
gboolean           dory_directory_is_in_admin              (DoryDirectory         *directory);
gboolean           dory_directory_is_in_search             (DoryDirectory         *directory);
/* Return false if directory contains anything besides a Dory metafile.
 * Only valid if directory is monitored. Used by the Trash monitor.
 */
gboolean           dory_directory_is_not_empty             (DoryDirectory         *directory);

/* Convenience functions for dealing with a list of DoryDirectory objects that each have a ref.
 * These are just convenient names for functions that work on lists of GtkObject *.
 */
GList *            dory_directory_list_ref                 (GList                     *directory_list);
void               dory_directory_list_unref               (GList                     *directory_list);
void               dory_directory_list_free                (GList                     *directory_list);
GList *            dory_directory_list_copy                (GList                     *directory_list);
GList *            dory_directory_list_sort_by_uri         (GList                     *directory_list);

/* Fast way to check if a directory is the desktop directory */
gboolean           dory_directory_is_desktop_directory     (DoryDirectory         *directory);

gboolean           dory_directory_is_editable              (DoryDirectory         *directory);

void               dory_directory_set_show_thumbnails      (DoryDirectory         *directory,
                                gboolean show_thumbnails);

#endif /* DORY_DIRECTORY_H */
