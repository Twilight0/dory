/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

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

#ifndef DORY_ICON_VIEW_CONTAINER_H
#define DORY_ICON_VIEW_CONTAINER_H

#include "dory-icon-view.h"

#include <libdory-private/dory-icon-private.h>

typedef struct DoryIconViewContainer DoryIconViewContainer;
typedef struct DoryIconViewContainerClass DoryIconViewContainerClass;

#define DORY_TYPE_ICON_VIEW_CONTAINER dory_icon_view_container_get_type()
#define DORY_ICON_VIEW_CONTAINER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_ICON_VIEW_CONTAINER, DoryIconViewContainer))
#define DORY_ICON_VIEW_CONTAINER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_ICON_VIEW_CONTAINER, DoryIconViewContainerClass))
#define DORY_IS_ICON_VIEW_CONTAINER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_ICON_VIEW_CONTAINER))
#define DORY_IS_ICON_VIEW_CONTAINER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_ICON_VIEW_CONTAINER))
#define DORY_ICON_VIEW_CONTAINER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_ICON_VIEW_CONTAINER, DoryIconViewContainerClass))

typedef struct DoryIconViewContainerDetails DoryIconViewContainerDetails;

struct DoryIconViewContainer {
	DoryIconContainer parent;

	DoryIconView *view;
	gboolean    sort_for_desktop;
};

struct DoryIconViewContainerClass {
	DoryIconContainerClass parent_class;
};

GType                  dory_icon_view_container_get_type         (void);
DoryIconContainer *dory_icon_view_container_construct        (DoryIconViewContainer *icon_container,
                                                              DoryIconView          *view,
                                                              gboolean               is_desktop);
DoryIconContainer *dory_icon_view_container_new              (DoryIconView          *view,
                                                              gboolean               is_desktop);
void                   dory_icon_view_container_set_sort_desktop (DoryIconViewContainer *container,
								      gboolean         desktop);

#endif /* DORY_ICON_VIEW_CONTAINER_H */
