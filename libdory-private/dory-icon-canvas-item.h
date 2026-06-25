/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* Dory - Icon canvas item class for icon container.
 *
 * Copyright (C) 2000 Eazel, Inc.
 *
 * Author: Andy Hertzfeld <andy@eazel.com>
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
 */

#ifndef DORY_ICON_CANVAS_ITEM_H
#define DORY_ICON_CANVAS_ITEM_H

#include <eel/eel-canvas.h>
#include <eel/eel-art-extensions.h>

G_BEGIN_DECLS

#define DORY_TYPE_ICON_CANVAS_ITEM dory_icon_canvas_item_get_type()
#define DORY_ICON_CANVAS_ITEM(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_ICON_CANVAS_ITEM, DoryIconCanvasItem))
#define DORY_ICON_CANVAS_ITEM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_ICON_CANVAS_ITEM, DoryIconCanvasItemClass))
#define DORY_IS_ICON_CANVAS_ITEM(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_ICON_CANVAS_ITEM))
#define DORY_IS_ICON_CANVAS_ITEM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_ICON_CANVAS_ITEM))
#define DORY_ICON_CANVAS_ITEM_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_ICON_CANVAS_ITEM, DoryIconCanvasItemClass))

typedef struct DoryIconCanvasItem DoryIconCanvasItem;
typedef struct DoryIconCanvasItemClass DoryIconCanvasItemClass;
typedef struct DoryIconCanvasItemDetails DoryIconCanvasItemDetails;

struct DoryIconCanvasItem {
	EelCanvasItem item;
	DoryIconCanvasItemDetails *details;
	gpointer user_data;
};

struct DoryIconCanvasItemClass {
	EelCanvasItemClass parent_class;
};

/* not namespaced due to their length */
typedef enum {
	BOUNDS_USAGE_FOR_LAYOUT,
	BOUNDS_USAGE_FOR_ENTIRE_ITEM,
	BOUNDS_USAGE_FOR_DISPLAY
} DoryIconCanvasItemBoundsUsage;

/* GObject */
GType       dory_icon_canvas_item_get_type                 (void);

/* attributes */
void        dory_icon_canvas_item_set_image                (DoryIconCanvasItem       *item,
								GdkPixbuf                    *image);
cairo_surface_t* dory_icon_canvas_item_get_drag_surface    (DoryIconCanvasItem       *item);
void        dory_icon_canvas_item_set_emblems              (DoryIconCanvasItem       *item,
								GList                        *emblem_pixbufs);
void        dory_icon_canvas_item_set_show_stretch_handles (DoryIconCanvasItem       *item,
								gboolean                      show_stretch_handles);
double      dory_icon_canvas_item_get_max_text_width       (DoryIconCanvasItem       *item);
const char *dory_icon_canvas_item_get_editable_text        (DoryIconCanvasItem       *icon_item);
void        dory_icon_canvas_item_set_renaming             (DoryIconCanvasItem       *icon_item,
								gboolean                      state);

/* geometry and hit testing */
gboolean    dory_icon_canvas_item_hit_test_rectangle       (DoryIconCanvasItem       *item,
								EelIRect                      canvas_rect);
gboolean    dory_icon_canvas_item_hit_test_stretch_handles (DoryIconCanvasItem       *item,
								gdouble                       world_x,
								gdouble                       world_y,
								GtkCornerType                *corner);
void        dory_icon_canvas_item_invalidate_label         (DoryIconCanvasItem       *item);
void        dory_icon_canvas_item_invalidate_label_size    (DoryIconCanvasItem       *item);
EelDRect    dory_icon_canvas_item_get_icon_rectangle       (const DoryIconCanvasItem *item);
EelDRect    dory_icon_canvas_item_get_text_rectangle       (DoryIconCanvasItem       *item,
								gboolean                      for_layout);
void        dory_icon_canvas_item_get_icon_canvas_rectangle (DoryIconCanvasItem *item,
                                                             EelIRect *rect);
void        dory_icon_canvas_item_get_bounds_for_layout    (DoryIconCanvasItem       *item,
								double *x1, double *y1, double *x2, double *y2);
void        dory_icon_canvas_item_get_bounds_for_entire_item (DoryIconCanvasItem       *item,
								  double *x1, double *y1, double *x2, double *y2);
void        dory_icon_canvas_item_update_bounds            (DoryIconCanvasItem       *item,
								double i2w_dx, double i2w_dy);
void        dory_icon_canvas_item_set_is_visible           (DoryIconCanvasItem       *item,
								gboolean                      visible);
/* whether the entire label text must be visible at all times */
void        dory_icon_canvas_item_set_entire_text          (DoryIconCanvasItem       *icon_item,
								gboolean                      entire_text);
gint        dory_icon_canvas_item_get_fixed_text_height_for_layout (DoryIconCanvasItem *item);
G_END_DECLS

#endif /* DORY_ICON_CANVAS_ITEM_H */
