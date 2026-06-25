/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* fm-properties-window.h - interface for window that lets user modify 
                            icon properties

   Copyright (C) 2000 Eazel, Inc.

   The Gnome Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gnome Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
   Boston, MA 02110-1335, USA.

   Authors: Darin Adler <darin@bentspoon.com>
*/

#ifndef DORY_PROPERTIES_WINDOW_H
#define DORY_PROPERTIES_WINDOW_H

#include <gtk/gtk.h>
#include <libdory-private/dory-file.h>

typedef struct DoryPropertiesWindow DoryPropertiesWindow;

#define DORY_TYPE_PROPERTIES_WINDOW dory_properties_window_get_type()
#define DORY_PROPERTIES_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_PROPERTIES_WINDOW, DoryPropertiesWindow))
#define DORY_PROPERTIES_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_PROPERTIES_WINDOW, DoryPropertiesWindowClass))
#define DORY_IS_PROPERTIES_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_PROPERTIES_WINDOW))
#define DORY_IS_PROPERTIES_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_PROPERTIES_WINDOW))
#define DORY_PROPERTIES_WINDOW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_PROPERTIES_WINDOW, DoryPropertiesWindowClass))

typedef struct DoryPropertiesWindowDetails DoryPropertiesWindowDetails;

struct DoryPropertiesWindow {
	GtkDialog window;
	DoryPropertiesWindowDetails *details;	
};

struct DoryPropertiesWindowClass {
	GtkDialogClass parent_class;
	
	/* Keybinding signals */
	void (* close)    (DoryPropertiesWindow *window);
};

typedef struct DoryPropertiesWindowClass DoryPropertiesWindowClass;

GType   dory_properties_window_get_type   (void);

void 	dory_properties_window_present    (GList       *files,
					       GtkWidget   *parent_widget,
					       const gchar *startup_id);

#endif /* DORY_PROPERTIES_WINDOW_H */
