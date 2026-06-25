/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Copyright (C) 2005 Mr Jamie McCracken
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
 * Author: Jamie McCracken (jamiemcc@gnome.org)
 *
 */

#ifndef DORY_SEARCH_ENGINE_TRACKER_H
#define DORY_SEARCH_ENGINE_TRACKER_H

#include <libdory-private/dory-search-engine.h>

#define DORY_TYPE_SEARCH_ENGINE_TRACKER		(dory_search_engine_tracker_get_type ())
#define DORY_SEARCH_ENGINE_TRACKER(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_SEARCH_ENGINE_TRACKER, DorySearchEngineTracker))
#define DORY_SEARCH_ENGINE_TRACKER_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_SEARCH_ENGINE_TRACKER, DorySearchEngineTrackerClass))
#define DORY_IS_SEARCH_ENGINE_TRACKER(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_SEARCH_ENGINE_TRACKER))
#define DORY_IS_SEARCH_ENGINE_TRACKER_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_SEARCH_ENGINE_TRACKER))
#define DORY_SEARCH_ENGINE_TRACKER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_SEARCH_ENGINE_TRACKER, DorySearchEngineTrackerClass))

typedef struct DorySearchEngineTrackerDetails DorySearchEngineTrackerDetails;

typedef struct DorySearchEngineTracker {
	DorySearchEngine parent;
	DorySearchEngineTrackerDetails *details;
} DorySearchEngineTracker;

typedef struct {
	DorySearchEngineClass parent_class;
} DorySearchEngineTrackerClass;

GType dory_search_engine_tracker_get_type (void);

DorySearchEngine* dory_search_engine_tracker_new (void);

#endif /* DORY_SEARCH_ENGINE_TRACKER_H */
