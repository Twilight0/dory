/*
 *  dory-property-page-provider.h - Interface for Dory extensions
 *                                      that provide property pages.
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
 * add property page to property dialogs.  Extensions are called when 
 * Dory needs property pages for a selection.  They are passed a 
 * list of DoryFileInfo objects for which information should
 * be displayed  */

#ifndef DORY_PROPERTY_PAGE_PROVIDER_H
#define DORY_PROPERTY_PAGE_PROVIDER_H

#include <glib-object.h>
#include "dory-extension-types.h"
#include "dory-file-info.h"
#include "dory-property-page.h"

G_BEGIN_DECLS

#define DORY_TYPE_PROPERTY_PAGE_PROVIDER           (dory_property_page_provider_get_type ())

G_DECLARE_INTERFACE (DoryPropertyPageProvider, dory_property_page_provider,
                     DORY, PROPERTY_PAGE_PROVIDER,
                     GObject)

typedef DoryPropertyPageProviderInterface DoryPropertyPageProviderIface;

struct _DoryPropertyPageProviderInterface {
	GTypeInterface g_iface;

	GList *(*get_pages) (DoryPropertyPageProvider     *provider,
			     GList                    *files);
};

/* Interface Functions */
GList                  *dory_property_page_provider_get_pages (DoryPropertyPageProvider *provider,
								   GList                        *files);

G_END_DECLS

#endif
