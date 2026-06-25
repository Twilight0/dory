/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   dory-desktop-directory.h: Subclass of DoryDirectory to implement
   a virtual directory consisting of the desktop directory and the desktop
   icons
 
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

#ifndef DORY_DESKTOP_DIRECTORY_H
#define DORY_DESKTOP_DIRECTORY_H

#include <libdory-private/dory-directory.h>

#define DORY_TYPE_DESKTOP_DIRECTORY dory_desktop_directory_get_type()
#define DORY_DESKTOP_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_DESKTOP_DIRECTORY, DoryDesktopDirectory))
#define DORY_DESKTOP_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_DESKTOP_DIRECTORY, DoryDesktopDirectoryClass))
#define DORY_IS_DESKTOP_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_DESKTOP_DIRECTORY))
#define DORY_IS_DESKTOP_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_DESKTOP_DIRECTORY))
#define DORY_DESKTOP_DIRECTORY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_DESKTOP_DIRECTORY, DoryDesktopDirectoryClass))

typedef struct DoryDesktopDirectoryDetails DoryDesktopDirectoryDetails;

typedef struct {
	DoryDirectory parent_slot;
	DoryDesktopDirectoryDetails *details;
    gint display_number;
} DoryDesktopDirectory;

typedef struct {
	DoryDirectoryClass parent_slot;

} DoryDesktopDirectoryClass;

GType   dory_desktop_directory_get_type             (void);
DoryDirectory * dory_desktop_directory_get_real_directory   (DoryDesktopDirectory *desktop_directory);

#endif /* DORY_DESKTOP_DIRECTORY_H */
