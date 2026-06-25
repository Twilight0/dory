/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
   dory-mime-application-chooser.c: Manages applications for mime types
 
   Copyright (C) 2004 Novell, Inc.
 
   The Gnome Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Library is distributed in the hope that it will be useful,
   but APPLICATIONOUT ANY WARRANTY; applicationout even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along application the Gnome Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
   Boston, MA 02110-1335, USA.

   Authors: Dave Camp <dave@novell.com>
*/

#ifndef DORY_MIME_APPLICATION_CHOOSER_H
#define DORY_MIME_APPLICATION_CHOOSER_H

#include <gtk/gtk.h>

#define DORY_TYPE_MIME_APPLICATION_CHOOSER         (dory_mime_application_chooser_get_type ())
#define DORY_MIME_APPLICATION_CHOOSER(obj)         (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_MIME_APPLICATION_CHOOSER, DoryMimeApplicationChooser))
#define DORY_MIME_APPLICATION_CHOOSER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_MIME_APPLICATION_CHOOSER, DoryMimeApplicationChooserClass))
#define DORY_IS_MIME_APPLICATION_CHOOSER(obj)      (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_MIME_APPLICATION_CHOOSER)

typedef struct _DoryMimeApplicationChooser        DoryMimeApplicationChooser;
typedef struct _DoryMimeApplicationChooserClass   DoryMimeApplicationChooserClass;
typedef struct _DoryMimeApplicationChooserDetails DoryMimeApplicationChooserDetails;

struct _DoryMimeApplicationChooser {
	GtkBox parent;
	DoryMimeApplicationChooserDetails *details;
};

struct _DoryMimeApplicationChooserClass {
	GtkBoxClass parent_class;
};

GType      dory_mime_application_chooser_get_type (void);
GtkWidget * dory_mime_application_chooser_new (const char *uri,
                                                    GList *files,
                                               const char *mime_type,
                                                GtkWidget *ok_button);
GAppInfo  *dory_mime_application_chooser_get_info (DoryMimeApplicationChooser *chooser);
const gchar *dory_mime_application_chooser_get_uri (DoryMimeApplicationChooser *chooser);

#endif /* DORY_MIME_APPLICATION_CHOOSER_H */
