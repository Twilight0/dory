/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* DoryUndoManager - Manages undo and redo transactions.
 *                       This is the public interface used by the application.                      
 *
 * Copyright (C) 2000 Eazel, Inc.
 *
 * Author: Gene Z. Ragan <gzr@eazel.com>
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

#ifndef DORY_UNDO_MANAGER_H
#define DORY_UNDO_MANAGER_H

#include <libdory-private/dory-undo.h>

#define DORY_TYPE_UNDO_MANAGER dory_undo_manager_get_type()
#define DORY_UNDO_MANAGER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_UNDO_MANAGER, DoryUndoManager))
#define DORY_UNDO_MANAGER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_UNDO_MANAGER, DoryUndoManagerClass))
#define DORY_IS_UNDO_MANAGER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_UNDO_MANAGER))
#define DORY_IS_UNDO_MANAGER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_UNDO_MANAGER))
#define DORY_UNDO_MANAGER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_UNDO_MANAGER, DoryUndoManagerClass))
	
typedef struct DoryUndoManagerDetails DoryUndoManagerDetails;

typedef struct {
	GObject parent;
	DoryUndoManagerDetails *details;
} DoryUndoManager;

typedef struct {
	GObjectClass parent_slot;
	void (* changed) (GObject *object, gpointer data);
} DoryUndoManagerClass;

GType                dory_undo_manager_get_type                           (void);
DoryUndoManager *dory_undo_manager_new                                (void);

/* Undo operations. */
void                 dory_undo_manager_undo                               (DoryUndoManager *undo_manager);

/* Attach the undo manager to a Gtk object so that object and the widgets inside it can participate in undo. */
void                 dory_undo_manager_attach                             (DoryUndoManager *manager,
									       GObject             *object);

void		dory_undo_manager_append (DoryUndoManager *manager,
					      DoryUndoTransaction *transaction);
void            dory_undo_manager_forget (DoryUndoManager *manager,
					      DoryUndoTransaction *transaction);

#endif /* DORY_UNDO_MANAGER_H */
