/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* fm-list-view.h - interface for list view of directory.

   Copyright (C) 2000 Eazel, Inc.
   Copyright (C) 2001 Anders Carlsson <andersca@gnu.org>
   
   The Gnome Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gnome Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
   Boston, MA 02110-1335, USA.

   Authors: John Sullivan <sullivan@eazel.com>
            Anders Carlsson <andersca@gnu.org>
*/

#ifndef DORY_LIST_VIEW_H
#define DORY_LIST_VIEW_H

#include "dory-view.h"

#define DORY_TYPE_LIST_VIEW dory_list_view_get_type()
#define DORY_LIST_VIEW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_LIST_VIEW, DoryListView))
#define DORY_LIST_VIEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_LIST_VIEW, DoryListViewClass))
#define DORY_IS_LIST_VIEW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_LIST_VIEW))
#define DORY_IS_LIST_VIEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_LIST_VIEW))
#define DORY_LIST_VIEW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_LIST_VIEW, DoryListViewClass))

#define DORY_LIST_VIEW_ID "OAFIID:Dory_File_Manager_List_View"

typedef struct DoryListViewDetails DoryListViewDetails;

typedef struct {
	DoryView parent_instance;
	DoryListViewDetails *details;
} DoryListView;

typedef struct {
	DoryViewClass parent_class;
} DoryListViewClass;

GType dory_list_view_get_type (void);
void  dory_list_view_register (void);
GtkTreeView* dory_list_view_get_tree_view (DoryListView *list_view);

#endif /* DORY_LIST_VIEW_H */
