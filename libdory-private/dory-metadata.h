/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   dory-metadata.h: #defines and other metadata-related info
 
   Copyright (C) 2000 Eazel, Inc.
  
   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
  
   You should have received a copy of the GNU General Public
   License along with this program; if not, write to the
   Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
   Boston, MA 02110-1335, USA.
  
   Author: John Sullivan <sullivan@eazel.com>
*/

#ifndef DORY_METADATA_H
#define DORY_METADATA_H

/* Keys for getting/setting Dory metadata. All metadata used in Dory
 * should define its key here, so we can keep track of the whole set easily.
 * Any updates here needs to be added in dory-metadata.c too.
 */

#include <glib.h>

/* Per-file */

#define DORY_METADATA_KEY_DEFAULT_VIEW		 	"dory-default-view"

#define DORY_METADATA_KEY_LOCATION_BACKGROUND_COLOR 	"folder-background-color"
#define DORY_METADATA_KEY_LOCATION_BACKGROUND_IMAGE 	"folder-background-image"

#define DORY_METADATA_KEY_ICON_VIEW_ZOOM_LEVEL       	"dory-icon-view-zoom-level"
#define DORY_METADATA_KEY_ICON_VIEW_AUTO_LAYOUT      	"dory-icon-view-auto-layout"
#define DORY_METADATA_KEY_ICON_VIEW_SORT_BY          	"dory-icon-view-sort-by"
#define DORY_METADATA_KEY_ICON_VIEW_SORT_REVERSED    	"dory-icon-view-sort-reversed"
#define DORY_METADATA_KEY_ICON_VIEW_KEEP_ALIGNED            "dory-icon-view-keep-aligned"
#define DORY_METADATA_KEY_ICON_VIEW_LAYOUT_TIMESTAMP	"dory-icon-view-layout-timestamp"

#define DORY_METADATA_KEY_LIST_VIEW_ZOOM_LEVEL       	"dory-list-view-zoom-level"
#define DORY_METADATA_KEY_LIST_VIEW_SORT_COLUMN      	"dory-list-view-sort-column"
#define DORY_METADATA_KEY_LIST_VIEW_SORT_REVERSED    	"dory-list-view-sort-reversed"
#define DORY_METADATA_KEY_LIST_VIEW_VISIBLE_COLUMNS    	"dory-list-view-visible-columns"
#define DORY_METADATA_KEY_LIST_VIEW_COLUMN_ORDER    	"dory-list-view-column-order"

#define DORY_METADATA_KEY_COMPACT_VIEW_ZOOM_LEVEL		"dory-compact-view-zoom-level"

#define DORY_METADATA_KEY_WINDOW_GEOMETRY			"dory-window-geometry"
#define DORY_METADATA_KEY_WINDOW_SCROLL_POSITION		"dory-window-scroll-position"
#define DORY_METADATA_KEY_WINDOW_SHOW_HIDDEN_FILES		"dory-window-show-hidden-files"
#define DORY_METADATA_KEY_WINDOW_MAXIMIZED			"dory-window-maximized"
#define DORY_METADATA_KEY_WINDOW_STICKY			"dory-window-sticky"
#define DORY_METADATA_KEY_WINDOW_KEEP_ABOVE			"dory-window-keep-above"

#define DORY_METADATA_KEY_SIDEBAR_BACKGROUND_COLOR   	"dory-sidebar-background-color"
#define DORY_METADATA_KEY_SIDEBAR_BACKGROUND_IMAGE   	"dory-sidebar-background-image"
#define DORY_METADATA_KEY_SIDEBAR_BUTTONS			"dory-sidebar-buttons"

#define DORY_METADATA_KEY_ANNOTATION                    "annotation"

#define DORY_METADATA_KEY_ICON_POSITION              	"dory-icon-position"
#define DORY_METADATA_KEY_ICON_POSITION_TIMESTAMP		"dory-icon-position-timestamp"
#define DORY_METADATA_KEY_ICON_SCALE                 	"icon-scale"
#define DORY_METADATA_KEY_CUSTOM_ICON                	"custom-icon"
#define DORY_METADATA_KEY_CUSTOM_ICON_NAME                	"custom-icon-name"
#define DORY_METADATA_KEY_EMBLEMS				"emblems"

#define DORY_METADATA_KEY_MONITOR               "monitor"
#define DORY_METADATA_KEY_DESKTOP_GRID_HORIZONTAL  "desktop-horizontal"
#define DORY_METADATA_KEY_SHOW_THUMBNAILS "show-thumbnails"
#define DORY_METADATA_KEY_DESKTOP_GRID_ADJUST      "desktop-grid-adjust"

#define DORY_METADATA_KEY_PINNED                   "pinned-to-top"
#define DORY_METADATA_KEY_FAVORITE                 "xapp-favorite"
#define DORY_METADATA_KEY_FAVORITE_AVAILABLE     "xapp-favorite-available"

guint dory_metadata_get_id (const char *metadata);

#endif /* DORY_METADATA_H */
