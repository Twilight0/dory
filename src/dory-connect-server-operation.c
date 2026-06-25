/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Dory
 *
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
 *
 * Author: Cosimo Cecchi <cosimoc@gnome.org>
 */

#include <config.h>

#include "dory-connect-server-operation.h"

#include "dory-connect-server-dialog.h"

G_DEFINE_TYPE (DoryConnectServerOperation,
	       dory_connect_server_operation, GTK_TYPE_MOUNT_OPERATION);

enum {
	PROP_DIALOG = 1,
	NUM_PROPERTIES
};

struct _DoryConnectServerOperationDetails {
	DoryConnectServerDialog *dialog;
};

static void
fill_details_async_cb (GObject *source,
		       GAsyncResult *result,
		       gpointer user_data)
{
	DoryConnectServerDialog *dialog;
	DoryConnectServerOperation *self;
	gboolean res;

	self = user_data;
	dialog = DORY_CONNECT_SERVER_DIALOG (source);

	res = dory_connect_server_dialog_fill_details_finish (dialog, result);

	if (!res) {
		g_mount_operation_reply (G_MOUNT_OPERATION (self), G_MOUNT_OPERATION_ABORTED);
	} else {
		g_mount_operation_reply (G_MOUNT_OPERATION (self), G_MOUNT_OPERATION_HANDLED);
	}
}

static void
dory_connect_server_operation_ask_password (GMountOperation *op,
						const gchar *message,
						const gchar *default_user,
						const gchar *default_domain,
						GAskPasswordFlags flags)
{
	DoryConnectServerOperation *self;

	self = DORY_CONNECT_SERVER_OPERATION (op);

	dory_connect_server_dialog_fill_details_async (self->details->dialog,
							   G_MOUNT_OPERATION (self),
							   default_user,
							   default_domain,
							   flags,
							   fill_details_async_cb,
							   self);
}

static void
dory_connect_server_operation_set_property (GObject *object,
						guint property_id,
						const GValue *value,
						GParamSpec *pspec)
{
	DoryConnectServerOperation *self;

	self = DORY_CONNECT_SERVER_OPERATION (object);

	switch (property_id) {
	case PROP_DIALOG:
		self->details->dialog = g_value_dup_object (value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}

static void
dory_connect_server_operation_dispose (GObject *object)
{
	DoryConnectServerOperation *self = DORY_CONNECT_SERVER_OPERATION (object);

	g_clear_object (&self->details->dialog);

	G_OBJECT_CLASS (dory_connect_server_operation_parent_class)->dispose (object);
}

static void
dory_connect_server_operation_class_init (DoryConnectServerOperationClass *klass)
{
	GMountOperationClass *mount_op_class;
	GObjectClass *object_class;
	GParamSpec *pspec;

	object_class = G_OBJECT_CLASS (klass);
	object_class->set_property = dory_connect_server_operation_set_property;
	object_class->dispose = dory_connect_server_operation_dispose;

	mount_op_class = G_MOUNT_OPERATION_CLASS (klass);
	mount_op_class->ask_password = dory_connect_server_operation_ask_password;

	pspec = g_param_spec_object ("dialog", "The connect dialog",
				     "The connect to server dialog",
				     DORY_TYPE_CONNECT_SERVER_DIALOG,
				     G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS);
	g_object_class_install_property (object_class, PROP_DIALOG, pspec);

	g_type_class_add_private (klass, sizeof (DoryConnectServerOperationDetails));
}

static void
dory_connect_server_operation_init (DoryConnectServerOperation *self)
{
	self->details = G_TYPE_INSTANCE_GET_PRIVATE (self,
						     DORY_TYPE_CONNECT_SERVER_OPERATION,
						     DoryConnectServerOperationDetails);
}

GMountOperation *
dory_connect_server_operation_new (DoryConnectServerDialog *dialog)
{
	return g_object_new (DORY_TYPE_CONNECT_SERVER_OPERATION,
			     "dialog", dialog,
			     NULL);
}
