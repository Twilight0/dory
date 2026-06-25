/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* dory-mime-actions.h - uri-specific versions of mime action functions

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

   Authors: Maciej Stachowiak <mjs@eazel.com>
*/

#ifndef DORY_MIME_ACTIONS_H
#define DORY_MIME_ACTIONS_H

#include <gio/gio.h>

#include <libdory-private/dory-file.h>

#include "dory-window.h"

DoryFileAttributes dory_mime_actions_get_required_file_attributes (void);

GAppInfo *             dory_mime_get_default_application_for_file     (DoryFile            *file);
GList *                dory_mime_get_applications_for_file            (DoryFile            *file);

GAppInfo *             dory_mime_get_default_application_for_files    (GList                   *files);
GList *                dory_mime_get_applications_for_files           (GList                   *file);

gboolean               dory_mime_file_opens_in_view                   (DoryFile            *file);
gboolean               dory_mime_file_opens_in_external_app           (DoryFile            *file);
void                   dory_mime_activate_files                       (GtkWindow               *parent_window,
									   DoryWindowSlot      *slot,
									   GList                   *files,
									   const char              *launch_directory,
									   DoryWindowOpenFlags  flags,
									   gboolean                 user_confirmation);
void                   dory_mime_activate_file                        (GtkWindow               *parent_window,
									   DoryWindowSlot      *slot_info,
									   DoryFile            *file,
									   const char              *launch_directory,
									   DoryWindowOpenFlags  flags);
void                   dory_mime_launch_fm_and_select_file            (GFile *file);

#endif /* DORY_MIME_ACTIONS_H */
