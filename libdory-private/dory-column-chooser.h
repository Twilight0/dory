/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* dory-column-choose.h - A column chooser widget

   Copyright (C) 2004 Novell, Inc.

   The Gnome Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gnome Library; see the column COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
   Boston, MA 02110-1335, USA.

   Authors: Dave Camp <dave@ximian.com>
*/

#ifndef DORY_COLUMN_CHOOSER_H
#define DORY_COLUMN_CHOOSER_H

#include <gtk/gtk.h>
#include <libdory-private/dory-file.h>

#define DORY_TYPE_COLUMN_CHOOSER dory_column_chooser_get_type()
#define DORY_COLUMN_CHOOSER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_COLUMN_CHOOSER, DoryColumnChooser))
#define DORY_COLUMN_CHOOSER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_COLUMN_CHOOSER, DoryColumnChooserClass))
#define DORY_IS_COLUMN_CHOOSER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_COLUMN_CHOOSER))
#define DORY_IS_COLUMN_CHOOSER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_COLUMN_CHOOSER))
#define DORY_COLUMN_CHOOSER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_COLUMN_CHOOSER, DoryColumnChooserClass))

typedef struct _DoryColumnChooserDetails DoryColumnChooserDetails;

typedef struct {
	GtkBox parent;
	
	DoryColumnChooserDetails *details;
} DoryColumnChooser;

typedef struct {
        GtkBoxClass parent_slot;

	void (*changed) (DoryColumnChooser *chooser);
	void (*use_default) (DoryColumnChooser *chooser);
} DoryColumnChooserClass;

GType      dory_column_chooser_get_type            (void);
GtkWidget *dory_column_chooser_new                 (DoryFile *file);
void       dory_column_chooser_set_settings    (DoryColumnChooser   *chooser,
						    char                   **visible_columns, 
						    char                   **column_order);
void       dory_column_chooser_get_settings    (DoryColumnChooser *chooser,
						    char                  ***visible_columns, 
						    char                  ***column_order);

#endif /* DORY_COLUMN_CHOOSER_H */
