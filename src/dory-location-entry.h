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

#ifndef DORY_LOCATION_ENTRY_H
#define DORY_LOCATION_ENTRY_H

#include <libdory-private/dory-entry.h>

#define DORY_TYPE_LOCATION_ENTRY dory_location_entry_get_type()
#define DORY_LOCATION_ENTRY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_LOCATION_ENTRY, DoryLocationEntry))
#define DORY_LOCATION_ENTRY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_LOCATION_ENTRY, DoryLocationEntryClass))
#define DORY_IS_LOCATION_ENTRY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_LOCATION_ENTRY))
#define DORY_IS_LOCATION_ENTRY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_LOCATION_ENTRY))
#define DORY_LOCATION_ENTRY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_LOCATION_ENTRY, DoryLocationEntryClass))

typedef struct DoryLocationEntryDetails DoryLocationEntryDetails;

typedef struct DoryLocationEntry {
	DoryEntry parent;
	DoryLocationEntryDetails *details;
} DoryLocationEntry;

typedef struct {
	DoryEntryClass parent_class;
} DoryLocationEntryClass;

typedef enum {
	DORY_LOCATION_ENTRY_ACTION_GOTO,
	DORY_LOCATION_ENTRY_ACTION_CLEAR
} DoryLocationEntryAction;

GType      dory_location_entry_get_type     	(void);
GtkWidget* dory_location_entry_new          	(void);
void       dory_location_entry_set_special_text     (DoryLocationEntry *entry,
							 const char            *special_text);
void       dory_location_entry_set_secondary_action (DoryLocationEntry *entry,
							 DoryLocationEntryAction secondary_action);
DoryLocationEntryAction dory_location_entry_get_secondary_action (DoryLocationEntry *entry);
void       dory_location_entry_update_current_location (DoryLocationEntry *entry,
							    const char *path);

#endif /* DORY_LOCATION_ENTRY_H */
