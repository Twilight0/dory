/*
 *  dory-menu-provider.h - Interface for Dory extensions that 
 *                             provide context menu items.
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
 * add context menu entries to files.  Extensions are called when
 * Dory constructs the context menu for a file.  They are passed a
 * list of DoryFileInfo objects which holds the current selection */

#ifndef DORY_MENU_PROVIDER_H
#define DORY_MENU_PROVIDER_H

#include <glib-object.h>
#include <gtk/gtk.h>
#include "dory-extension-types.h"
#include "dory-file-info.h"
#include "dory-menu.h"

G_BEGIN_DECLS

#define DORY_TYPE_MENU_PROVIDER           (dory_menu_provider_get_type ())

G_DECLARE_INTERFACE (DoryMenuProvider, dory_menu_provider,
                     DORY, MENU_PROVIDER,
                     GObject)

typedef DoryMenuProviderInterface DoryMenuProviderIface;

struct _DoryMenuProviderInterface {
	GTypeInterface g_iface;

	GList *(*get_file_items)       (DoryMenuProvider *provider,
					GtkWidget            *window,
					GList                *files);
	GList *(*get_background_items) (DoryMenuProvider *provider,
					GtkWidget            *window,
					DoryFileInfo     *current_folder);
};

/* Interface Functions */
GList                  *dory_menu_provider_get_file_items       (DoryMenuProvider *provider,
								     GtkWidget            *window,
								     GList                *files);
GList                  *dory_menu_provider_get_background_items (DoryMenuProvider *provider,
								     GtkWidget            *window,
								     DoryFileInfo     *current_folder);

/* This function emit a signal to inform dory that its item list has changed. */
void                    dory_menu_provider_emit_items_updated_signal (DoryMenuProvider *provider);

G_END_DECLS

#endif
