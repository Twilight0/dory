/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* dory-file-undo-manager.c - Manages the undo/redo stack
 *
 * Copyright (C) 2007-2011 Amos Brocco
 * Copyright (C) 2010, 2012 Red Hat, Inc.
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
 * Authors: Amos Brocco <amos.brocco@gmail.com>
 *          Cosimo Cecchi <cosimoc@redhat.com>
 *
 */

#include <config.h>

#include "dory-file-undo-manager.h"

#include "dory-file-operations.h"
#include "dory-file.h"
#include "dory-trash-monitor.h"

#include <glib/gi18n.h>

#define DEBUG_FLAG DORY_DEBUG_UNDO
#include "dory-debug.h"

enum {
	SIGNAL_UNDO_CHANGED,
	NUM_SIGNALS,
};

static guint signals[NUM_SIGNALS] = { 0, };

G_DEFINE_TYPE (DoryFileUndoManager, dory_file_undo_manager, G_TYPE_OBJECT)

struct _DoryFileUndoManagerPrivate
{
	DoryFileUndoInfo *info;
	DoryFileUndoManagerState state;
	DoryFileUndoManagerState last_state;

	guint undo_redo_flag : 1;

	gulong trash_signal_id;
};

static DoryFileUndoManager *undo_singleton = NULL;

static DoryFileUndoManager *
get_singleton (void)
{
	if (undo_singleton == NULL) {
		undo_singleton = g_object_new (DORY_TYPE_FILE_UNDO_MANAGER, NULL);
		g_object_add_weak_pointer (G_OBJECT (undo_singleton), (gpointer) &undo_singleton);
	}

	return undo_singleton;
}

static void
file_undo_manager_clear (DoryFileUndoManager *self)
{
	g_clear_object (&self->priv->info);
	self->priv->state = DORY_FILE_UNDO_MANAGER_STATE_NONE;
}

static void
trash_state_changed_cb (DoryTrashMonitor *monitor,
			gboolean is_empty,
			gpointer user_data)
{
	DoryFileUndoManager *self = user_data;

	if (!is_empty) {
		return;
	}

	if (self->priv->state == DORY_FILE_UNDO_MANAGER_STATE_NONE) {
		return;
	}

	if (DORY_IS_FILE_UNDO_INFO_TRASH (self->priv->info)) {
		file_undo_manager_clear (self);
		g_signal_emit (self, signals[SIGNAL_UNDO_CHANGED], 0);
	}
}

static void
dory_file_undo_manager_init (DoryFileUndoManager * self)
{
	DoryFileUndoManagerPrivate *priv = self->priv = 
		G_TYPE_INSTANCE_GET_PRIVATE (self, 
					     DORY_TYPE_FILE_UNDO_MANAGER, 
					     DoryFileUndoManagerPrivate);

	priv->trash_signal_id = g_signal_connect (dory_trash_monitor_get (),
						  "trash-state-changed",
						  G_CALLBACK (trash_state_changed_cb), self);
}

static void
dory_file_undo_manager_finalize (GObject * object)
{
	DoryFileUndoManager *self = DORY_FILE_UNDO_MANAGER (object);
	DoryFileUndoManagerPrivate *priv = self->priv;

	if (priv->trash_signal_id != 0) {
		g_signal_handler_disconnect (dory_trash_monitor_get (),
					     priv->trash_signal_id);
		priv->trash_signal_id = 0;
	}

	file_undo_manager_clear (self);

	G_OBJECT_CLASS (dory_file_undo_manager_parent_class)->finalize (object);
}

static void
dory_file_undo_manager_class_init (DoryFileUndoManagerClass *klass)
{
	GObjectClass *oclass;

	oclass = G_OBJECT_CLASS (klass);

	oclass->finalize = dory_file_undo_manager_finalize;

	signals[SIGNAL_UNDO_CHANGED] =
		g_signal_new ("undo-changed",
			      G_TYPE_FROM_CLASS (klass),
			      G_SIGNAL_RUN_LAST,
			      0, NULL, NULL,
			      g_cclosure_marshal_VOID__VOID,
			      G_TYPE_NONE, 0);

	g_type_class_add_private (klass, sizeof (DoryFileUndoManagerPrivate));
}

