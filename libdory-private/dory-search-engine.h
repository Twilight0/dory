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

#ifndef DORY_SEARCH_ENGINE_H
#define DORY_SEARCH_ENGINE_H

#include <glib-object.h>
#include <libdory-private/dory-query.h>

#define DORY_TYPE_SEARCH_ENGINE		(dory_search_engine_get_type ())
#define DORY_SEARCH_ENGINE(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_SEARCH_ENGINE, DorySearchEngine))
#define DORY_SEARCH_ENGINE_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_SEARCH_ENGINE, DorySearchEngineClass))
#define DORY_IS_SEARCH_ENGINE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_SEARCH_ENGINE))
#define DORY_IS_SEARCH_ENGINE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_SEARCH_ENGINE))
#define DORY_SEARCH_ENGINE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_SEARCH_ENGINE, DorySearchEngineClass))

typedef struct DorySearchEngineDetails DorySearchEngineDetails;

typedef struct DorySearchEngine {
	GObject parent;
	DorySearchEngineDetails *details;
} DorySearchEngine;

typedef struct {
	GObjectClass parent_class;
	
	/* VTable */
	void (*set_query) (DorySearchEngine *engine, DoryQuery *query);
	void (*start) (DorySearchEngine *engine);
	void (*stop) (DorySearchEngine *engine);

	/* Signals */
	void (*hits_added) (DorySearchEngine *engine, GList *hit_infos);
	void (*hits_subtracted) (DorySearchEngine *engine, GList *hits);
	void (*finished) (DorySearchEngine *engine);
	void (*error) (DorySearchEngine *engine, const char *error_message);
} DorySearchEngineClass;

GType          dory_search_engine_get_type  (void);
gboolean       dory_search_engine_enabled (void);

DorySearchEngine* dory_search_engine_new       (void);

void           dory_search_engine_set_query (DorySearchEngine *engine, DoryQuery *query);
void	       dory_search_engine_start (DorySearchEngine *engine);
void	       dory_search_engine_stop (DorySearchEngine *engine);

void	       dory_search_engine_hits_added (DorySearchEngine *engine, GList *hits);
void	       dory_search_engine_hits_subtracted (DorySearchEngine *engine, GList *hits);
void	       dory_search_engine_finished (DorySearchEngine *engine);
void	       dory_search_engine_error (DorySearchEngine *engine, const char *error_message);

typedef struct {
    gchar     *uri;           // The file uri;
    gchar     *snippet;          // List of hits.
    gint64     hits;
} FileSearchResult;

FileSearchResult *file_search_result_new     (gchar *uri, gchar *snippet);
void              file_search_result_free    (FileSearchResult *result);
void              file_search_result_add_hit (FileSearchResult *result);

gboolean       dory_search_engine_check_filename_pattern (DoryQuery   *query,
                                                          GError     **error);
gboolean       dory_search_engine_check_content_pattern  (DoryQuery   *query,
                                                          GError     **error);

void              dory_search_engine_report_accounting (void);
#endif /* DORY_SEARCH_ENGINE_H */
