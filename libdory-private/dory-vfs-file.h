/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   dory-vfs-file.h: Subclass of DoryFile to implement the
   the case of a VFS file.
 
   Copyright (C) 1999, 2000 Eazel, Inc.
  
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
  
   Author: Darin Adler <darin@bentspoon.com>
*/

#ifndef DORY_VFS_FILE_H
#define DORY_VFS_FILE_H

#include <libdory-private/dory-file.h>

#define DORY_TYPE_VFS_FILE dory_vfs_file_get_type()
#define DORY_VFS_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_VFS_FILE, DoryVFSFile))
#define DORY_VFS_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_VFS_FILE, DoryVFSFileClass))
#define DORY_IS_VFS_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_VFS_FILE))
#define DORY_IS_VFS_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_VFS_FILE))
#define DORY_VFS_FILE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_VFS_FILE, DoryVFSFileClass))

typedef struct DoryVFSFileDetails DoryVFSFileDetails;

typedef struct {
	DoryFile parent_slot;
} DoryVFSFile;

typedef struct {
	DoryFileClass parent_slot;
} DoryVFSFileClass;

GType   dory_vfs_file_get_type (void);

#endif /* DORY_VFS_FILE_H */
