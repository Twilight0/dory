/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Dory
 *
 * Copyright (C) 2011, Red Hat, Inc.
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, MA 02110-1335, USA.
 *
 * Author: Cosimo Cecchi <cosimoc@redhat.com>
 *
 */

#ifndef __DORY_TOOLBAR_H__
#define __DORY_TOOLBAR_H__

#include <gtk/gtk.h>

#define DORY_TYPE_TOOLBAR dory_toolbar_get_type()
#define DORY_TOOLBAR(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_TOOLBAR, DoryToolbar))
#define DORY_TOOLBAR_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_TOOLBAR, DoryToolbarClass))
#define DORY_IS_TOOLBAR(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_TOOLBAR))
#define DORY_IS_TOOLBAR_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_TOOLBAR))
#define DORY_TOOLBAR_GET_CLASS(obj) \
	(G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_TOOLBAR, DoryToolbarClass))

typedef struct _DoryToolbar DoryToolbar;
typedef struct _DoryToolbarPriv DoryToolbarPriv;
typedef struct _DoryToolbarClass DoryToolbarClass;

typedef enum {
	DORY_TOOLBAR_MODE_PATH_BAR,
	DORY_TOOLBAR_MODE_LOCATION_BAR,
} DoryToolbarMode;

struct _DoryToolbar {
	GtkBox parent;

	/* private */
	DoryToolbarPriv *priv;
};

struct _DoryToolbarClass {
	GtkBoxClass parent_class;
};

GType dory_toolbar_get_type (void);

GtkWidget *dory_toolbar_new (GtkActionGroup *action_group);

gboolean  dory_toolbar_get_show_location_entry (DoryToolbar *self);
GtkWidget *dory_toolbar_get_path_bar (DoryToolbar *self);
GtkWidget *dory_toolbar_get_location_bar (DoryToolbar *self);

void dory_toolbar_set_show_main_bar (DoryToolbar *self,
					 gboolean show_main_bar);
void dory_toolbar_set_show_location_entry (DoryToolbar *self,
					       gboolean show_location_entry);
void dory_toolbar_update_for_location (DoryToolbar *self);
#endif /* __DORY_TOOLBAR_H__ */
