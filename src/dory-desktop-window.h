/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Dory
 *
 * Copyright (C) 2000 Eazel, Inc.
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
 * Authors: Darin Adler <darin@bentspoon.com>
 */

/* dory-desktop-window.h
 */

#ifndef DORY_DESKTOP_WINDOW_H
#define DORY_DESKTOP_WINDOW_H

#include "dory-window.h"

#define DORY_TYPE_DESKTOP_WINDOW dory_desktop_window_get_type()
#define DORY_DESKTOP_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_DESKTOP_WINDOW, DoryDesktopWindow))
#define DORY_DESKTOP_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_DESKTOP_WINDOW, DoryDesktopWindowClass))
#define DORY_IS_DESKTOP_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_DESKTOP_WINDOW))
#define DORY_IS_DESKTOP_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_DESKTOP_WINDOW))
#define DORY_DESKTOP_WINDOW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_DESKTOP_WINDOW, DoryDesktopWindowClass))

typedef struct DoryDesktopWindowDetails DoryDesktopWindowDetails;

typedef struct {
	DoryWindow parent_spot;
	DoryDesktopWindowDetails *details;
} DoryDesktopWindow;

typedef struct {
	DoryWindowClass parent_spot;
} DoryDesktopWindowClass;

GType                  dory_desktop_window_get_type            (void);
DoryDesktopWindow     *dory_desktop_window_new                 (gint monitor);
gboolean               dory_desktop_window_loaded              (DoryDesktopWindow *window);
gint                   dory_desktop_window_get_monitor         (DoryDesktopWindow *window);
void                   dory_desktop_window_update_geometry     (DoryDesktopWindow *window);
gboolean               dory_desktop_window_get_grid_adjusts    (DoryDesktopWindow *window,
                                                                gint              *h_adjust,
                                                                gint              *v_adjust);
gboolean               dory_desktop_window_set_grid_adjusts    (DoryDesktopWindow *window,
                                                                gint               h_adjust,
                                                                gint               v_adjust);
GtkActionGroup *       dory_desktop_window_get_action_group (DoryDesktopWindow *window);
#endif /* DORY_DESKTOP_WINDOW_H */
