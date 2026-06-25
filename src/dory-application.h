/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * dory-application: main Dory application class.
 *
 * Copyright (C) 2000 Red Hat, Inc.
 * Copyright (C) 2010 Cosimo Cecchi <cosimoc@gnome.org>
 *
 * Dory is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Dory is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
 * Boston, MA 02110-1335, USA.
 */

#ifndef __DORY_APPLICATION_H__
#define __DORY_APPLICATION_H__

#include <gdk/gdk.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

#include <libdory-private/dory-undo-manager.h>

#include "dory-window.h"

#define DORY_TYPE_APPLICATION dory_application_get_type()
#define DORY_APPLICATION(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_APPLICATION, DoryApplication))
#define DORY_APPLICATION_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_APPLICATION, DoryApplicationClass))
#define DORY_IS_APPLICATION(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_APPLICATION))
#define DORY_IS_APPLICATION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_APPLICATION))
#define DORY_APPLICATION_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_APPLICATION, DoryApplicationClass))

typedef struct _DoryApplicationPriv DoryApplicationPriv;
typedef struct DoryApplicationClass DoryApplicationClass;

typedef struct {
	GtkApplication parent;

	DoryUndoManager *undo_manager;

	DoryApplicationPriv *priv;
} DoryApplication;

struct DoryApplicationClass {
	GtkApplicationClass parent_class;

    void         (* continue_startup) (DoryApplication *application);
    void         (* continue_quit) (DoryApplication *application);

    void         (* open_location) (DoryApplication *application,
                                    GFile *location,
                                    GFile *selection,
                                    const char *startup_id,
                                    const gboolean open_in_tabs);

    void         (* show_items)    (DoryApplication *application,
                                    GFile          **uris,
                                    gint             n_uris,
                                    const char      *startup_id);

    DoryWindow * (* create_window) (DoryApplication *application,
                                    GdkScreen       *screen);

    void         (* notify_unmount_done) (DoryApplication *application,
                                          const gchar *message);

    void         (* notify_unmount_show) (DoryApplication *application,
                                          const gchar *message);

    void         (* close_all_windows)   (DoryApplication *application);

};

GType dory_application_get_type (void);
DoryApplication *dory_application_initialize_singleton (GType object_type,
                                                        const gchar *first_property_name,
                                                        ...);
DoryApplication *dory_application_get_singleton (void);
void dory_application_quit (DoryApplication *self);
DoryWindow *     dory_application_create_window (DoryApplication *application,
                                                 GdkScreen           *screen);
void dory_application_open_location (DoryApplication *application,
                                     GFile *location,
                                     GFile *selection,
                                     const char *startup_id,
                                     const gboolean open_in_tabs);
void dory_application_show_items    (DoryApplication *application,
                                     GFile          **uris,
                                     gint             n_uris,
                                     const char      *startup_id);
void dory_application_close_all_windows (DoryApplication *self);

void dory_application_notify_unmount_show (DoryApplication *application,
                                               const gchar *message);

void dory_application_notify_unmount_done (DoryApplication *application,
                                               const gchar *message);
gboolean dory_application_check_required_directory (DoryApplication *application,
                                                    gchar           *path);
void dory_application_check_thumbnail_cache (DoryApplication *application);
gboolean dory_application_get_cache_bad (DoryApplication *application);
void dory_application_clear_cache_flag (DoryApplication *application);
void dory_application_set_cache_flag (DoryApplication *application);
void dory_application_ignore_cache_problem (DoryApplication *application);
gboolean dory_application_get_cache_problem_ignored (DoryApplication *application);
gboolean dory_application_get_show_desktop (DoryApplication *application);

#endif /* __DORY_APPLICATION_H__ */
