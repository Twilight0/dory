/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Dory
 *
 * Dory is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Dory is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, MA 02110-1335, USA.
 *
 */

/* dory-blank-desktop-window.h
 */

#ifndef DORY_BLANK_DESKTOP_WINDOW_H
#define DORY_BLANK_DESKTOP_WINDOW_H

#include <gtk/gtk.h>

#include <libdory-private/dory-action-manager.h>

#define DORY_TYPE_BLANK_DESKTOP_WINDOW dory_blank_desktop_window_get_type()
#define DORY_BLANK_DESKTOP_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_BLANK_DESKTOP_WINDOW, DoryBlankDesktopWindow))
#define DORY_BLANK_DESKTOP_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_BLANK_DESKTOP_WINDOW, DoryBlankDesktopWindowClass))
#define DORY_IS_BLANK_DESKTOP_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_BLANK_DESKTOP_WINDOW))
#define DORY_IS_BLANK_DESKTOP_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_BLANK_DESKTOP_WINDOW))
#define DORY_BLANK_DESKTOP_WINDOW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_BLANK_DESKTOP_WINDOW, DoryBlankDesktopWindowClass))

typedef struct DoryBlankDesktopWindowDetails DoryBlankDesktopWindowDetails;

typedef struct {
	GtkWindow parent_spot;
	DoryBlankDesktopWindowDetails *details;
} DoryBlankDesktopWindow;

typedef struct {
	GtkWindowClass parent_spot;

    void   (* plugin_manager)  (DoryBlankDesktopWindow *window);
} DoryBlankDesktopWindowClass;

GType                   dory_blank_desktop_window_get_type            (void);
DoryBlankDesktopWindow *dory_blank_desktop_window_new                 (gint monitor);
DoryActionManager      *dory_desktop_manager_get_action_manager       (void);
void                    dory_blank_desktop_window_update_geometry     (DoryBlankDesktopWindow *window);

#endif /* DORY_BLANK_DESKTOP_WINDOW_H */
