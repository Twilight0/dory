/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* dory-bookmark.c - implementation of individual bookmarks.
 *
 * Copyright (C) 1999, 2000 Eazel, Inc.
 * Copyright (C) 2011, Red Hat, Inc.
 *
 * The Gnome Library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * The Gnome Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with the Gnome Library; see the file COPYING.LIB.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
 * Boston, MA 02110-1335, USA.
 *
 * Authors: John Sullivan <sullivan@eazel.com>
 *          Cosimo Cecchi <cosimoc@redhat.com>
 */

#include <config.h>

#include "dory-bookmark.h"
#include "dory-metadata.h"

#include <eel/eel-vfs-extensions.h>
#include <gio/gio.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include <libdory-private/dory-file.h>
#include <libdory-private/dory-file-utilities.h>
#include <libdory-private/dory-icon-names.h>

#define DEBUG_FLAG DORY_DEBUG_BOOKMARKS
#include <libdory-private/dory-debug.h>

enum {
	CONTENTS_CHANGED,
    LOCATION_MOUNTED,
    LOOKUP_METADATA,
	LAST_SIGNAL
};

enum {
	PROP_NAME = 1,
	PROP_CUSTOM_NAME,
	PROP_LOCATION,
	PROP_ICON_NAME,
    PROP_METADATA,
	NUM_PROPERTIES
};

#define ELLIPSISED_MENU_ITEM_MIN_CHARS  32

static GParamSpec* properties[NUM_PROPERTIES] = { NULL };
static guint signals[LAST_SIGNAL] = { 0 };

struct DoryBookmarkDetails
{
	char *name;
	gboolean has_custom_name;
	GFile *location;
    gchar *icon_name;
	DoryFile *file;

	char *scroll_file;

    DoryBookmarkMetadata *metadata;
};

static void	  dory_bookmark_disconnect_file	  (DoryBookmark	 *file);

G_DEFINE_TYPE (DoryBookmark, dory_bookmark, G_TYPE_OBJECT);

static void
dory_bookmark_set_name_internal (DoryBookmark *bookmark,
				     const char *new_name)
{
	if (g_strcmp0 (bookmark->details->name, new_name) != 0) {
		g_free (bookmark->details->name);
		bookmark->details->name = g_strdup (new_name);

		g_object_notify_by_pspec (G_OBJECT (bookmark), properties[PROP_NAME]);
	}
}

static void
dory_bookmark_update_icon (DoryBookmark *bookmark)
{
    gchar *new_icon_name;

    if (bookmark->details->file == NULL) {
        return;
    }

    if (!dory_file_is_not_yet_confirmed (bookmark->details->file) &&
        dory_file_check_if_ready (bookmark->details->file,
                                  DORY_FILE_ATTRIBUTES_FOR_ICON)) {
        DEBUG ("%s: set new icon", dory_bookmark_get_name (bookmark));

        new_icon_name = dory_file_get_control_icon_name (bookmark->details->file);
        g_object_set (bookmark,
                      "icon-name", new_icon_name,
                      NULL);

        g_free (new_icon_name);
    }
}

static void
bookmark_set_name_from_ready_file (DoryBookmark *self,
				   DoryFile *file)
{
	gchar *display_name;

	if (self->details->has_custom_name) {
		return;
	}

	display_name = dory_file_get_display_name (self->details->file);

	if (dory_file_is_home (self->details->file)) {
		dory_bookmark_set_custom_name (self, _("Home"));
	} else if (g_strcmp0 (self->details->name, display_name) != 0) {
		dory_bookmark_set_custom_name (self, display_name);
		DEBUG ("%s: name changed to %s", dory_bookmark_get_name (self), display_name);
	}

	g_free (display_name);
}

static gchar *
get_default_folder_icon_name (DoryBookmark *bookmark)
{
    gchar *ret = NULL;

    if (g_file_is_native (bookmark->details->location)) {
        ret = g_strdup (DORY_ICON_SYMBOLIC_FOLDER);
    } else {
        gchar *uri = g_file_get_uri (bookmark->details->location);
        if (g_str_has_prefix (uri, EEL_SEARCH_URI)) {
            ret = g_strdup (DORY_ICON_SYMBOLIC_FOLDER_SAVED_SEARCH);
        } else {
            ret = g_strdup (DORY_ICON_SYMBOLIC_FOLDER_REMOTE);
        }
        g_free (uri);
    }

    return ret;
}

