/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Copyright (C) 2005 Novell, Inc.
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
 * Author: Anders Carlsson <andersca@imendio.com>
 *
 */

#ifndef DORY_QUERY_H
#define DORY_QUERY_H

#include <glib-object.h>

#define DORY_TYPE_QUERY		(dory_query_get_type ())
#define DORY_QUERY(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_QUERY, DoryQuery))
#define DORY_QUERY_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_QUERY, DoryQueryClass))
#define DORY_IS_QUERY(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_QUERY))
#define DORY_IS_QUERY_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_QUERY))
#define DORY_QUERY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_QUERY, DoryQueryClass))

typedef struct DoryQueryDetails DoryQueryDetails;

typedef struct DoryQuery {
	GObject parent;
	DoryQueryDetails *details;
} DoryQuery;

typedef struct {
	GObjectClass parent_class;
} DoryQueryClass;

GType          dory_query_get_type (void);
gboolean       dory_query_enabled  (void);

DoryQuery* dory_query_new      (void);

char *         dory_query_get_file_pattern (DoryQuery *query);
void           dory_query_set_file_pattern (DoryQuery *query, const char *text);

char *         dory_query_get_content_pattern (DoryQuery *query);
void           dory_query_set_content_pattern (DoryQuery *query, const char *text);
gboolean       dory_query_has_content_pattern (DoryQuery *query);

char *         dory_query_get_location       (DoryQuery *query);
void           dory_query_set_location       (DoryQuery *query, const char *uri);

GList *        dory_query_get_mime_types     (DoryQuery *query);
void           dory_query_set_mime_types     (DoryQuery *query, GList *mime_types);
void           dory_query_add_mime_type      (DoryQuery *query, const char *mime_type);

void           dory_query_set_show_hidden    (DoryQuery *query, gboolean hidden);
gboolean       dory_query_get_show_hidden    (DoryQuery *query);

gboolean       dory_query_get_file_case_sensitive (DoryQuery *query);
void           dory_query_set_file_case_sensitive (DoryQuery *query, gboolean case_sensitive);

gboolean       dory_query_get_content_case_sensitive (DoryQuery *query);
void           dory_query_set_content_case_sensitive (DoryQuery *query, gboolean case_sensitive);

gboolean       dory_query_get_use_file_regex      (DoryQuery *query);
void           dory_query_set_use_file_regex      (DoryQuery *query, gboolean file_use_regex);

gboolean       dory_query_get_use_content_regex      (DoryQuery *query);
void           dory_query_set_use_content_regex      (DoryQuery *query, gboolean content_use_regex);

gboolean       dory_query_get_recurse         (DoryQuery *query);
void           dory_query_set_recurse         (DoryQuery *query, gboolean recurse);

char *         dory_query_to_readable_string (DoryQuery *query);
DoryQuery *dory_query_load               (char *file);
gboolean       dory_query_save               (DoryQuery *query, char *file);

#endif /* DORY_QUERY_H */
