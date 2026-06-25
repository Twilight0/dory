/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* DoryUndoTransaction - An object for an undoable transaction.
 *                           Used internally by undo machinery.
 *                           Not public.
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

#ifndef DORY_UNDO_TRANSACTION_H
#define DORY_UNDO_TRANSACTION_H

#include <libdory-private/dory-undo.h>

#define DORY_TYPE_UNDO_TRANSACTION dory_undo_transaction_get_type()
#define DORY_UNDO_TRANSACTION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_UNDO_TRANSACTION, DoryUndoTransaction))
#define DORY_UNDO_TRANSACTION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_UNDO_TRANSACTION, DoryUndoTransactionClass))
#define DORY_IS_UNDO_TRANSACTION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_UNDO_TRANSACTION))
#define DORY_IS_UNDO_TRANSACTION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_UNDO_TRANSACTION))
#define DORY_UNDO_TRANSACTION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_UNDO_TRANSACTION, DoryUndoTransactionClass))

/* The typedef for DoryUndoTransaction is in dory-undo.h
   to avoid circular deps */
typedef struct _DoryUndoTransactionClass DoryUndoTransactionClass;

struct _DoryUndoTransaction {
	GObject parent_slot;
	
	char *operation_name;
	char *undo_menu_item_label;
	char *undo_menu_item_hint;
	char *redo_menu_item_label;
	char *redo_menu_item_hint;
	GList *atom_list;

	DoryUndoManager *owner;
};

struct _DoryUndoTransactionClass {
	GObjectClass parent_slot;
};

GType                    dory_undo_transaction_get_type            (void);
DoryUndoTransaction *dory_undo_transaction_new                 (const char              *operation_name,
									const char              *undo_menu_item_label,
									const char              *undo_menu_item_hint,
									const char              *redo_menu_item_label,
									const char              *redo_menu_item_hint);
void                     dory_undo_transaction_add_atom            (DoryUndoTransaction *transaction,
									const DoryUndoAtom  *atom);
void                     dory_undo_transaction_add_to_undo_manager (DoryUndoTransaction *transaction,
									DoryUndoManager     *manager);
void                     dory_undo_transaction_unregister_object   (GObject                 *atom_target);
void                     dory_undo_transaction_undo                (DoryUndoTransaction *transaction);

#endif /* DORY_UNDO_TRANSACTION_H */
