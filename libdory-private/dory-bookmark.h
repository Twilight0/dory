/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* dory-bookmark.h - implementation of individual bookmarks.
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

#ifndef DORY_BOOKMARK_H
#define DORY_BOOKMARK_H

#include <gtk/gtk.h>
#include <gio/gio.h>
typedef struct DoryBookmark DoryBookmark;

#define DORY_TYPE_BOOKMARK dory_bookmark_get_type()
#define DORY_BOOKMARK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_BOOKMARK, DoryBookmark))
#define DORY_BOOKMARK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_BOOKMARK, DoryBookmarkClass))
#define DORY_IS_BOOKMARK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_BOOKMARK))
#define DORY_IS_BOOKMARK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_BOOKMARK))
#define DORY_BOOKMARK_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_BOOKMARK, DoryBookmarkClass))

typedef struct DoryBookmarkDetails DoryBookmarkDetails;

struct DoryBookmark {
	GObject object;
	DoryBookmarkDetails *details;	
};

typedef struct
{
  gchar  *bookmark_name;
  gchar **emblems;
} DoryBookmarkMetadata;

struct DoryBookmarkClass {
	GObjectClass parent_class;

	/* Signals that clients can connect to. */

	/* The contents-changed signal is emitted when the bookmark's contents
	 * (custom name or URI) changed.
	 */
	void	(* contents_changed) (DoryBookmark *bookmark);

    gboolean (* location_mounted)         (GFile *location);
};

typedef struct DoryBookmarkClass DoryBookmarkClass;

GType                 dory_bookmark_get_type               (void);
DoryBookmark *    dory_bookmark_new                    (GFile                *location,
                                                        const char           *custom_name,
                                                        const char           *icon_name,
                                                        DoryBookmarkMetadata *md);
DoryBookmark *    dory_bookmark_copy                   (DoryBookmark      *bookmark);
const char *          dory_bookmark_get_name               (DoryBookmark      *bookmark);
GFile *               dory_bookmark_get_location           (DoryBookmark      *bookmark);
char *                dory_bookmark_get_uri                (DoryBookmark      *bookmark);
gchar *               dory_bookmark_get_icon_name          (DoryBookmark      *bookmark);
gboolean	      dory_bookmark_get_has_custom_name    (DoryBookmark      *bookmark);		
void                  dory_bookmark_set_custom_name        (DoryBookmark      *bookmark,
								const char            *new_name);		
gboolean              dory_bookmark_uri_get_exists         (DoryBookmark      *bookmark);
int                   dory_bookmark_compare_with           (gconstpointer          a,
								gconstpointer          b);
int                   dory_bookmark_compare_uris           (gconstpointer          a,
								gconstpointer          b);

void                  dory_bookmark_set_scroll_pos         (DoryBookmark      *bookmark,
								const char            *uri);
char *                dory_bookmark_get_scroll_pos         (DoryBookmark      *bookmark);


/* Helper functions for displaying bookmarks */
GtkWidget *           dory_bookmark_menu_item_new          (DoryBookmark      *bookmark);

void                  dory_bookmark_connect                (DoryBookmark *bookmark);

/* Bookmark metadata struct functions */

DoryBookmarkMetadata *dory_bookmark_get_updated_metadata   (DoryBookmark  *bookmark);
DoryBookmarkMetadata *dory_bookmark_get_current_metadata   (DoryBookmark  *bookmark);
gboolean              dory_bookmark_metadata_compare       (DoryBookmarkMetadata *d1,
                                                            DoryBookmarkMetadata *d2);
DoryBookmarkMetadata *dory_bookmark_metadata_new           (void);
DoryBookmarkMetadata *dory_bookmark_metadata_copy          (DoryBookmarkMetadata *meta);
void                  dory_bookmark_metadata_free          (DoryBookmarkMetadata *metadata);

#endif /* DORY_BOOKMARK_H */
