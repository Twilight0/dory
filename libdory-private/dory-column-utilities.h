/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* dory-column-utilities.h - Utilities related to column specifications

   Copyright (C) 2004 Novell, Inc.

   The Gnome Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gnome Library; see the column COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
   Boston, MA 02110-1335, USA.

   Authors: Dave Camp <dave@ximian.com>
*/

#ifndef DORY_COLUMN_UTILITIES_H
#define DORY_COLUMN_UTILITIES_H

#include <libdory-extension/dory-column.h>
#include <libdory-private/dory-file.h>

GList *dory_get_all_columns       (void);
GList *dory_get_common_columns    (void);
GList *dory_get_columns_for_file (DoryFile *file);
GList *dory_column_list_copy      (GList       *columns);
void   dory_column_list_free      (GList       *columns);

GList *dory_sort_columns          (GList       *columns,
				       char       **column_order);


#endif /* DORY_COLUMN_UTILITIES_H */
