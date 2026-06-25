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

#include <config.h>
#include <string.h>

#include "dory-query.h"
#include <eel/eel-glib-extensions.h>
#include <glib/gi18n.h>
#include <libdory-private/dory-file-utilities.h>

struct DoryQueryDetails {
    gchar *file_pattern;
    gchar *content_pattern;
    char *location_uri;
    GList *mime_types;
    gboolean show_hidden;
    gboolean file_case_sensitive;
    gboolean file_use_regex;
    gboolean content_case_sensitive;
    gboolean content_use_regex;
    gboolean count_hits;
    gboolean recurse;
};

G_DEFINE_TYPE (DoryQuery, dory_query, G_TYPE_OBJECT);

static void
finalize (GObject *object)
{
	DoryQuery *query;

	query = DORY_QUERY (object);
    g_free (query->details->file_pattern);
	g_free (query->details->content_pattern);
	g_free (query->details->location_uri);

	G_OBJECT_CLASS (dory_query_parent_class)->finalize (object);
}

static void
dory_query_class_init (DoryQueryClass *class)
{
	GObjectClass *gobject_class;

	gobject_class = G_OBJECT_CLASS (class);
	gobject_class->finalize = finalize;

	g_type_class_add_private (class, sizeof (DoryQueryDetails));
}

static void
dory_query_init (DoryQuery *query)
{
	query->details = G_TYPE_INSTANCE_GET_PRIVATE (query, DORY_TYPE_QUERY,
						      DoryQueryDetails);
}

DoryQuery *
dory_query_new (void)
{
	return g_object_new (DORY_TYPE_QUERY,  NULL);
}

char *
dory_query_get_file_pattern (DoryQuery *query)
{
    g_return_val_if_fail (DORY_IS_QUERY (query), NULL);

	return g_strdup (query->details->file_pattern);
}

void
dory_query_set_file_pattern (DoryQuery *query, const char *text)
{
    g_return_if_fail (DORY_IS_QUERY (query));

	g_free (query->details->file_pattern);
	query->details->file_pattern = g_strstrip (g_strdup (text));
}

char *
dory_query_get_content_pattern (DoryQuery *query)
{
    g_return_val_if_fail (DORY_IS_QUERY (query), NULL);

    return g_strdup (query->details->content_pattern);
}

void
dory_query_set_content_pattern (DoryQuery *query, const char *text)
{
    g_return_if_fail (DORY_IS_QUERY (query));

    g_clear_pointer (&query->details->content_pattern, g_free);

    if (text && text[0] != '\0') {
        query->details->content_pattern = g_strstrip (g_strdup (text));
    }
}

gboolean
dory_query_has_content_pattern (DoryQuery *query)
{
    g_return_val_if_fail (DORY_IS_QUERY (query), FALSE);

    return query->details->content_pattern != NULL;
}

char *
dory_query_get_location (DoryQuery *query)
{
    g_return_val_if_fail (DORY_IS_QUERY (query), NULL);

	return g_strdup (query->details->location_uri);
}

void
dory_query_set_location (DoryQuery *query, const char *uri)
{
    g_return_if_fail (DORY_IS_QUERY (query));

	g_free (query->details->location_uri);
	query->details->location_uri = g_strdup (uri);
}

GList *
dory_query_get_mime_types (DoryQuery *query)
{
    g_return_val_if_fail (DORY_IS_QUERY (query), NULL);

	return eel_g_str_list_copy (query->details->mime_types);
}

void
dory_query_set_mime_types (DoryQuery *query, GList *mime_types)
{
    g_return_if_fail (DORY_IS_QUERY (query));

	g_list_free_full (query->details->mime_types, g_free);
	query->details->mime_types = eel_g_str_list_copy (mime_types);
}

void
dory_query_add_mime_type (DoryQuery *query, const char *mime_type)
{
    g_return_if_fail (DORY_IS_QUERY (query));

	query->details->mime_types = g_list_append (query->details->mime_types,
						    g_strdup (mime_type));
}

void
dory_query_set_show_hidden (DoryQuery *query, gboolean hidden)
{
    g_return_if_fail (DORY_IS_QUERY (query));

    query->details->show_hidden = hidden;
}

gboolean
dory_query_get_show_hidden (DoryQuery *query)
{
    g_return_val_if_fail (DORY_IS_QUERY (query), FALSE);

    return query->details->show_hidden;
}

char *
dory_query_to_readable_string (DoryQuery *query)
{
    g_return_val_if_fail (DORY_IS_QUERY (query), NULL);

    GFile *file;
    gchar *location_title, *readable;

	if (!query || !query->details->file_pattern || query->details->file_pattern[0] == '\0') {
		return g_strdup (_("Search"));
	}

    file = g_file_new_for_uri (query->details->location_uri);
    location_title = dory_compute_search_title_for_location (file);

    g_object_unref (file);

    readable = g_strdup_printf (_("Search in \"%s\""), location_title);

    g_free (location_title);

    return readable;
}

gboolean
dory_query_get_file_case_sensitive (DoryQuery *query)
{
    g_return_val_if_fail (DORY_IS_QUERY (query), FALSE);
    return query->details->file_case_sensitive;
}

void
dory_query_set_file_case_sensitive (DoryQuery *query, gboolean case_sensitive)
{
    g_return_if_fail (DORY_IS_QUERY (query));

    query->details->file_case_sensitive = case_sensitive;
}

gboolean
dory_query_get_content_case_sensitive (DoryQuery *query)
{
    g_return_val_if_fail (DORY_IS_QUERY (query), FALSE);
    return query->details->content_case_sensitive;
}

void
dory_query_set_content_case_sensitive (DoryQuery *query, gboolean case_sensitive)
{
    g_return_if_fail (DORY_IS_QUERY (query));

    query->details->content_case_sensitive = case_sensitive;
}

gboolean
dory_query_get_use_file_regex (DoryQuery *query)
{
    g_return_val_if_fail (DORY_IS_QUERY (query), FALSE);
    return query->details->file_use_regex;
}

void
dory_query_set_use_file_regex (DoryQuery *query, gboolean file_use_regex)
{
    g_return_if_fail (DORY_IS_QUERY (query));

    query->details->file_use_regex = file_use_regex;
}

gboolean
dory_query_get_use_content_regex (DoryQuery *query)
{
    g_return_val_if_fail (DORY_IS_QUERY (query), FALSE);
    return query->details->content_use_regex;
}

void
dory_query_set_use_content_regex (DoryQuery *query, gboolean content_use_regex)
{
    g_return_if_fail (DORY_IS_QUERY (query));

    query->details->content_use_regex = content_use_regex;
}

gboolean
dory_query_get_count_hits (DoryQuery *query)
{
    g_return_val_if_fail (DORY_IS_QUERY (query), FALSE);
    return query->details->count_hits;
}

void
dory_query_set_count_hits (DoryQuery *query, gboolean count_hits)
{
    g_return_if_fail (DORY_IS_QUERY (query));

    query->details->count_hits = count_hits;
}

gboolean
dory_query_get_recurse (DoryQuery *query)
{
    g_return_val_if_fail (DORY_IS_QUERY (query), FALSE);
    return query->details->recurse;
}

void
dory_query_set_recurse (DoryQuery *query, gboolean recurse)
{
    g_return_if_fail (DORY_IS_QUERY (query));

    query->details->recurse = recurse;
}

