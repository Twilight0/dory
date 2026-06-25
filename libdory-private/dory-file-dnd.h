/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/* dory-file-drag.h - Drag & drop handling code that operated on 
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

#ifndef DORY_FILE_DND_H
#define DORY_FILE_DND_H

#include <libdory-private/dory-dnd.h>
#include <libdory-private/dory-file.h>

#define DORY_FILE_DND_ERASE_KEYWORD "erase"

gboolean dory_drag_can_accept_item              (DoryFile *drop_target_item,
						     const char   *item_uri);
gboolean dory_drag_can_accept_items             (DoryFile *drop_target_item,
						     const GList  *items);
gboolean dory_drag_can_accept_info              (DoryFile *drop_target_item,
						     DoryIconDndTargetType drag_type,
						     const GList *items);

#endif /* DORY_FILE_DND_H */

