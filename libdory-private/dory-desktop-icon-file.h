/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   dory-desktop-file.h: Subclass of DoryFile to implement the
   the case of a desktop icon file
 
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

#ifndef DORY_DESKTOP_ICON_FILE_H
#define DORY_DESKTOP_ICON_FILE_H

#include <libdory-private/dory-file.h>
#include <libdory-private/dory-desktop-link.h>

#define DORY_TYPE_DESKTOP_ICON_FILE dory_desktop_icon_file_get_type()
#define DORY_DESKTOP_ICON_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_DESKTOP_ICON_FILE, DoryDesktopIconFile))
#define DORY_DESKTOP_ICON_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_DESKTOP_ICON_FILE, DoryDesktopIconFileClass))
#define DORY_IS_DESKTOP_ICON_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_DESKTOP_ICON_FILE))
#define DORY_IS_DESKTOP_ICON_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_DESKTOP_ICON_FILE))
#define DORY_DESKTOP_ICON_FILE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_DESKTOP_ICON_FILE, DoryDesktopIconFileClass))

typedef struct DoryDesktopIconFileDetails DoryDesktopIconFileDetails;

typedef struct {
	DoryFile parent_slot;
	DoryDesktopIconFileDetails *details;
} DoryDesktopIconFile;

typedef struct {
	DoryFileClass parent_slot;
} DoryDesktopIconFileClass;

GType   dory_desktop_icon_file_get_type (void);

DoryDesktopIconFile *dory_desktop_icon_file_new      (DoryDesktopLink     *link);
void                     dory_desktop_icon_file_update   (DoryDesktopIconFile *icon_file);
void                     dory_desktop_icon_file_remove   (DoryDesktopIconFile *icon_file);
DoryDesktopLink     *dory_desktop_icon_file_get_link (DoryDesktopIconFile *icon_file);

#endif /* DORY_DESKTOP_ICON_FILE_H */
