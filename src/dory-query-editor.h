/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Copyright (C) 2005 Red Hat, Inc.
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
 * Author: Alexander Larsson <alexl@redhat.com>
 *
 */

#ifndef DORY_QUERY_EDITOR_H
#define DORY_QUERY_EDITOR_H

#include <gtk/gtk.h>

#include <libdory-private/dory-query.h>

G_BEGIN_DECLS

#define DORY_TYPE_QUERY_EDITOR (dory_query_editor_get_type ())

G_DECLARE_FINAL_TYPE (DoryQueryEditor, dory_query_editor, DORY, QUERY_EDITOR, GtkBox)

GtkWidget* dory_query_editor_new                (void);

DoryQuery   *dory_query_editor_get_query          (DoryQueryEditor *editor);
void         dory_query_editor_set_query          (DoryQueryEditor *editor,
                                                   DoryQuery       *query);
GFile       *dory_query_editor_get_location       (DoryQueryEditor *editor);
void         dory_query_editor_set_location       (DoryQueryEditor *editor,
                                                   GFile           *location);
void         dory_query_editor_set_active         (DoryQueryEditor *editor,
                                                   gchar           *base_uri,
                                                   gboolean         active);
gboolean     dory_query_editor_get_active         (DoryQueryEditor *editor);
const gchar *dory_query_editor_get_base_uri       (DoryQueryEditor *editor);

G_END_DECLS

#endif /* DORY_QUERY_EDITOR_H */
