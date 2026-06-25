/*
 *  dory-file-info.h - Information about a file 
 *
 *  Copyright (C) 2003 Novell, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin Street, Suite 500, MA 02110-1335, USA.
 *
 */

/* DoryFileInfo is an interface to the DoryFile object.  It 
 * provides access to the asynchronous data in the DoryFile.
 * Extensions are passed objects of this type for operations. */

#ifndef DORY_FILE_INFO_H
#define DORY_FILE_INFO_H

#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define DORY_TYPE_FILE_INFO           (dory_file_info_get_type ())
#define DORY_FILE_INFO(obj)           (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_FILE_INFO, DoryFileInfo))
#define DORY_IS_FILE_INFO(obj)        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_FILE_INFO))
#define DORY_FILE_INFO_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), DORY_TYPE_FILE_INFO, DoryFileInfoInterface))

#ifndef DORY_FILE_DEFINED
#define DORY_FILE_DEFINED
/* Using DoryFile for the vtable to make implementing this in 
 * DoryFile easier */
typedef struct DoryFile          DoryFile;
#endif

typedef DoryFile DoryFileInfo;
typedef struct _DoryFileInfoInterface DoryFileInfoInterface;

struct _DoryFileInfoInterface 
{
	GTypeInterface g_iface;

	gboolean          (*is_gone)              (DoryFileInfo *file);
	
	char *            (*get_name)             (DoryFileInfo *file);
	char *            (*get_uri)              (DoryFileInfo *file);
	char *            (*get_parent_uri)       (DoryFileInfo *file);
	char *            (*get_uri_scheme)       (DoryFileInfo *file);
	
	char *            (*get_mime_type)        (DoryFileInfo *file);
	gboolean          (*is_mime_type)         (DoryFileInfo *file,
						   const char       *mime_Type);
	gboolean          (*is_directory)         (DoryFileInfo *file);
	
	void              (*add_emblem)           (DoryFileInfo *file,
						   const char       *emblem_name);
	char *            (*get_string_attribute) (DoryFileInfo *file,
						   const char       *attribute_name);
	void              (*add_string_attribute) (DoryFileInfo *file,
						   const char       *attribute_name,
						   const char       *value);
	void              (*invalidate_extension_info) (DoryFileInfo *file);
	
	char *            (*get_activation_uri)   (DoryFileInfo *file);

	GFileType         (*get_file_type)        (DoryFileInfo *file);
	GFile *           (*get_location)         (DoryFileInfo *file);
	GFile *           (*get_parent_location)  (DoryFileInfo *file);
	DoryFileInfo* (*get_parent_info)      (DoryFileInfo *file);
	GMount *          (*get_mount)            (DoryFileInfo *file);
	gboolean          (*can_write)            (DoryFileInfo *file);
  
};

GList            *dory_file_info_list_copy            (GList            *files);
void              dory_file_info_list_free            (GList            *files);
GType             dory_file_info_get_type             (void);

/* Return true if the file has been deleted */
gboolean          dory_file_info_is_gone              (DoryFileInfo *file);

/* Name and Location */
GFileType         dory_file_info_get_file_type        (DoryFileInfo *file);
GFile *           dory_file_info_get_location         (DoryFileInfo *file);
char *            dory_file_info_get_name             (DoryFileInfo *file);
char *            dory_file_info_get_uri              (DoryFileInfo *file);
char *            dory_file_info_get_activation_uri   (DoryFileInfo *file);
GFile *           dory_file_info_get_parent_location  (DoryFileInfo *file);
char *            dory_file_info_get_parent_uri       (DoryFileInfo *file);
GMount *          dory_file_info_get_mount            (DoryFileInfo *file);
char *            dory_file_info_get_uri_scheme       (DoryFileInfo *file);
/* It's not safe to call this recursively multiple times, as it works
 * only for files already cached by Dory.
 */
DoryFileInfo* dory_file_info_get_parent_info      (DoryFileInfo *file);

/* File Type */
char *            dory_file_info_get_mime_type        (DoryFileInfo *file);
gboolean          dory_file_info_is_mime_type         (DoryFileInfo *file,
							   const char       *mime_type);
gboolean          dory_file_info_is_directory         (DoryFileInfo *file);
gboolean          dory_file_info_can_write            (DoryFileInfo *file);


/* Modifying the DoryFileInfo */
void              dory_file_info_add_emblem           (DoryFileInfo *file,
							   const char       *emblem_name);
char *            dory_file_info_get_string_attribute (DoryFileInfo *file,
							   const char       *attribute_name);
void              dory_file_info_add_string_attribute (DoryFileInfo *file,
							   const char       *attribute_name,
							   const char       *value);

/* Invalidating file info */
void              dory_file_info_invalidate_extension_info (DoryFileInfo *file);

DoryFileInfo *dory_file_info_lookup                (GFile *location);
DoryFileInfo *dory_file_info_create                (GFile *location);
DoryFileInfo *dory_file_info_lookup_for_uri        (const char *uri);
DoryFileInfo *dory_file_info_create_for_uri        (const char *uri);

G_END_DECLS

#endif
