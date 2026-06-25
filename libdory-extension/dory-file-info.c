/*
 *  dory-file-info.c - Information about a file 
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

#include <config.h>
#include "dory-file-info.h"
#include "dory-extension-private.h"

G_DEFINE_INTERFACE (DoryFileInfo, dory_file_info, G_TYPE_OBJECT)

DoryFileInfo *(*dory_file_info_getter) (GFile *location, gboolean create);

/**
 * SECTION:dory-file-info
 * @Title: DoryFileInfo
 * @Short_description: A wrapper interface for extensions to access DoryFile info.
 *
 * This inteface is implemented by DoryFile and provides access to certain information
 * regarding a given file object.  It is also used to add file attributes and notify
 * a file of changes to those attribues when using a #DoryInfoProvider.
 **/

static void
dory_file_info_default_init (DoryFileInfoInterface *klass)
{
}

/**
 * dory_file_info_list_copy:
 * @files: (element-type DoryFileInfo): the files to copy
 *
 * Returns: (element-type DoryFileInfo) (transfer full): a copy of @files.
 *  Use #dory_file_info_list_free to free the list and unref its contents.
 */
GList *
dory_file_info_list_copy (GList *files)
{
    GList *ret;
    GList *l;
    
    ret = g_list_copy (files);
    for (l = ret; l != NULL; l = l->next) {
        g_object_ref (G_OBJECT (l->data));
    }

    return ret;
}

/**
 * dory_file_info_list_free:
 * @files: (element-type DoryFileInfo): a list created with
 *   #dory_file_info_list_copy
 *
 */
void              
dory_file_info_list_free (GList *files)
{
    GList *l;
    
    for (l = files; l != NULL; l = l->next) {
        g_object_unref (G_OBJECT (l->data));
    }
    
    g_list_free (files);
}

gboolean
dory_file_info_is_gone (DoryFileInfo *file)
{
	g_return_val_if_fail (DORY_IS_FILE_INFO (file), FALSE);
	g_return_val_if_fail (DORY_FILE_INFO_GET_IFACE (file)->is_gone != NULL, FALSE);
	
	return DORY_FILE_INFO_GET_IFACE (file)->is_gone (file);
}

GFileType
dory_file_info_get_file_type (DoryFileInfo *file)
{
	g_return_val_if_fail (DORY_IS_FILE_INFO (file), G_FILE_TYPE_UNKNOWN);
	g_return_val_if_fail (DORY_FILE_INFO_GET_IFACE (file)->get_file_type != NULL, G_FILE_TYPE_UNKNOWN);

	return DORY_FILE_INFO_GET_IFACE (file)->get_file_type (file);
}

