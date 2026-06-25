/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* Dory - Floating status bar.
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

#ifndef __DORY_FLOATING_BAR_H__
#define __DORY_FLOATING_BAR_H__

#include <gtk/gtk.h>

#define DORY_FLOATING_BAR_ACTION_ID_STOP 1

#define DORY_TYPE_FLOATING_BAR dory_floating_bar_get_type()
#define DORY_FLOATING_BAR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_FLOATING_BAR, DoryFloatingBar))
#define DORY_FLOATING_BAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_FLOATING_BAR, DoryFloatingBarClass))
#define DORY_IS_FLOATING_BAR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_FLOATING_BAR))
#define DORY_IS_FLOATING_BAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_FLOATING_BAR))
#define DORY_FLOATING_BAR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_FLOATING_BAR, DoryFloatingBarClass))

typedef struct _DoryFloatingBar DoryFloatingBar;
typedef struct _DoryFloatingBarClass DoryFloatingBarClass;
typedef struct _DoryFloatingBarDetails DoryFloatingBarDetails;

struct _DoryFloatingBar {
	GtkBox parent;
	DoryFloatingBarDetails *priv;
};

struct _DoryFloatingBarClass {
	GtkBoxClass parent_class;
};

/* GObject */
GType       dory_floating_bar_get_type  (void);

GtkWidget * dory_floating_bar_new              (const gchar *label,
						    gboolean show_spinner);

void        dory_floating_bar_set_label        (DoryFloatingBar *self,
						    const gchar *label);
void        dory_floating_bar_set_show_spinner (DoryFloatingBar *self,
						    gboolean show_spinner);

void        dory_floating_bar_add_action       (DoryFloatingBar *self,
						    const gchar *stock_id,
						    gint action_id);
void        dory_floating_bar_cleanup_actions  (DoryFloatingBar *self);

#endif /* __DORY_FLOATING_BAR_H__ */

