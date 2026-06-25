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

#ifndef __DORY_CONNECT_SERVER_OPERATION_H__
#define __DORY_CONNECT_SERVER_OPERATION_H__

#include <gio/gio.h>
#include <gtk/gtk.h>

#include "dory-connect-server-dialog.h"

#define DORY_TYPE_CONNECT_SERVER_OPERATION\
	(dory_connect_server_operation_get_type ())
#define DORY_CONNECT_SERVER_OPERATION(obj)\
  (G_TYPE_CHECK_INSTANCE_CAST ((obj),\
			       DORY_TYPE_CONNECT_SERVER_OPERATION,\
			       DoryConnectServerOperation))
#define DORY_CONNECT_SERVER_OPERATION_CLASS(klass)\
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_CONNECT_SERVER_OPERATION,\
			    DoryConnectServerOperationClass))
#define DORY_IS_CONNECT_SERVER_OPERATION(obj)\
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_CONNECT_SERVER_OPERATION)

typedef struct _DoryConnectServerOperationDetails
  DoryConnectServerOperationDetails;

typedef struct {
	GtkMountOperation parent;
	DoryConnectServerOperationDetails *details;
} DoryConnectServerOperation;

typedef struct {
	GtkMountOperationClass parent_class;
} DoryConnectServerOperationClass;

GType dory_connect_server_operation_get_type (void);

GMountOperation *
dory_connect_server_operation_new (DoryConnectServerDialog *dialog);


#endif /* __DORY_CONNECT_SERVER_OPERATION_H__ */