static void
undo_info_apply_ready (GObject *source,
		       GAsyncResult *res,
		       gpointer user_data)
{
	DoryFileUndoManager *self = user_data;
	DoryFileUndoInfo *info = DORY_FILE_UNDO_INFO (source);
	gboolean success, user_cancel;

	success = dory_file_undo_info_apply_finish (info, res, &user_cancel, NULL);

	/* just return in case we got another another operation set */
	if ((self->priv->info != NULL) &&
	    (self->priv->info != info)) {
		return;
	}

	if (success) {
		if (self->priv->last_state == DORY_FILE_UNDO_MANAGER_STATE_UNDO) {
			self->priv->state = DORY_FILE_UNDO_MANAGER_STATE_REDO;
		} else if (self->priv->last_state == DORY_FILE_UNDO_MANAGER_STATE_REDO) {
			self->priv->state = DORY_FILE_UNDO_MANAGER_STATE_UNDO;
		}

		self->priv->info = g_object_ref (info);
	} else if (user_cancel) {
		self->priv->state = self->priv->last_state;
		self->priv->info = g_object_ref (info);
	} else {
		file_undo_manager_clear (self);
	}

	g_signal_emit (self, signals[SIGNAL_UNDO_CHANGED], 0);
}

static void
do_undo_redo (DoryFileUndoManager *self,
	      GtkWindow *parent_window)
{
	gboolean undo = self->priv->state == DORY_FILE_UNDO_MANAGER_STATE_UNDO;

	self->priv->last_state = self->priv->state;
	
	dory_file_undo_manager_push_flag ();
	dory_file_undo_info_apply_async (self->priv->info, undo, parent_window,
					     undo_info_apply_ready, self);

	/* clear actions while undoing */
	file_undo_manager_clear (self);
	g_signal_emit (self, signals[SIGNAL_UNDO_CHANGED], 0);
}

void
dory_file_undo_manager_redo (GtkWindow *parent_window)
{
	DoryFileUndoManager *self = get_singleton ();

	if (self->priv->state != DORY_FILE_UNDO_MANAGER_STATE_REDO) {
		g_warning ("Called redo, but state is %s!", self->priv->state == 0 ?
			   "none" : "undo");
		return;
	}

	do_undo_redo (self, parent_window);
}

void
dory_file_undo_manager_undo (GtkWindow *parent_window)
{
	DoryFileUndoManager *self = get_singleton ();

	if (self->priv->state != DORY_FILE_UNDO_MANAGER_STATE_UNDO) {
		g_warning ("Called undo, but state is %s!", self->priv->state == 0 ?
			   "none" : "redo");
		return;
	}

	do_undo_redo (self, parent_window);
}

void
dory_file_undo_manager_set_action (DoryFileUndoInfo *info)
{
	DoryFileUndoManager *self = get_singleton ();

	DEBUG ("Setting undo information %p", info);

	file_undo_manager_clear (self);

	if (info != NULL) {
		self->priv->info = g_object_ref (info);
		self->priv->state = DORY_FILE_UNDO_MANAGER_STATE_UNDO;
		self->priv->last_state = DORY_FILE_UNDO_MANAGER_STATE_NONE;
	}

	g_signal_emit (self, signals[SIGNAL_UNDO_CHANGED], 0);
}

DoryFileUndoInfo *
dory_file_undo_manager_get_action (void)
{
	DoryFileUndoManager *self = get_singleton ();

	return self->priv->info;
}

DoryFileUndoManagerState 
dory_file_undo_manager_get_state (void)
{
	DoryFileUndoManager *self = get_singleton ();

	return self->priv->state;
}

void
dory_file_undo_manager_push_flag (void)
{
	DoryFileUndoManager *self = get_singleton ();
	DoryFileUndoManagerPrivate *priv = self->priv;

	priv->undo_redo_flag = TRUE;
}

gboolean
dory_file_undo_manager_pop_flag (void)
{
	DoryFileUndoManager *self = get_singleton ();
	DoryFileUndoManagerPrivate *priv = self->priv;
	gboolean retval = FALSE;

	if (priv->undo_redo_flag) {
		retval = TRUE;
	}

	priv->undo_redo_flag = FALSE;
	return retval;
}

DoryFileUndoManager *
dory_file_undo_manager_get (void)
{
	return get_singleton ();
}
