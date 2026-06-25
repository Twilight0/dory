/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* fm-icon-view.h - interface for icon view of directory.

   Copyright (C) 2000 Eazel, Inc.

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

   Authors: Mike Engber <engber@eazel.com>
*/

#ifndef DORY_DESKTOP_ICON_VIEW_H
#define DORY_DESKTOP_ICON_VIEW_H

#include "dory-icon-view.h"

#define DORY_TYPE_DESKTOP_ICON_VIEW dory_desktop_icon_view_get_type()
#define DORY_DESKTOP_ICON_VIEW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_DESKTOP_ICON_VIEW, DoryDesktopIconView))
#define DORY_DESKTOP_ICON_VIEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_DESKTOP_ICON_VIEW, DoryDesktopIconViewClass))
#define DORY_IS_DESKTOP_ICON_VIEW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_DESKTOP_ICON_VIEW))
#define DORY_IS_DESKTOP_ICON_VIEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_DESKTOP_ICON_VIEW))
#define DORY_DESKTOP_ICON_VIEW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_DESKTOP_ICON_VIEW, DoryDesktopIconViewClass))

#define DORY_DESKTOP_ICON_VIEW_ID "OAFIID:Dory_File_Manager_Desktop_Icon_View"

typedef struct DoryDesktopIconViewDetails DoryDesktopIconViewDetails;
typedef struct {
	DoryIconView parent;
	DoryDesktopIconViewDetails *details;
} DoryDesktopIconView;

typedef struct {
	DoryIconViewClass parent_class;
} DoryDesktopIconViewClass;

/* GObject support */
GType   dory_desktop_icon_view_get_type (void);
void dory_desktop_icon_view_register (void);

#endif /* DORY_DESKTOP_ICON_VIEW_H */
