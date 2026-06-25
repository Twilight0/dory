/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   dory-clipboard-monitor.h: lets you notice clipboard changes.
    
   Copyright (C) 2004 Red Hat, Inc.
  
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

#ifndef DORY_CLIPBOARD_MONITOR_H
#define DORY_CLIPBOARD_MONITOR_H

#include <gtk/gtk.h>

#define DORY_TYPE_CLIPBOARD_MONITOR dory_clipboard_monitor_get_type()
#define DORY_CLIPBOARD_MONITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_CLIPBOARD_MONITOR, DoryClipboardMonitor))
#define DORY_CLIPBOARD_MONITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_CLIPBOARD_MONITOR, DoryClipboardMonitorClass))
#define DORY_IS_CLIPBOARD_MONITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_CLIPBOARD_MONITOR))
#define DORY_IS_CLIPBOARD_MONITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_CLIPBOARD_MONITOR))
#define DORY_CLIPBOARD_MONITOR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_CLIPBOARD_MONITOR, DoryClipboardMonitorClass))

typedef struct DoryClipboardMonitorDetails DoryClipboardMonitorDetails;
typedef struct DoryClipboardInfo DoryClipboardInfo;

typedef struct {
	GObject parent_slot;

	DoryClipboardMonitorDetails *details;
} DoryClipboardMonitor;

typedef struct {
	GObjectClass parent_slot;
  
	void (* clipboard_changed) (DoryClipboardMonitor *monitor);
	void (* clipboard_info) (DoryClipboardMonitor *monitor,
	                         DoryClipboardInfo *info);
} DoryClipboardMonitorClass;

struct DoryClipboardInfo {
	GList *files;
	gboolean cut;
};

GType   dory_clipboard_monitor_get_type (void);

DoryClipboardMonitor *   dory_clipboard_monitor_get (void);
void dory_clipboard_monitor_set_clipboard_info (DoryClipboardMonitor *monitor,
                                                    DoryClipboardInfo *info);
DoryClipboardInfo * dory_clipboard_monitor_get_clipboard_info (DoryClipboardMonitor *monitor);
void dory_clipboard_monitor_emit_changed (void);

void dory_clear_clipboard_callback (GtkClipboard *clipboard,
                                        gpointer      user_data);
void dory_get_clipboard_callback   (GtkClipboard     *clipboard,
                                        GtkSelectionData *selection_data,
                                        guint             info,
                                        gpointer          user_data);



#endif /* DORY_CLIPBOARD_MONITOR_H */

