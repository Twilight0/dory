/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* dory-file-undo-manager.h - Manages the undo/redo stack
 *
 * Copyright (C) 2007-2011 Amos Brocco
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
 * Boston, MA 02110-1335, USA.
 *
 * Author: Amos Brocco <amos.brocco@gmail.com>
 */

#ifndef __DORY_FILE_UNDO_MANAGER_H__
#define __DORY_FILE_UNDO_MANAGER_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include <gio/gio.h>

#include <libdory-private/dory-file-undo-operations.h>

typedef struct _DoryFileUndoManager DoryFileUndoManager;
typedef struct _DoryFileUndoManagerClass DoryFileUndoManagerClass;
typedef struct _DoryFileUndoManagerPrivate DoryFileUndoManagerPrivate;

#define DORY_TYPE_FILE_UNDO_MANAGER\
	(dory_file_undo_manager_get_type())
#define DORY_FILE_UNDO_MANAGER(object)\
	(G_TYPE_CHECK_INSTANCE_CAST((object), DORY_TYPE_FILE_UNDO_MANAGER,\
				    DoryFileUndoManager))
#define DORY_FILE_UNDO_MANAGER_CLASS(klass)\
	(G_TYPE_CHECK_CLASS_CAST((klass), DORY_TYPE_FILE_UNDO_MANAGER,\
				 DoryFileUndoManagerClass))
#define DORY_IS_FILE_UNDO_MANAGER(object)\
	(G_TYPE_CHECK_INSTANCE_TYPE((object), DORY_TYPE_FILE_UNDO_MANAGER))
#define DORY_IS_FILE_UNDO_MANAGER_CLASS(klass)\
	(G_TYPE_CHECK_CLASS_TYPE((klass), DORY_TYPE_FILE_UNDO_MANAGER))
#define DORY_FILE_UNDO_MANAGER_GET_CLASS(object)\
	(G_TYPE_INSTANCE_GET_CLASS((object), DORY_TYPE_FILE_UNDO_MANAGER,\
				   DoryFileUndoManagerClass))

typedef enum {
	DORY_FILE_UNDO_MANAGER_STATE_NONE,
	DORY_FILE_UNDO_MANAGER_STATE_UNDO,
	DORY_FILE_UNDO_MANAGER_STATE_REDO
} DoryFileUndoManagerState;

struct _DoryFileUndoManager {
	GObject parent_instance;

	/* < private > */
	DoryFileUndoManagerPrivate* priv;
};

struct _DoryFileUndoManagerClass {
	GObjectClass parent_class;
};

GType dory_file_undo_manager_get_type (void) G_GNUC_CONST;

DoryFileUndoManager * dory_file_undo_manager_get (void);

void dory_file_undo_manager_set_action (DoryFileUndoInfo *info);
DoryFileUndoInfo *dory_file_undo_manager_get_action (void);

DoryFileUndoManagerState dory_file_undo_manager_get_state (void);

void dory_file_undo_manager_undo (GtkWindow *parent_window);
void dory_file_undo_manager_redo (GtkWindow *parent_window);

void dory_file_undo_manager_push_flag (void);
gboolean dory_file_undo_manager_pop_flag (void);

#endif /* __DORY_FILE_UNDO_MANAGER_H__ */
