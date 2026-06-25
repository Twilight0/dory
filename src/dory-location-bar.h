/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Dory
 *
 * Copyright (C) 2000 Eazel, Inc.
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
 * You should have received a copy of the GNU General Public
 * License along with this program; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
 * Boston, MA 02110-1335, USA.
 *
 * Author: Maciej Stachowiak <mjs@eazel.com>
 *         Ettore Perazzoli <ettore@gnu.org>
 */

/* dory-location-bar.h - Location bar for Dory
 */

#ifndef DORY_LOCATION_BAR_H
#define DORY_LOCATION_BAR_H

#include <libdory-private/dory-entry.h>
#include <gtk/gtk.h>

#define DORY_TYPE_LOCATION_BAR dory_location_bar_get_type()
#define DORY_LOCATION_BAR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_LOCATION_BAR, DoryLocationBar))
#define DORY_LOCATION_BAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_LOCATION_BAR, DoryLocationBarClass))
#define DORY_IS_LOCATION_BAR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_LOCATION_BAR))
#define DORY_IS_LOCATION_BAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_LOCATION_BAR))
#define DORY_LOCATION_BAR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_LOCATION_BAR, DoryLocationBarClass))

typedef struct DoryLocationBarDetails DoryLocationBarDetails;

typedef struct DoryLocationBar {
	GtkBox parent;
	DoryLocationBarDetails *details;
} DoryLocationBar;

typedef struct {
	GtkBoxClass parent_class;

	/* for GtkBindingSet */
	void         (* cancel)           (DoryLocationBar *bar);
} DoryLocationBarClass;

GType      dory_location_bar_get_type     	(void);
GtkWidget* dory_location_bar_new          	(void);
DoryEntry * dory_location_bar_get_entry (DoryLocationBar *location_bar);

void	dory_location_bar_activate	 (DoryLocationBar *bar);
void    dory_location_bar_set_location     (DoryLocationBar *bar,
						const char          *location);
gboolean dory_location_bar_has_focus (DoryLocationBar *location_bar);

#endif /* DORY_LOCATION_BAR_H */
