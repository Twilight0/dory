/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* dory-icon-view.h - interface for icon view of directory.
 *
 * Copyright (C) 2000 Eazel, Inc.
 *
 * The Gnome Library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * The Gnome Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with the Gnome Library; see the file COPYING.LIB.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
 * Boston, MA 02110-1335, USA.
 *
 * Authors: John Sullivan <sullivan@eazel.com>
 *
 */

#ifndef DORY_ICON_VIEW_H
#define DORY_ICON_VIEW_H

#include "dory-view.h"

typedef struct DoryIconView DoryIconView;
typedef struct DoryIconViewClass DoryIconViewClass;

#define DORY_TYPE_ICON_VIEW dory_icon_view_get_type()
#define DORY_ICON_VIEW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_ICON_VIEW, DoryIconView))
#define DORY_ICON_VIEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_ICON_VIEW, DoryIconViewClass))
#define DORY_IS_ICON_VIEW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_ICON_VIEW))
#define DORY_IS_ICON_VIEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_ICON_VIEW))
#define DORY_ICON_VIEW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_ICON_VIEW, DoryIconViewClass))

#define DORY_ICON_VIEW_ID "OAFIID:Dory_File_Manager_Icon_View"
#define FM_COMPACT_VIEW_ID "OAFIID:Dory_File_Manager_Compact_View"

typedef struct DoryIconViewDetails DoryIconViewDetails;

struct DoryIconView {
	DoryView parent;
	DoryIconViewDetails *details;
};

struct DoryIconViewClass {
	DoryViewClass parent_class;

    gboolean use_grid_container;
};

/* GObject support */
GType   dory_icon_view_get_type      (void);
int     dory_icon_view_compare_files (DoryIconView   *icon_view,
					  DoryFile *a,
					  DoryFile *b);
gboolean dory_icon_view_is_compact   (DoryIconView *icon_view);

void    dory_icon_view_register         (void);
void    dory_icon_view_compact_register (void);

DoryIconContainer * dory_icon_view_get_icon_container (DoryIconView *view);

void    dory_icon_view_set_sort_criterion_by_sort_type (DoryIconView     *icon_view,
                                                        DoryFileSortType  sort_type);
void    dory_icon_view_set_directory_keep_aligned (DoryIconView *icon_view,
                                                   DoryFile *file,
                                                   gboolean keep_aligned);
gchar  *dory_icon_view_get_directory_sort_by      (DoryIconView *icon_view, DoryFile *file);
gboolean dory_icon_view_get_directory_sort_reversed (DoryIconView *icon_view, DoryFile *file);
void    dory_icon_view_flip_sort_reversed (DoryIconView *icon_view);
gboolean dory_icon_view_set_sort_reversed (DoryIconView *icon_view,
                                          gboolean      new_value,
                                          gboolean      set_metadata);
void   dory_icon_view_set_directory_horizontal_layout (DoryIconView *icon_view,
                                                       DoryFile     *file,
                                                       gboolean      horizontal);
gboolean dory_icon_view_get_directory_horizontal_layout (DoryIconView *icon_view,
                                                         DoryFile     *file);

void dory_icon_view_set_directory_grid_adjusts (DoryIconView *icon_view,
                                                DoryFile     *file,
                                                gint          horizontal,
                                                gint          vertical);
void dory_icon_view_get_directory_grid_adjusts (DoryIconView *icon_view,
                                                DoryFile     *file,
                                                gint         *horizontal,
                                                gint         *vertical);
#endif /* DORY_ICON_VIEW_H */
