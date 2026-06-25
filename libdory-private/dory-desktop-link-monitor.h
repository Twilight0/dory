/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   dory-desktop-link-monitor.h: singleton that manages the desktop links
    
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

#ifndef DORY_DESKTOP_LINK_MONITOR_H
#define DORY_DESKTOP_LINK_MONITOR_H

#include <gtk/gtk.h>
#include <libdory-private/dory-desktop-link.h>

#define DORY_TYPE_DESKTOP_LINK_MONITOR dory_desktop_link_monitor_get_type()
#define DORY_DESKTOP_LINK_MONITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_DESKTOP_LINK_MONITOR, DoryDesktopLinkMonitor))
#define DORY_DESKTOP_LINK_MONITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_DESKTOP_LINK_MONITOR, DoryDesktopLinkMonitorClass))
#define DORY_IS_DESKTOP_LINK_MONITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_DESKTOP_LINK_MONITOR))
#define DORY_IS_DESKTOP_LINK_MONITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_DESKTOP_LINK_MONITOR))
#define DORY_DESKTOP_LINK_MONITOR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_DESKTOP_LINK_MONITOR, DoryDesktopLinkMonitorClass))

typedef struct DoryDesktopLinkMonitorDetails DoryDesktopLinkMonitorDetails;

typedef struct {
	GObject parent_slot;
	DoryDesktopLinkMonitorDetails *details;
} DoryDesktopLinkMonitor;

typedef struct {
	GObjectClass parent_slot;
} DoryDesktopLinkMonitorClass;

GType   dory_desktop_link_monitor_get_type (void);

DoryDesktopLinkMonitor *   dory_desktop_link_monitor_get (void);
void dory_desktop_link_monitor_delete_link (DoryDesktopLinkMonitor *monitor,
						DoryDesktopLink *link,
						GtkWidget *parent_view);

/* Used by dory-desktop-link.c */
char * dory_desktop_link_monitor_make_filename_unique (DoryDesktopLinkMonitor *monitor,
							   const char *filename);

#endif /* DORY_DESKTOP_LINK_MONITOR_H */
