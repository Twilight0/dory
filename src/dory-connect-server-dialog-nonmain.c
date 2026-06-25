/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Dory
 *
 * Copyright (C) 2005 Red Hat, Inc.
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

#include <config.h>
#include <gio/gio.h>
#include "dory-connect-server-dialog.h"
#include <libdory-private/dory-global-preferences.h>

/* This file contains the glue for the calls from the connect to server dialog
 * to the main dory binary. A different version of this glue is in
 * dory-connect-server-dialog-main.c for the standalone version.
 */

static GSimpleAsyncResult *display_location_res = NULL;

static void
window_go_to_cb (DoryWindow *window,
		 GError *error,
		 gpointer user_data)
{
	if (error != NULL) {
		g_simple_async_result_set_from_error (display_location_res, error);
	}

	g_simple_async_result_complete (display_location_res);

	g_object_unref (display_location_res);
	display_location_res = NULL;
}

gboolean
dory_connect_server_dialog_display_location_finish (DoryConnectServerDialog *self,
							GAsyncResult *res,
							GError **error)
{
	if (g_simple_async_result_propagate_error (G_SIMPLE_ASYNC_RESULT (res), error)) {
		return FALSE;
	}

	return TRUE;
}

void
dory_connect_server_dialog_display_location_async (DoryConnectServerDialog *self,
						       GFile *location,
						       GAsyncReadyCallback callback,
						       gpointer user_data)
{
	DoryWindow *window;
	GtkWidget *widget;

	widget = GTK_WIDGET (self);

	display_location_res =
		g_simple_async_result_new (G_OBJECT (self),
					   callback, user_data,
					   dory_connect_server_dialog_display_location_async);

	window = dory_application_create_window (dory_application_get_singleton (),
						     gtk_widget_get_screen (widget));

	dory_window_go_to_full (window, location,
				    window_go_to_cb, self);
}
