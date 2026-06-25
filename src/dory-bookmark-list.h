/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Dory
 *
 * Copyright (C) 1999, 2000 Eazel, Inc.
 *
 * Dory is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Dory is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, MA 02110-1335, USA.
 *
 * Authors: John Sullivan <sullivan@eazel.com>
 */

/* dory-bookmark-list.h - interface for centralized list of bookmarks.
 */

#ifndef DORY_BOOKMARK_LIST_H
#define DORY_BOOKMARK_LIST_H

#include <libdory-private/dory-bookmark.h>
#include <gio/gio.h>

typedef struct DoryBookmarkList DoryBookmarkList;
typedef struct DoryBookmarkListClass DoryBookmarkListClass;

#define DORY_TYPE_BOOKMARK_LIST dory_bookmark_list_get_type()
#define DORY_BOOKMARK_LIST(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_BOOKMARK_LIST, DoryBookmarkList))
#define DORY_BOOKMARK_LIST_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_BOOKMARK_LIST, DoryBookmarkListClass))
#define DORY_IS_BOOKMARK_LIST(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_BOOKMARK_LIST))
#define DORY_IS_BOOKMARK_LIST_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_BOOKMARK_LIST))
#define DORY_BOOKMARK_LIST_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_BOOKMARK_LIST, DoryBookmarkListClass))

struct DoryBookmarkList {
	GObject object;

	GList *list; 
	GFileMonitor *monitor;
	GQueue *pending_ops;
    GVolumeMonitor *volume_monitor;

    guint idle_notify_id;
};

struct DoryBookmarkListClass {
	GObjectClass parent_class;
	void (* changed) (DoryBookmarkList *bookmarks);
};

GType                   dory_bookmark_list_get_type            (void);
DoryBookmarkList *  dory_bookmark_list_get_default                 (void);
void                    dory_bookmark_list_append              (DoryBookmarkList   *bookmarks,
								    DoryBookmark *bookmark);
gboolean                dory_bookmark_list_contains            (DoryBookmarkList   *bookmarks,
								    DoryBookmark *bookmark);
void                    dory_bookmark_list_delete_item_at      (DoryBookmarkList   *bookmarks,
								    guint                   index);
void                    dory_bookmark_list_delete_items_with_uri (DoryBookmarkList *bookmarks,
								    const char		   *uri);
void                    dory_bookmark_list_insert_item         (DoryBookmarkList   *bookmarks,
								    DoryBookmark *bookmark,
								    guint                   index);
GList *                 dory_bookmark_list_get_for_uri         (DoryBookmarkList   *bookmarks,
                                                                const char *uri);
guint                   dory_bookmark_list_length              (DoryBookmarkList   *bookmarks);
DoryBookmark *      dory_bookmark_list_item_at             (DoryBookmarkList   *bookmarks,
								    guint                   index);
void                    dory_bookmark_list_move_item           (DoryBookmarkList *bookmarks,
								    guint                 index,
								    guint                 destination);
void                    dory_bookmark_list_sort_ascending           (DoryBookmarkList *bookmarks);
void                    dory_bookmark_list_set_window_geometry (DoryBookmarkList   *bookmarks,
								    const char             *geometry);
const char *            dory_bookmark_list_get_window_geometry (DoryBookmarkList   *bookmarks);

#endif /* DORY_BOOKMARK_LIST_H */
