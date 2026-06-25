/*
 * dory-previewer: dory previewer DBus wrapper
 *
 * Copyright (C) 2011, Red Hat, Inc.
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
 * Author: Cosimo Cecchi <cosimoc@redhat.com>
 *
 */

#ifndef __DORY_PREVIEWER_H__
#define __DORY_PREVIEWER_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define DORY_TYPE_PREVIEWER dory_previewer_get_type()
#define DORY_PREVIEWER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_PREVIEWER, DoryPreviewer))
#define DORY_PREVIEWER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_PREVIEWER, DoryPreviewerClass))
#define DORY_IS_PREVIEWER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_PREVIEWER))
#define DORY_IS_PREVIEWER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_PREVIEWER))
#define DORY_PREVIEWER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_PREVIEWER, DoryPreviewerClass))

typedef struct _DoryPreviewerPriv DoryPreviewerPriv;

typedef struct {
  GObject parent;

  /* private */
  DoryPreviewerPriv *priv;
} DoryPreviewer;

typedef struct {
  GObjectClass parent_class;
} DoryPreviewerClass;

GType dory_previewer_get_type (void);

DoryPreviewer *dory_previewer_get_singleton (void);
void dory_previewer_call_show_file (DoryPreviewer *previewer,
                                        const gchar *uri,
                                        guint xid,
					gboolean close_if_already_visible);
void dory_previewer_call_close (DoryPreviewer *previewer);

G_END_DECLS

#endif /* __DORY_PREVIEWER_H__ */
