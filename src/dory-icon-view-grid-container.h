/* -*- Mode: C; indent-tabs-mode: f; c-basic-offset: 4; tab-width: 4 -*- */

/* fm-icon-container.h - the container widget for file manager icons

   Copyright (C) 2002 Sun Microsystems, Inc.

   The Gnome Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gnome Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
   Boston, MA 02110-1335, USA.

   Author: Michael Meeks <michael@ximian.com>
*/

#ifndef DORY_ICON_VIEW_GRID_CONTAINER_H
#define DORY_ICON_VIEW_GRID_CONTAINER_H

#include "dory-icon-view.h"

#include <libdory-private/dory-icon-private.h>

typedef struct DoryIconViewGridContainer DoryIconViewGridContainer;
typedef struct DoryIconViewGridContainerClass DoryIconViewGridContainerClass;

#define DORY_TYPE_ICON_VIEW_GRID_CONTAINER dory_icon_view_grid_container_get_type()
#define DORY_ICON_VIEW_GRID_CONTAINER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_ICON_VIEW_GRID_CONTAINER, DoryIconViewGridContainer))
#define DORY_ICON_VIEW_GRID_CONTAINER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_ICON_VIEW_GRID_CONTAINER, DoryIconViewGridContainerClass))
#define DORY_IS_ICON_VIEW_GRID_CONTAINER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_ICON_VIEW_GRID_CONTAINER))
#define DORY_IS_ICON_VIEW_GRID_CONTAINER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_ICON_VIEW_GRID_CONTAINER))
#define DORY_ICON_VIEW_GRID_CONTAINER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_ICON_VIEW_GRID_CONTAINER, DoryIconViewGridContainerClass))

typedef struct DoryIconViewGridContainerDetails DoryIconViewGridContainerDetails;

struct DoryIconViewGridContainer {
	DoryIconContainer parent;

	DoryIconView *view;
	gboolean      sort_for_desktop;
    gboolean      horizontal;
    gboolean      manual_sort_dirty;
    gint          text_ellipsis_limit;

    GQuark       *attributes;
};

struct DoryIconViewGridContainerClass {
	DoryIconContainerClass parent_class;
};

GType                  dory_icon_view_grid_container_get_type         (void);
DoryIconContainer *dory_icon_view_grid_container_construct        (DoryIconViewGridContainer *icon_container,
                                                                   DoryIconView              *view,
                                                                   gboolean                   is_desktop);
DoryIconContainer *dory_icon_view_grid_container_new              (DoryIconView              *view,
                                                                   gboolean                   is_desktop);
void                   dory_icon_view_grid_container_set_sort_desktop (DoryIconViewGridContainer *container,
                                                                       gboolean                   desktop);

#endif /* DORY_ICON_VIEW_GRID_CONTAINER_H */
