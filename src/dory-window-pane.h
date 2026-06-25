/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   dory-window-pane.h: Dory window pane

   Copyright (C) 2008 Free Software Foundation, Inc.

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

   Author: Holger Berndt <berndth@gmx.de>
*/

#ifndef DORY_WINDOW_PANE_H
#define DORY_WINDOW_PANE_H

#include <glib-object.h>

#include "dory-window.h"

#include <libdory-private/dory-icon-info.h>

#define DORY_TYPE_WINDOW_PANE	 (dory_window_pane_get_type())
#define DORY_WINDOW_PANE_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), DORY_TYPE_WINDOW_PANE, DoryWindowPaneClass))
#define DORY_WINDOW_PANE(obj)	 (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_WINDOW_PANE, DoryWindowPane))
#define DORY_IS_WINDOW_PANE(obj)      (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_WINDOW_PANE))
#define DORY_IS_WINDOW_PANE_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), DORY_TYPE_WINDOW_PANE))
#define DORY_WINDOW_PANE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), DORY_TYPE_WINDOW_PANE, DoryWindowPaneClass))

struct _DoryWindowPaneClass {
	GtkBoxClass parent_class;
};

/* A DoryWindowPane is a layer between a slot and a window.
 * Each slot is contained in one pane, and each pane can contain
 * one or more slots. It also supports the notion of an "active slot".
 * On the other hand, each pane is contained in a window, while each
 * window can contain one or multiple panes. Likewise, the window has
 * the notion of an "active pane".
 *
 * A navigation window may have one or more panes.
 */
struct _DoryWindowPane {
	GtkBox parent;

	/* hosting window */
	DoryWindow *window;

	/* available slots, and active slot.
	 * Both of them may never be NULL. */
	GList *slots;
	DoryWindowSlot *active_slot;

	/* location bar */
	GtkWidget *location_bar;
	GtkWidget *path_bar;
	GtkWidget *search_bar;
	GtkWidget *tool_bar;

	gboolean temporary_navigation_bar;
	gboolean temporary_search_bar;

	gboolean show_location_entry;

	/* notebook */
	GtkWidget *notebook;

	GtkActionGroup *action_group;
	GtkActionGroup *toolbar_action_group;

	GtkWidget *last_focus_widget;
};

GType dory_window_pane_get_type (void);

DoryWindowPane *dory_window_pane_new (DoryWindow *window);

DoryWindowSlot *dory_window_pane_open_slot (DoryWindowPane *pane,
					    DoryWindowOpenSlotFlags flags);
/* This removes the slot from the given pane but does not close the pane and/or
 * window as well if there are no more slots left afterwards. This
 * functionality is provided by `dory_window_pane_close_slot' below.
 */
void dory_window_pane_remove_slot_unsafe (DoryWindowPane *pane,
					  DoryWindowSlot *slot);

void dory_window_pane_sync_location_widgets (DoryWindowPane *pane);
void dory_window_pane_sync_search_widgets (DoryWindowPane *pane);
void dory_window_pane_set_active (DoryWindowPane *pane, gboolean is_active);
void dory_window_pane_close_slot (DoryWindowPane *pane, DoryWindowSlot *slot);
GtkActionGroup * dory_window_pane_get_toolbar_action_group (DoryWindowPane   *pane);
void dory_window_pane_grab_focus (DoryWindowPane *pane);
void dory_window_pane_sync_up_actions (DoryWindowPane *pane);
/* bars */
void     dory_window_pane_ensure_location_bar (DoryWindowPane *pane);

#endif /* DORY_WINDOW_PANE_H */
