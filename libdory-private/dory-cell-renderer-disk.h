/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-
 
   Copyright (C) 2007 Martin Wehner
  
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

   Author: Martin Wehner <martin.wehner@gmail.com>
*/

#ifndef DORY_CELL_RENDERER_DISK_H
#define DORY_CELL_RENDERER_DISK_H

#include <gtk/gtk.h>

#define DORY_TYPE_CELL_RENDERER_DISK dory_cell_renderer_disk_get_type()
#define DORY_CELL_RENDERER_DISK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_CELL_RENDERER_DISK, DoryCellRendererDisk))
#define DORY_CELL_RENDERER_DISK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_CELL_RENDERER_DISK, DoryCellRendererDiskClass))
#define DORY_IS_CELL_RENDERER_DISK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_CELL_RENDERER_DISK))
#define DORY_IS_CELL_RENDERER_DISK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_CELL_RENDERER_DISK))
#define DORY_CELL_RENDERER_DISK_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_CELL_RENDERER_DISK, DoryCellRendererDiskClass))


typedef struct _DoryCellRendererDisk DoryCellRendererDisk;
typedef struct _DoryCellRendererDiskClass DoryCellRendererDiskClass;

struct _DoryCellRendererDisk {
	GtkCellRendererText parent;
    gint disk_full_percent;
    gboolean show_disk_full_percent;
    GtkTextDirection direction;
};

struct _DoryCellRendererDiskClass {
	GtkCellRendererTextClass parent_class;
};

GType		 dory_cell_renderer_disk_get_type (void);
GtkCellRenderer *dory_cell_renderer_disk_new      (void);

#endif /* DORY_CELL_RENDERER_DISK_H */
