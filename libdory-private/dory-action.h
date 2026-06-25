/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-
 
   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
  
   You should have received a copy of the GNU General Public
   License along with this program; if not, write to the
   Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
   Boston, MA 02110-1335, USA.

*/

#ifndef DORY_ACTION_H
#define DORY_ACTION_H

#include <gtk/gtk.h>
#include <glib.h>
#include "dory-file.h"

// GtkAction were deprecated before auto-free functionality was added.
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GtkAction, g_object_unref)

#define DORY_TYPE_ACTION dory_action_get_type()
G_DECLARE_FINAL_TYPE (DoryAction, dory_action, DORY, ACTION, GtkAction)

struct _DoryAction {
    GtkAction parent_instance;

    gchar *uuid; // basename of key_file_path
    gchar *key_file_path;
    gchar *parent_dir;
    gboolean has_accel;
};

struct _DoryActionClass {
    GtkActionClass parent_class;
};

DoryAction   *dory_action_new                  (const gchar *name, const gchar *path);
void          dory_action_activate             (DoryAction *action, GList *selection, DoryFile *parent, GtkWindow *window);

const gchar  *dory_action_get_orig_label       (DoryAction *action);
const gchar  *dory_action_get_orig_tt          (DoryAction *action);
gchar        *dory_action_get_label            (DoryAction *action, GList *selection, DoryFile *parent, GtkWindow *window);
gchar        *dory_action_get_tt               (DoryAction *action, GList *selection, DoryFile *parent, GtkWindow *window);
void          dory_action_update_display_state (DoryAction *action, GList *selection, DoryFile *parent, gboolean for_places, GtkWindow *window);

// Layout model overrides
void          dory_action_override_label       (DoryAction *action, const gchar *label);
void          dory_action_override_icon        (DoryAction *action, const gchar *icon_name);
#endif /* DORY_ACTION_H */
