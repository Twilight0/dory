/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* dory-empty-view.h - interface for empty view of directory.

   Copyright (C) 2006 Free Software Foundation, Inc.
   
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

   Authors: Christian Neumair <chris@gnome-de.org>
*/

#ifndef DORY_EMPTY_VIEW_H
#define DORY_EMPTY_VIEW_H

#include "dory-view.h"

#define DORY_TYPE_EMPTY_VIEW dory_empty_view_get_type()
#define DORY_EMPTY_VIEW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_EMPTY_VIEW, DoryEmptyView))
#define DORY_EMPTY_VIEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_EMPTY_VIEW, DoryEmptyViewClass))
#define DORY_IS_EMPTY_VIEW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_EMPTY_VIEW))
#define DORY_IS_EMPTY_VIEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_EMPTY_VIEW))
#define DORY_EMPTY_VIEW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_EMPTY_VIEW, DoryEmptyViewClass))

#define DORY_EMPTY_VIEW_ID "OAFIID:Dory_File_Manager_Empty_View"

typedef struct DoryEmptyViewDetails DoryEmptyViewDetails;

typedef struct {
	DoryView parent_instance;
	DoryEmptyViewDetails *details;
} DoryEmptyView;

typedef struct {
	DoryViewClass parent_class;
} DoryEmptyViewClass;

GType dory_empty_view_get_type (void);
void  dory_empty_view_register (void);

#endif /* DORY_EMPTY_VIEW_H */