static gchar *
construct_default_icon_from_metadata (DoryBookmark *bookmark)
{
    return get_default_folder_icon_name (bookmark);

/* TODO: Remove emblem stuff from bookmarks */
    // if (ret != NULL && md->emblems != NULL) {
    //     guint i = 0;

    //     GIcon *emb_icon;
    //     GEmblem *emblem;

    //     emb_icon = g_themed_icon_new (md->emblems[i]);
    //     emblem = g_emblem_new (emb_icon);

    //     ret = g_emblemed_icon_new (ret, emblem);

    //     i++;

    //     while (i < g_strv_length (md->emblems)) {
    //         emb_icon = g_themed_icon_new (md->emblems[i]);
    //         emblem = g_emblem_new (emb_icon);

    //         g_emblemed_icon_add_emblem (G_EMBLEMED_ICON (ret), emblem);

    //         i++;
    //     }
    // }

    // return ret;
}

static void
dory_bookmark_set_icon_to_default (DoryBookmark *bookmark)
{
    gchar *icon_name;

    icon_name = construct_default_icon_from_metadata (bookmark);

    if (!dory_bookmark_uri_get_exists (bookmark)) {
        DEBUG ("%s: file does not exist, use special icon", dory_bookmark_get_name (bookmark));

        g_clear_pointer (&icon_name, g_free);

        icon_name = g_strdup (DORY_ICON_SYMBOLIC_MISSING_BOOKMARK);
    }

    DEBUG ("%s: setting icon to default", dory_bookmark_get_name (bookmark));

    g_object_set (bookmark,
                  "icon-name", icon_name,
                  NULL);

    g_free (icon_name);
}

static gboolean
metadata_changed (DoryBookmark *bookmark)
{
    gboolean ret = FALSE;
    DoryBookmarkMetadata *data = dory_bookmark_get_updated_metadata (bookmark);
    gboolean has_custom = data && data->emblems;

    gboolean had_custom = bookmark->details->metadata != NULL;

    if ((has_custom && !had_custom) ||
       (had_custom && !has_custom)) {
        ret = TRUE;

    } else if (has_custom && had_custom) {
        DoryBookmarkMetadata *md = bookmark->details->metadata;
        ret = dory_bookmark_metadata_compare (data, md);
    }

    dory_bookmark_metadata_free (data);

    return ret;
}

static void
bookmark_file_changed_callback (DoryFile *file,
				DoryBookmark *bookmark)
{
	GFile *location;

	g_assert (file == bookmark->details->file);

	DEBUG ("%s: file changed", dory_bookmark_get_name (bookmark));

	location = dory_file_get_location (file);

	if (!g_file_equal (bookmark->details->location, location) &&
	    !dory_file_is_in_trash (file)) {
		DEBUG ("%s: file got moved", dory_bookmark_get_name (bookmark));

		g_object_unref (bookmark->details->location);
		bookmark->details->location = g_object_ref (location);

		g_object_notify_by_pspec (G_OBJECT (bookmark), properties[PROP_LOCATION]);
		g_signal_emit (bookmark, signals[CONTENTS_CHANGED], 0);
	}

	g_object_unref (location);

	if (dory_file_is_gone (file) ||
	    dory_file_is_in_trash (file)) {
		/* The file we were monitoring has been trashed, deleted,
		 * or moved in a way that we didn't notice. We should make
		 * a spanking new DoryFile object for this
		 * location so if a new file appears in this place
		 * we will notice. However, we can't immediately do so
		 * because creating a new DoryFile directly as a result
		 * of noticing a file goes away may trigger i/o on that file
		 * again, noticeing it is gone, leading to a loop.
		 * So, the new DoryFile is created when the bookmark
		 * is used again. However, this is not really a problem, as
		 * we don't want to change the icon or anything about the
		 * bookmark just because its not there anymore.
		 */
		DEBUG ("%s: trashed", dory_bookmark_get_name (bookmark));
		dory_bookmark_disconnect_file (bookmark);
        dory_bookmark_set_icon_to_default (bookmark);
	} else {
		dory_bookmark_update_icon (bookmark);
		bookmark_set_name_from_ready_file (bookmark, file);

        if (metadata_changed (bookmark)) {
            g_signal_emit (bookmark, signals[CONTENTS_CHANGED], 0);
        }
	}
}

