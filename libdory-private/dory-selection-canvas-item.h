/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* Dory - Canvas item for floating selection.
 *
 * Copyright (C) 1997, 1998, 1999, 2000 Free Software Foundation
 * Copyright (C) 2011 Red Hat Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
 * Boston, MA 02110-1335, USA.
 *
 * Authors: Federico Mena <federico@nuclecu.unam.mx>
 *          Cosimo Cecchi <cosimoc@redhat.com>
 */

#ifndef __DORY_SELECTION_CANVAS_ITEM_H__
#define __DORY_SELECTION_CANVAS_ITEM_H__

#include <eel/eel-canvas.h>

G_BEGIN_DECLS

#define DORY_TYPE_SELECTION_CANVAS_ITEM dory_selection_canvas_item_get_type()
#define DORY_SELECTION_CANVAS_ITEM(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_SELECTION_CANVAS_ITEM, DorySelectionCanvasItem))
#define DORY_SELECTION_CANVAS_ITEM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_SELECTION_CANVAS_ITEM, DorySelectionCanvasItemClass))
#define DORY_IS_SELECTION_CANVAS_ITEM(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_SELECTION_CANVAS_ITEM))
#define DORY_IS_SELECTION_CANVAS_ITEM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_SELECTION_CANVAS_ITEM))
#define DORY_SELECTION_CANVAS_ITEM_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_SELECTION_CANVAS_ITEM, DorySelectionCanvasItemClass))

typedef struct _DorySelectionCanvasItem DorySelectionCanvasItem;
typedef struct _DorySelectionCanvasItemClass DorySelectionCanvasItemClass;
typedef struct _DorySelectionCanvasItemDetails DorySelectionCanvasItemDetails;

struct _DorySelectionCanvasItem {
	EelCanvasItem item;
	DorySelectionCanvasItemDetails *priv;
	gpointer user_data;
};

struct _DorySelectionCanvasItemClass {
	EelCanvasItemClass parent_class;
};

/* GObject */
GType       dory_selection_canvas_item_get_type                 (void);

void dory_selection_canvas_item_fade_out (DorySelectionCanvasItem *self,
					      guint transition_time);

G_END_DECLS

#endif /* __DORY_SELECTION_CANVAS_ITEM_H__ */
