/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Dory
 *
 * Copyright (C) 2003 Red Hat, Inc.
 * Copyright (C) 2010 Cosimo Cecchi <cosimoc@gnome.org>
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
 * You should have received a copy of the GNU General Public
 * License along with this program; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
 * Boston, MA 02110-1335, USA.
 */

#ifndef DORY_CONNECT_SERVER_DIALOG_H
#define DORY_CONNECT_SERVER_DIALOG_H

#include <gio/gio.h>
#include <gtk/gtk.h>

#include "dory-application.h"
#include "dory-window.h"

#define DORY_TYPE_CONNECT_SERVER_DIALOG\
	(dory_connect_server_dialog_get_type ())
#define DORY_CONNECT_SERVER_DIALOG(obj)\
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_CONNECT_SERVER_DIALOG,\
				     DoryConnectServerDialog))
#define DORY_CONNECT_SERVER_DIALOG_CLASS(klass)\
	(G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_CONNECT_SERVER_DIALOG,\
				  DoryConnectServerDialogClass))
#define DORY_IS_CONNECT_SERVER_DIALOG(obj)\
	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_CONNECT_SERVER_DIALOG)

typedef struct _DoryConnectServerDialog DoryConnectServerDialog;
typedef struct _DoryConnectServerDialogClass DoryConnectServerDialogClass;
typedef struct _DoryConnectServerDialogDetails DoryConnectServerDialogDetails;

struct _DoryConnectServerDialog {
	GtkDialog parent;
	DoryConnectServerDialogDetails *details;
};

struct _DoryConnectServerDialogClass {
	GtkDialogClass parent_class;
};

GType dory_connect_server_dialog_get_type (void);

GtkWidget* dory_connect_server_dialog_new (DoryWindow *window);

void dory_connect_server_dialog_display_location_async (DoryConnectServerDialog *self,
							    GFile *location,
							    GAsyncReadyCallback callback,
							    gpointer user_data);
gboolean dory_connect_server_dialog_display_location_finish (DoryConnectServerDialog *self,
								 GAsyncResult *result,
								 GError **error);

void dory_connect_server_dialog_fill_details_async (DoryConnectServerDialog *self,
							GMountOperation *operation,
							const gchar *default_user,
							const gchar *default_domain,
							GAskPasswordFlags flags,
							GAsyncReadyCallback callback,
							gpointer user_data);
gboolean dory_connect_server_dialog_fill_details_finish (DoryConnectServerDialog *self,
							     GAsyncResult *result);

#endif /* DORY_CONNECT_SERVER_DIALOG_H */
