/*
 *  dory-column-provider.c - Interface for Dory extensions 
 *                               that provide column specifications.
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

#include <config.h>
#include "dory-column-provider.h"

#include <glib-object.h>

G_DEFINE_INTERFACE (DoryColumnProvider, dory_column_provider, G_TYPE_OBJECT)

/**
 * SECTION:dory-column-provider
 * @Title: DoryColumnProvider
 * @Short_description: An interface to provide additional Dory list view columns.
 *
 * This allows additional columns to be shown in the list view.  This interface
 * generally needs to be used in tandem with a #DoryInfoProvider, to feed file
 * info back to populate the column(s).
 *
 **/

static void
dory_column_provider_default_init (DoryColumnProviderInterface *klass)
{
}

/**
 * dory_column_provider_get_columns:
 * @provider: a #DoryColumnProvider
 *
 * Returns: (element-type DoryColumn) (transfer full): the provided #DoryColumn objects
 **/
GList *
dory_column_provider_get_columns (DoryColumnProvider *provider)
{
	g_return_val_if_fail (DORY_IS_COLUMN_PROVIDER (provider), NULL);
	g_return_val_if_fail (DORY_COLUMN_PROVIDER_GET_IFACE (provider)->get_columns != NULL, NULL);

	return DORY_COLUMN_PROVIDER_GET_IFACE (provider)->get_columns (provider);
}

