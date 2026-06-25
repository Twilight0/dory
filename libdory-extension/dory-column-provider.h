/*
 *  dory-column-provider.h - Interface for Dory extensions that 
 *                               provide column descriptions.
 *
 *  Copyright (C) 2003 Novell, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin Street, Suite 500, MA 02110-1335, USA.
 * 
 *  Author:  Dave Camp <dave@ximian.com>
 *
 */

/* This interface is implemented by Dory extensions that want to
 * add columns to the list view and details to the icon view.
 * Extensions are asked for a list of columns to display.  Each
 * returned column refers to a string attribute which can be filled in
 * by DoryInfoProvider */

#ifndef DORY_COLUMN_PROVIDER_H
#define DORY_COLUMN_PROVIDER_H

#include <glib-object.h>
#include "dory-extension-types.h"
#include "dory-column.h"

G_BEGIN_DECLS

#define DORY_TYPE_COLUMN_PROVIDER           (dory_column_provider_get_type ())

G_DECLARE_INTERFACE (DoryColumnProvider, dory_column_provider,
                     DORY, COLUMN_PROVIDER,
                     GObject)

typedef DoryColumnProviderInterface DoryColumnProviderIface;

/**
 * DoryColumnProviderInterface:
 * @g_iface: the parent class
 * @get_columns: Fetch an array of @DoryColumn
 */
struct _DoryColumnProviderInterface {
    GTypeInterface g_iface;

    GList *(*get_columns) (DoryColumnProvider *provider);
};

/* Interface Functions */
GList                  *dory_column_provider_get_columns    (DoryColumnProvider *provider);

G_END_DECLS

#endif
