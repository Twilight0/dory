/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* dory-file-drag.c - Drag & drop handling code that operated on 
   DoryFile objects.

   Copyright (C) 2000 Eazel, Inc.

   The Gnome Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gnome Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
   Boston, MA 02110-1335, USA.

   Authors: Pavel Cisler <pavel@eazel.com>,
*/

#include <config.h>
#include "dory-file-dnd.h"
#include "dory-desktop-icon-file.h"

#include "dory-dnd.h"
#include "dory-directory.h"
#include "dory-file-utilities.h"
#include <string.h>

static gboolean
dory_drag_can_accept_files (DoryFile *drop_target_item)
{
	DoryDirectory *directory;

	if (dory_file_is_directory (drop_target_item)) {
		gboolean res;

		/* target is a directory, accept if editable */
		directory = dory_directory_get_for_file (drop_target_item);
		res = dory_directory_is_editable (directory);
		dory_directory_unref (directory);
		return res;
	}
	
	if (DORY_IS_DESKTOP_ICON_FILE (drop_target_item)) {
		return TRUE;
	}
	
	/* Launchers are an acceptable drop target */
	if (dory_file_is_launcher (drop_target_item)) {
		return TRUE;
	}

	if (dory_is_file_roller_installed () &&
	    dory_file_is_archive (drop_target_item)) {
		return TRUE;
	}
	
	return FALSE;
}

gboolean
dory_drag_can_accept_item (DoryFile *drop_target_item,
			       const char *item_uri)
{
	if (dory_file_matches_uri (drop_target_item, item_uri)) {
		/* can't accept itself */
		return FALSE;
	}

	return dory_drag_can_accept_files (drop_target_item);
}
				       
gboolean
dory_drag_can_accept_items (DoryFile *drop_target_item,
				const GList *items)
{
	int max;

	if (drop_target_item == NULL)
		return FALSE;

	g_assert (DORY_IS_FILE (drop_target_item));

	/* Iterate through selection checking if item will get accepted by the
	 * drop target. If more than 100 items selected, return an over-optimisic
	 * result
	 */
	for (max = 100; items != NULL && max >= 0; items = items->next, max--) {
		if (!dory_drag_can_accept_item (drop_target_item, 
			((DoryDragSelectionItem *)items->data)->uri)) {
			return FALSE;
		}
	}
	
	return TRUE;
}

gboolean
dory_drag_can_accept_info (DoryFile *drop_target_item,
			       DoryIconDndTargetType drag_type,
			       const GList *items)
{
	switch (drag_type) {
		case DORY_ICON_DND_GNOME_ICON_LIST:
			return dory_drag_can_accept_items (drop_target_item, items);

		case DORY_ICON_DND_URI_LIST:
		case DORY_ICON_DND_NETSCAPE_URL:
		case DORY_ICON_DND_TEXT:
			return dory_drag_can_accept_files (drop_target_item);

		case DORY_ICON_DND_XDNDDIRECTSAVE:
		case DORY_ICON_DND_RAW:
			return dory_drag_can_accept_files (drop_target_item); /* Check if we can accept files at this location */

		case DORY_ICON_DND_ROOTWINDOW_DROP:
			return FALSE;

		default:
			g_assert_not_reached ();
			return FALSE;
	}
}

