/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* 
   dory-trash-monitor.h: Dory trash state watcher.
 
   Copyright (C) 2000 Eazel, Inc.
  
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
  
   Author: Pavel Cisler <pavel@eazel.com>
*/

#ifndef DORY_TRASH_MONITOR_H
#define DORY_TRASH_MONITOR_H

#include <gtk/gtk.h>
#include <gio/gio.h>

typedef struct DoryTrashMonitor DoryTrashMonitor;
typedef struct DoryTrashMonitorClass DoryTrashMonitorClass;
typedef struct DoryTrashMonitorDetails DoryTrashMonitorDetails;

#define DORY_TYPE_TRASH_MONITOR dory_trash_monitor_get_type()
#define DORY_TRASH_MONITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_TRASH_MONITOR, DoryTrashMonitor))
#define DORY_TRASH_MONITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_TRASH_MONITOR, DoryTrashMonitorClass))
#define DORY_IS_TRASH_MONITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_TRASH_MONITOR))
#define DORY_IS_TRASH_MONITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_TRASH_MONITOR))
#define DORY_TRASH_MONITOR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_TRASH_MONITOR, DoryTrashMonitorClass))

struct DoryTrashMonitor {
	GObject object;
	DoryTrashMonitorDetails *details;
};

struct DoryTrashMonitorClass {
	GObjectClass parent_class;

	void (* trash_state_changed)		(DoryTrashMonitor 	*trash_monitor,
				      		 gboolean 		 new_state);
};

GType			dory_trash_monitor_get_type				(void);

DoryTrashMonitor   *dory_trash_monitor_get 				(void);
gboolean		dory_trash_monitor_is_empty 			(void);
GIcon                  *dory_trash_monitor_get_icon                         (void);
gchar                  *dory_trash_monitor_get_symbolic_icon_name           (void);

void		        dory_trash_monitor_add_new_trash_directories        (void);

#endif