char *
dory_file_info_get_name (DoryFileInfo *file)
{
	g_return_val_if_fail (DORY_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (DORY_FILE_INFO_GET_IFACE (file)->get_name != NULL, NULL);

	return DORY_FILE_INFO_GET_IFACE (file)->get_name (file);
}

/**
 * dory_file_info_get_location:
 * @file: a #DoryFileInfo
 *
 * Returns: (transfer full): a #GFile for the location of @file
 */
GFile *
dory_file_info_get_location (DoryFileInfo *file)
{
	g_return_val_if_fail (DORY_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (DORY_FILE_INFO_GET_IFACE (file)->get_location != NULL, NULL);

	return DORY_FILE_INFO_GET_IFACE (file)->get_location (file);
}
char *
dory_file_info_get_uri (DoryFileInfo *file)
{
	g_return_val_if_fail (DORY_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (DORY_FILE_INFO_GET_IFACE (file)->get_uri != NULL, NULL);

	return DORY_FILE_INFO_GET_IFACE (file)->get_uri (file);
}

char *
dory_file_info_get_activation_uri (DoryFileInfo *file)
{
	g_return_val_if_fail (DORY_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (DORY_FILE_INFO_GET_IFACE (file)->get_activation_uri != NULL, NULL);

	return DORY_FILE_INFO_GET_IFACE (file)->get_activation_uri (file);
}

/**
 * dory_file_info_get_parent_location:
 * @file: a #DoryFileInfo
 *
 * Returns: (allow-none) (transfer full): a #GFile for the parent location of @file, 
 *   or %NULL if @file has no parent
 */
GFile *
dory_file_info_get_parent_location (DoryFileInfo *file)
{
	g_return_val_if_fail (DORY_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (DORY_FILE_INFO_GET_IFACE (file)->get_parent_location != NULL, NULL);

	return DORY_FILE_INFO_GET_IFACE (file)->get_parent_location (file);
}

char *
dory_file_info_get_parent_uri (DoryFileInfo *file)
{
	g_return_val_if_fail (DORY_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (DORY_FILE_INFO_GET_IFACE (file)->get_parent_uri != NULL, NULL);

	return DORY_FILE_INFO_GET_IFACE (file)->get_parent_uri (file);
}

/**
 * dory_file_info_get_parent_info:
 * @file: a #DoryFileInfo
 *
 * Returns: (allow-none) (transfer full): a #DoryFileInfo for the parent of @file, 
 *   or %NULL if @file has no parent
 */
DoryFileInfo *
dory_file_info_get_parent_info (DoryFileInfo *file)
{
	g_return_val_if_fail (DORY_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (DORY_FILE_INFO_GET_IFACE (file)->get_parent_info != NULL, NULL);

	return DORY_FILE_INFO_GET_IFACE (file)->get_parent_info (file);
}

/**
 * dory_file_info_get_mount:
 * @file: a #DoryFileInfo
 *
 * Returns: (allow-none) (transfer full): a #GMount for the mount of @file, 
 *   or %NULL if @file has no mount
 */
GMount *
dory_file_info_get_mount (DoryFileInfo *file)
{
	g_return_val_if_fail (DORY_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (DORY_FILE_INFO_GET_IFACE (file)->get_mount != NULL, NULL);
    
	return DORY_FILE_INFO_GET_IFACE (file)->get_mount (file);
}

char *
dory_file_info_get_uri_scheme (DoryFileInfo *file)
{
	g_return_val_if_fail (DORY_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (DORY_FILE_INFO_GET_IFACE (file)->get_uri_scheme != NULL, NULL);

	return DORY_FILE_INFO_GET_IFACE (file)->get_uri_scheme (file);
}

char *
dory_file_info_get_mime_type (DoryFileInfo *file)
{
	g_return_val_if_fail (DORY_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (DORY_FILE_INFO_GET_IFACE (file)->get_mime_type != NULL, NULL);

	return DORY_FILE_INFO_GET_IFACE (file)->get_mime_type (file);
}

gboolean
dory_file_info_is_mime_type (DoryFileInfo *file,
				 const char *mime_type)
{
	g_return_val_if_fail (DORY_IS_FILE_INFO (file), FALSE);
	g_return_val_if_fail (mime_type != NULL, FALSE);
	g_return_val_if_fail (DORY_FILE_INFO_GET_IFACE (file)->is_mime_type != NULL, FALSE);

	return DORY_FILE_INFO_GET_IFACE (file)->is_mime_type (file,
								  mime_type);
}

gboolean
dory_file_info_is_directory (DoryFileInfo *file)
{
	g_return_val_if_fail (DORY_IS_FILE_INFO (file), FALSE);
	g_return_val_if_fail (DORY_FILE_INFO_GET_IFACE (file)->is_directory != NULL, FALSE);

	return DORY_FILE_INFO_GET_IFACE (file)->is_directory (file);
}

gboolean
dory_file_info_can_write (DoryFileInfo *file)
{
	g_return_val_if_fail (DORY_IS_FILE_INFO (file), FALSE);
	g_return_val_if_fail (DORY_FILE_INFO_GET_IFACE (file)->can_write != NULL, FALSE);

	return DORY_FILE_INFO_GET_IFACE (file)->can_write (file);
}

void
dory_file_info_add_emblem (DoryFileInfo *file,
			       const char *emblem_name)
{
	g_return_if_fail (DORY_IS_FILE_INFO (file));
	g_return_if_fail (DORY_FILE_INFO_GET_IFACE (file)->add_emblem != NULL);

	DORY_FILE_INFO_GET_IFACE (file)->add_emblem (file, emblem_name);
}

char *
dory_file_info_get_string_attribute (DoryFileInfo *file,
					 const char *attribute_name)
{
	g_return_val_if_fail (DORY_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (DORY_FILE_INFO_GET_IFACE (file)->get_string_attribute != NULL, NULL);
	g_return_val_if_fail (attribute_name != NULL, NULL);

	return DORY_FILE_INFO_GET_IFACE (file)->get_string_attribute 
		(file, attribute_name);
}

void
dory_file_info_add_string_attribute (DoryFileInfo *file,
					 const char *attribute_name,
					 const char *value)
{
	g_return_if_fail (DORY_IS_FILE_INFO (file));
	g_return_if_fail (DORY_FILE_INFO_GET_IFACE (file)->add_string_attribute != NULL);
	g_return_if_fail (attribute_name != NULL);
	g_return_if_fail (value != NULL);
	
	DORY_FILE_INFO_GET_IFACE (file)->add_string_attribute 
		(file, attribute_name, value);
}

/**
 * dory_file_info_invalidate_extension_info:
 * @file: a #DoryFile
 *
 * Notifies dory to re-run info provider extensions on the given file.
 *
 * This is useful if you have an extension that listens or responds to some external
 * interface for changes to local file metadata (such as a cloud drive changing file emblems.)
 *
 * When a change such as this occurs, call this on the file in question, and dory will
 * schedule a call to extension->update_file_info to update its own internal metadata.
 *
 * NOTE: This does *not* need to be called on the tail end of a update_full/update_complete
 * asynchronous extension.  Prior to Dory 3.6 this was indeed the case, however, due to a
 * recursion issue in dory-directory-async.c (see dory 9e67417f8f09.)
 */
void
dory_file_info_invalidate_extension_info (DoryFileInfo *file)
{
	g_return_if_fail (DORY_IS_FILE_INFO (file));
	g_return_if_fail (DORY_FILE_INFO_GET_IFACE (file)->invalidate_extension_info != NULL);
	
	DORY_FILE_INFO_GET_IFACE (file)->invalidate_extension_info (file);
}

/**
 * dory_file_info_lookup:
 * @location: the location to lookup the file info for
 *
 * Returns: (transfer full): a #DoryFileInfo
 */
DoryFileInfo *
dory_file_info_lookup (GFile *location)
{
	return dory_file_info_getter (location, FALSE);
}

/**
 * dory_file_info_create:
 * @location: the location to create the file info for
 *
 * Returns: (transfer full): a #DoryFileInfo
 */
DoryFileInfo *
dory_file_info_create (GFile *location)
{
	return dory_file_info_getter (location, TRUE);
}

/**
 * dory_file_info_lookup_for_uri:
 * @uri: the URI to lookup the file info for
 *
 * Returns: (transfer full): a #DoryFileInfo
 */
DoryFileInfo *
dory_file_info_lookup_for_uri (const char *uri)
{
	GFile *location;
	DoryFile *file;

	location = g_file_new_for_uri (uri);
	file = dory_file_info_lookup (location);
	g_object_unref (location);

	return file;
}

/**
 * dory_file_info_create_for_uri:
 * @uri: the URI to lookup the file info for
 *
 * Returns: (transfer full): a #DoryFileInfo
 */
DoryFileInfo *
dory_file_info_create_for_uri (const char *uri)
{
	GFile *location;
	DoryFile *file;

	location = g_file_new_for_uri (uri);
	file = dory_file_info_create (location);
	g_object_unref (location);

	return file;
}