static void
dory_bookmark_disconnect_file (DoryBookmark *bookmark)
{
	if (bookmark->details->file != NULL) {
		DEBUG ("%s: disconnecting file",
		       dory_bookmark_get_name (bookmark));

        g_signal_handlers_disconnect_by_func (bookmark->details->file,
                                              G_CALLBACK (bookmark_file_changed_callback),
                                              bookmark);

		g_clear_object (&bookmark->details->file);
	}
}

static void
dory_bookmark_connect_file (DoryBookmark *bookmark)
{
	if (bookmark->details->file != NULL) {
		DEBUG ("%s: file already connected, returning",
		       dory_bookmark_get_name (bookmark));
		return;
	}

	if (dory_bookmark_uri_get_exists (bookmark)) {
        DEBUG ("%s: creating file", dory_bookmark_get_name (bookmark));

		bookmark->details->file = dory_file_get (bookmark->details->location);

        g_assert (!dory_file_is_gone (bookmark->details->file));

        g_signal_connect_object (bookmark->details->file, "changed",
                                 G_CALLBACK (bookmark_file_changed_callback),
                                 bookmark, 0);
	}

	/* Set icon based on available information. */
	dory_bookmark_update_icon (bookmark);

	if (bookmark->details->icon_name == NULL) {
		dory_bookmark_set_icon_to_default (bookmark);
	}

	if (bookmark->details->file != NULL &&
	    dory_file_check_if_ready (bookmark->details->file, DORY_FILE_ATTRIBUTE_INFO)) {
		bookmark_set_name_from_ready_file (bookmark, bookmark->details->file);
	}

	if (bookmark->details->name == NULL) {
		bookmark->details->name = dory_compute_title_for_location (bookmark->details->location);
	}
}

/* GObject methods */

