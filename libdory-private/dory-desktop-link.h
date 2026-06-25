/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   dory-desktop-link.h: Class that handles the links on the desktop
    
   Copyright (C) 2003 Red Hat, Inc.
  
   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
  
   You should have received a copy of the GNU General Public
   License along with this program; if not, write to the
   Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
   Boston, MA 02110-1335, USA.
  
   Author: Alexander Larsson <alexl@redhat.com>
*/

#ifndef DORY_DESKTOP_LINK_H
#define DORY_DESKTOP_LINK_H

#include <libdory-private/dory-file.h>
#include <gio/gio.h>

#define DORY_TYPE_DESKTOP_LINK dory_desktop_link_get_type()
#define DORY_DESKTOP_LINK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_DESKTOP_LINK, DoryDesktopLink))
#define DORY_DESKTOP_LINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_DESKTOP_LINK, DoryDesktopLinkClass))
#define DORY_IS_DESKTOP_LINK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_DESKTOP_LINK))
#define DORY_IS_DESKTOP_LINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_DESKTOP_LINK))
#define DORY_DESKTOP_LINK_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_DESKTOP_LINK, DoryDesktopLinkClass))

typedef struct DoryDesktopLinkDetails DoryDesktopLinkDetails;

typedef struct {
	GObject parent_slot;
	DoryDesktopLinkDetails *details;
} DoryDesktopLink;

typedef struct {
	GObjectClass parent_slot;
} DoryDesktopLinkClass;

typedef enum {
	DORY_DESKTOP_LINK_HOME,
	DORY_DESKTOP_LINK_COMPUTER,
	DORY_DESKTOP_LINK_TRASH,
	DORY_DESKTOP_LINK_MOUNT,
	DORY_DESKTOP_LINK_NETWORK
} DoryDesktopLinkType;

GType   dory_desktop_link_get_type (void);

DoryDesktopLink *   dory_desktop_link_new                     (DoryDesktopLinkType  type);
DoryDesktopLink *   dory_desktop_link_new_from_mount          (GMount                 *mount);
DoryDesktopLinkType dory_desktop_link_get_link_type           (DoryDesktopLink     *link);
DoryFile *          dory_desktop_link_get_file                (DoryDesktopLink *link);
char *                  dory_desktop_link_get_file_name           (DoryDesktopLink     *link);
char *                  dory_desktop_link_get_display_name        (DoryDesktopLink     *link);
GIcon *                 dory_desktop_link_get_icon                (DoryDesktopLink     *link);
GFile *                 dory_desktop_link_get_activation_location (DoryDesktopLink     *link);
char *                  dory_desktop_link_get_activation_uri      (DoryDesktopLink     *link);
gboolean                dory_desktop_link_get_date                (DoryDesktopLink     *link,
								       DoryDateType         date_type,
								       time_t                  *date);
GMount *                dory_desktop_link_get_mount               (DoryDesktopLink     *link);
gboolean                dory_desktop_link_can_rename              (DoryDesktopLink     *link);
gboolean                dory_desktop_link_rename                  (DoryDesktopLink     *link,
								       const char              *name);


#endif /* DORY_DESKTOP_LINK_H */
