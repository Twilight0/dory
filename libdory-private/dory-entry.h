/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* DoryEntry: one-line text editing widget. This consists of bug fixes
 * and other improvements to GtkEntry, and all the changes could be rolled
 * into GtkEntry some day.
 *
 * Copyright (C) 2000 Eazel, Inc.
 *
 * Author: John Sullivan <sullivan@eazel.com>
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
 */

#ifndef DORY_ENTRY_H
#define DORY_ENTRY_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define DORY_TYPE_ENTRY dory_entry_get_type()
#define DORY_ENTRY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_ENTRY, DoryEntry))
#define DORY_ENTRY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_ENTRY, DoryEntryClass))
#define DORY_IS_ENTRY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_ENTRY))
#define DORY_IS_ENTRY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_ENTRY))
#define DORY_ENTRY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_ENTRY, DoryEntryClass))

typedef struct DoryEntryDetails DoryEntryDetails;

typedef struct {
	GtkEntry parent;
	DoryEntryDetails *details;
} DoryEntry;

typedef struct {
	GtkEntryClass parent_class;

	void (*user_changed)      (DoryEntry *entry);
	void (*selection_changed) (DoryEntry *entry);
} DoryEntryClass;

GType       dory_entry_get_type                 (void);
GtkWidget  *dory_entry_new                      (void);
GtkWidget  *dory_entry_new_with_max_length      (guint16        max);
void        dory_entry_set_text                 (DoryEntry *entry,
						     const char    *text);
void        dory_entry_select_all               (DoryEntry *entry);
void        dory_entry_select_all_at_idle       (DoryEntry *entry);
void        dory_entry_set_special_tab_handling (DoryEntry *entry,
						     gboolean       special_tab_handling);

G_END_DECLS

#endif /* DORY_ENTRY_H */