static void
dory_bookmark_set_property (GObject *object,
				guint property_id,
				const GValue *value,
				GParamSpec *pspec)
{
	DoryBookmark *self = DORY_BOOKMARK (object);
	switch (property_id) {
	case PROP_ICON_NAME:
        ;
        const gchar *new_icon_name;

        new_icon_name = g_value_get_string (value);
        if (new_icon_name != NULL && g_strcmp0 (self->details->icon_name, new_icon_name) != 0) {
            g_clear_pointer (&self->details->icon_name, g_free);
            self->details->icon_name = g_strdup (new_icon_name);
        }
		break;
	case PROP_LOCATION:
		self->details->location = g_value_dup_object (value);
		break;
	case PROP_CUSTOM_NAME:
		self->details->has_custom_name = g_value_get_boolean (value);
		break;
	case PROP_NAME:
		dory_bookmark_set_name_internal (self, g_value_get_string (value));
		break;
    case PROP_METADATA:
        if (self->details->metadata)
            g_clear_pointer (&self->details->metadata, dory_bookmark_metadata_free);

        self->details->metadata = g_value_get_pointer (value);
        break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}

static void
dory_bookmark_get_property (GObject *object,
				guint property_id,
				GValue *value,
				GParamSpec *pspec)
{
	DoryBookmark *self = DORY_BOOKMARK (object);

	switch (property_id) {
	case PROP_NAME:
		g_value_set_string (value, self->details->name);
		break;
	case PROP_ICON_NAME:
		g_value_set_string (value, self->details->icon_name);
		break;
	case PROP_LOCATION:
		g_value_set_object (value, self->details->location);
		break;
	case PROP_CUSTOM_NAME:
		g_value_set_boolean (value, self->details->has_custom_name);
		break;
    case PROP_METADATA:
        g_value_set_pointer (value, self->details->metadata);
        break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}

static void
dory_bookmark_finalize (GObject *object)
{
	DoryBookmark *bookmark;

	g_assert (DORY_IS_BOOKMARK (object));

	bookmark = DORY_BOOKMARK (object);

	dory_bookmark_disconnect_file (bookmark);

	g_object_unref (bookmark->details->location);
	g_clear_pointer (&bookmark->details->icon_name, g_free);

    g_clear_pointer (&bookmark->details->metadata, dory_bookmark_metadata_free);

	g_free (bookmark->details->name);
	g_free (bookmark->details->scroll_file);

	G_OBJECT_CLASS (dory_bookmark_parent_class)->finalize (object);
}

static void
dory_bookmark_class_init (DoryBookmarkClass *class)
{
	GObjectClass *oclass = G_OBJECT_CLASS (class);

	oclass->finalize = dory_bookmark_finalize;
	oclass->get_property = dory_bookmark_get_property;
	oclass->set_property = dory_bookmark_set_property;

	signals[CONTENTS_CHANGED] =
		g_signal_new ("contents-changed",
		              G_TYPE_FROM_CLASS (class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (DoryBookmarkClass, contents_changed),
		              NULL, NULL,
		              g_cclosure_marshal_VOID__VOID,
		              G_TYPE_NONE, 0);

    signals[LOCATION_MOUNTED] =
        g_signal_new ("location-mounted",
                      G_TYPE_FROM_CLASS (class),
                      G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (DoryBookmarkClass, location_mounted),
                      NULL, NULL,
                      g_cclosure_marshal_generic,
                      G_TYPE_BOOLEAN, 1,
                      G_TYPE_FILE);

	properties[PROP_NAME] =
		g_param_spec_string ("name",
				     "Bookmark's name",
				     "The name of this bookmark",
				     NULL,
				     G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT);

	properties[PROP_CUSTOM_NAME] =
		g_param_spec_boolean ("custom-name",
				      "Whether the bookmark has a custom name",
				      "Whether the bookmark has a custom name",
				      FALSE,
				      G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT);

	properties[PROP_LOCATION] =
		g_param_spec_object ("location",
				     "Bookmark's location",
				     "The location of this bookmark",
				     G_TYPE_FILE,
				     G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT_ONLY);

	properties[PROP_ICON_NAME] =
		g_param_spec_string ("icon-name",
				     "Bookmark's icon name",
				     "The icon name for this bookmark",
				     NULL,
				     G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

    properties[PROP_METADATA] =
        g_param_spec_pointer ("metadata",
                     "Bookmark's non-gvfs metadata",
                     "Metadata for defining the bookmark's icon",
                     G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties (oclass, NUM_PROPERTIES, properties);

	g_type_class_add_private (class, sizeof (DoryBookmarkDetails));
}

static void
dory_bookmark_init (DoryBookmark *bookmark)
{
	bookmark->details = G_TYPE_INSTANCE_GET_PRIVATE (bookmark, DORY_TYPE_BOOKMARK,
							 DoryBookmarkDetails);
}

const gchar *
dory_bookmark_get_name (DoryBookmark *bookmark)
{
	g_return_val_if_fail (DORY_IS_BOOKMARK (bookmark), NULL);

	return bookmark->details->name;
}

gboolean
dory_bookmark_get_has_custom_name (DoryBookmark *bookmark)
{
	g_return_val_if_fail(DORY_IS_BOOKMARK (bookmark), FALSE);

	return (bookmark->details->has_custom_name);
}

/**
 * dory_bookmark_set_custom_name:
 *
 * Change the user-displayed name of a bookmark.
 * @new_name: The new user-displayed name for this bookmark, mustn't be NULL.
 *
 **/
void
dory_bookmark_set_custom_name (DoryBookmark *bookmark,
				   const char *new_name)
{
	g_return_if_fail (new_name != NULL);
	g_return_if_fail (DORY_IS_BOOKMARK (bookmark));

	g_object_set (bookmark,
		      "custom-name", TRUE,
		      "name", new_name,
		      NULL);

	g_signal_emit (bookmark, signals[CONTENTS_CHANGED], 0);
}

/**
 * dory_bookmark_compare_with:
 *
 * Check whether two bookmarks are considered identical.
 * @a: first DoryBookmark*.
 * @b: second DoryBookmark*.
 *
 * Return value: 0 if @a and @b have same name and uri, 1 otherwise
 * (GCompareFunc style)
 **/
int
dory_bookmark_compare_with (gconstpointer a, gconstpointer b)
{
	DoryBookmark *bookmark_a;
	DoryBookmark *bookmark_b;

	g_return_val_if_fail (DORY_IS_BOOKMARK (a), 1);
	g_return_val_if_fail (DORY_IS_BOOKMARK (b), 1);

	bookmark_a = DORY_BOOKMARK (a);
	bookmark_b = DORY_BOOKMARK (b);

	if (!g_file_equal (bookmark_a->details->location,
			   bookmark_b->details->location)) {
		return 1;
	}

	if (g_strcmp0 (bookmark_a->details->name,
		       bookmark_b->details->name) != 0) {
		return 1;
	}

	return 0;
}

/**
 * dory_bookmark_compare_uris:
 *
 * Check whether the uris of two bookmarks are for the same location.
 * @a: first DoryBookmark*.
 * @b: second DoryBookmark*.
 *
 * Return value: 0 if @a and @b have matching uri, 1 otherwise
 * (GCompareFunc style)
 **/
int
dory_bookmark_compare_uris (gconstpointer a, gconstpointer b)
{
	DoryBookmark *bookmark_a;
	DoryBookmark *bookmark_b;

	g_return_val_if_fail (DORY_IS_BOOKMARK (a), 1);
	g_return_val_if_fail (DORY_IS_BOOKMARK (b), 1);

	bookmark_a = DORY_BOOKMARK (a);
	bookmark_b = DORY_BOOKMARK (b);

	return !g_file_equal (bookmark_a->details->location,
			      bookmark_b->details->location);
}

DoryBookmark *
dory_bookmark_copy (DoryBookmark *bookmark)
{
	g_return_val_if_fail (DORY_IS_BOOKMARK (bookmark), NULL);

    return dory_bookmark_new (bookmark->details->location,
                              bookmark->details->has_custom_name ?
                                  bookmark->details->name : NULL,
                              bookmark->details->icon_name,
                              bookmark->details->metadata ?
                                  dory_bookmark_metadata_copy (bookmark->details->metadata) : NULL);
}

gchar *
dory_bookmark_get_icon_name (DoryBookmark *bookmark)
{
	g_return_val_if_fail (DORY_IS_BOOKMARK (bookmark), NULL);

	/* Try to connect a file in case file exists now but didn't earlier. */
	dory_bookmark_connect_file (bookmark);

	if (bookmark->details->icon_name) {
		return g_strdup (bookmark->details->icon_name);
	}
	return NULL;
}

GFile *
dory_bookmark_get_location (DoryBookmark *bookmark)
{
	g_return_val_if_fail(DORY_IS_BOOKMARK (bookmark), NULL);

	/* Try to connect a file in case file exists now but didn't earlier.
	 * This allows a bookmark to update its image properly in the case
	 * where a new file appears with the same URI as a previously-deleted
	 * file. Calling connect_file here means that attempts to activate the
	 * bookmark will update its image if possible.
	 */
	dory_bookmark_connect_file (bookmark);

	return g_object_ref (bookmark->details->location);
}

char *
dory_bookmark_get_uri (DoryBookmark *bookmark)
{
	GFile *file;
	char *uri;

	file = dory_bookmark_get_location (bookmark);
	uri = g_file_get_uri (file);
	g_object_unref (file);
	return uri;
}

DoryBookmark *
dory_bookmark_new (GFile                *location,
                   const gchar          *custom_name,
                   const gchar          *icon_name,
                   DoryBookmarkMetadata *md)
{
	DoryBookmark *new_bookmark;
    gchar *name;

    if (custom_name == NULL)
        name = g_file_get_basename (location);
    else
        name = g_strdup (custom_name);

    new_bookmark = DORY_BOOKMARK (g_object_new (DORY_TYPE_BOOKMARK,
						"location", location,
						"icon-name", icon_name,
						"name", name,
						"custom-name", custom_name != NULL,
						"metadata", md,
						NULL));
    g_free (name);

    return new_bookmark;
}

static GtkWidget *
create_image_widget_for_bookmark (DoryBookmark *bookmark)
{
    GtkWidget *widget;
    gchar *icon_name;

    icon_name = dory_bookmark_get_icon_name (bookmark);

    widget = gtk_image_new_from_icon_name (icon_name, GTK_ICON_SIZE_MENU);

    g_free (icon_name);

	return widget;
}

/**
 * dory_bookmark_menu_item_new:
 *
 * Return a menu item representing a bookmark.
 * @bookmark: The bookmark the menu item represents.
 * Return value: A newly-created bookmark, not yet shown.
 **/
GtkWidget *
dory_bookmark_menu_item_new (DoryBookmark *bookmark)
{
	GtkWidget *menu_item;
	GtkWidget *image_widget;
	GtkLabel *label;
	const char *name;

	name = dory_bookmark_get_name (bookmark);
	menu_item = gtk_image_menu_item_new_with_label (name);
	label = GTK_LABEL (gtk_bin_get_child (GTK_BIN (menu_item)));
	gtk_label_set_use_underline (label, FALSE);
	gtk_label_set_ellipsize (label, PANGO_ELLIPSIZE_END);
	gtk_label_set_max_width_chars (label, ELLIPSISED_MENU_ITEM_MIN_CHARS);

	image_widget = create_image_widget_for_bookmark (bookmark);
	if (image_widget != NULL) {
		gtk_widget_show (image_widget);
		gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (menu_item),
					       image_widget);
	}

	return menu_item;
}

gboolean
dory_bookmark_uri_get_exists (DoryBookmark *bookmark)
{
	char *path_name;
	gboolean exists = FALSE;

    path_name = g_file_get_path (bookmark->details->location);

    if (g_file_is_native (bookmark->details->location) && 
        (!dory_location_is_network_safe (bookmark->details->location)) && g_file_test (path_name, G_FILE_TEST_EXISTS)) {
		exists = TRUE;
	} else {
        g_signal_emit_by_name (bookmark, "location-mounted", bookmark->details->location, &exists);
    }

	g_free (path_name);

	return exists;
}

void
dory_bookmark_set_scroll_pos (DoryBookmark      *bookmark,
				  const char            *uri)
{
	g_free (bookmark->details->scroll_file);
	bookmark->details->scroll_file = g_strdup (uri);
}

char *
dory_bookmark_get_scroll_pos (DoryBookmark      *bookmark)
{
	return g_strdup (bookmark->details->scroll_file);
}

void
dory_bookmark_connect (DoryBookmark *bookmark)
{
    dory_bookmark_connect_file (bookmark);
}

static gchar **
char_list_to_strv (GList *list)
{
    GList *iter;

    GPtrArray *array = g_ptr_array_new ();

    for (iter = list; iter != NULL; iter = iter->next) {
        g_ptr_array_add (array, g_strdup (iter->data));
    }

    g_ptr_array_add (array, NULL);

    return (char **) g_ptr_array_free (array, FALSE);
}

DoryBookmarkMetadata *
dory_bookmark_get_updated_metadata (DoryBookmark  *bookmark)
{
    DoryBookmarkMetadata *ret = NULL;

    if (!bookmark->details->file)
        return NULL;

    if (bookmark->details->file && !dory_file_is_gone (bookmark->details->file)) {
        GList *custom_emblems = NULL;

        custom_emblems = dory_file_get_metadata_list (bookmark->details->file, DORY_METADATA_KEY_EMBLEMS);

        if (custom_emblems) {
            ret = dory_bookmark_metadata_new ();
            ret->emblems = char_list_to_strv (custom_emblems);

            g_list_free_full (custom_emblems, g_free);
        }

    } else if (bookmark->details->metadata) {
        ret = dory_bookmark_metadata_copy (bookmark->details->metadata);
    }

    return ret;
}

DoryBookmarkMetadata *
dory_bookmark_get_current_metadata (DoryBookmark *bookmark)
{
    if (bookmark->details->metadata)
        return bookmark->details->metadata;

    return NULL;
}

DoryBookmarkMetadata *
dory_bookmark_metadata_new (void)
{
    DoryBookmarkMetadata *meta = g_new0 (DoryBookmarkMetadata, 1);

    return meta;
}

DoryBookmarkMetadata *
dory_bookmark_metadata_copy (DoryBookmarkMetadata *meta)
{
    DoryBookmarkMetadata *copy = dory_bookmark_metadata_new ();

    copy->bookmark_name = g_strdup (meta->bookmark_name);
    copy->emblems = g_strdupv (meta->emblems);

    return copy;
}

gboolean
dory_bookmark_metadata_compare (DoryBookmarkMetadata *d1,
                                DoryBookmarkMetadata *d2)
{
    if (g_strcmp0 (d1->bookmark_name, d2->bookmark_name) != 0 ||
        (g_strv_length (d1->emblems) != g_strv_length (d2->emblems)))
        return FALSE;

    guint i;

    for (i = 0; i < g_strv_length (d1->emblems); i++) {
        if (g_strcmp0 (d1->emblems[i], d2->emblems[i]) != 0)
            return FALSE;
    }

    return TRUE;
}

void
dory_bookmark_metadata_free (DoryBookmarkMetadata *metadata)
{
    if (metadata == NULL) {
        return;
    }

    g_free (metadata->bookmark_name);
    g_strfreev (metadata->emblems);

    g_free (metadata);
}
