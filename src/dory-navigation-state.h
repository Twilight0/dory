/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* Dory - Dory navigation state
 *
 * Copyright (C) 2011 Red Hat Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
 * Boston, MA 02110-1335, USA.
 *
 * Authors: Cosimo Cecchi <cosimoc@redhat.com>
 *
 */

#ifndef __DORY_NAVIGATION_STATE_H__
#define __DORY_NAVIGATION_STATE_H__

#include <glib-object.h>
#include <gtk/gtk.h>

#define DORY_TYPE_NAVIGATION_STATE dory_navigation_state_get_type()
#define DORY_NAVIGATION_STATE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_NAVIGATION_STATE, DoryNavigationState))
#define DORY_NAVIGATION_STATE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_NAVIGATION_STATE, DoryNavigationStateClass))
#define DORY_IS_NAVIGATION_STATE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_NAVIGATION_STATE))
#define DORY_IS_NAVIGATION_STATE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_NAVIGATION_STATE))
#define DORY_NAVIGATION_STATE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_NAVIGATION_STATE, DoryNavigationStateClass))

typedef struct _DoryNavigationState DoryNavigationState;
typedef struct _DoryNavigationStateClass DoryNavigationStateClass;
typedef struct _DoryNavigationStateDetails DoryNavigationStateDetails;

struct _DoryNavigationState {
	GObject parent;
	DoryNavigationStateDetails *priv;
};

struct _DoryNavigationStateClass {
	GObjectClass parent_class;
};

/* GObject */
GType       dory_navigation_state_get_type  (void);

DoryNavigationState * dory_navigation_state_new (GtkActionGroup *slave,
                                                         const gchar **action_names);

void dory_navigation_state_add_group (DoryNavigationState *state,
                                          GtkActionGroup *group);
void dory_navigation_state_set_master (DoryNavigationState *state,
                                           GtkActionGroup *master);
GtkActionGroup * dory_navigation_state_get_master (DoryNavigationState *self);

void dory_navigation_state_sync_all (DoryNavigationState *state);

void dory_navigation_state_set_boolean (DoryNavigationState *self,
					    const gchar *action_name,
					    gboolean value);

#endif /* __DORY_NAVIGATION_STATE_H__ */
