/*
 *  dory-column.h - Info columns exported by 
 *                      DoryColumnProvider objects.
 *
 *  Copyright (C) 2003 Novell, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin Street, Suite 500, MA 02110-1335, USA.
 * 
 *  Author:  Dave Camp <dave@ximian.com>
 *
 */

#ifndef DORY_COLUMN_H
#define DORY_COLUMN_H

#include <glib-object.h>
#include <pango/pango.h>

#include "dory-extension-types.h"

G_BEGIN_DECLS

#define DORY_TYPE_COLUMN            dory_column_get_type()

G_DECLARE_FINAL_TYPE (DoryColumn, dory_column, DORY, COLUMN, GObject)

DoryColumn *  dory_column_new             (const char     *name,
                                           const char     *attribute,
                                           const char     *label,
                                           const char     *description);
DoryColumn *  dory_column_new2 (const char         *name,
                                const char         *attribute,
                                const char         *label,
                                const char         *description,
                                gint                width_chars,
                                PangoEllipsizeMode  ellipsize);

/* DoryColumn has the following properties:
 *   name (string)        - the identifier for the column
 *   attribute (string)   - the file attribute to be displayed in the 
 *                          column
 *   label (string)       - the user-visible label for the column
 *   description (string) - a user-visible description of the column
 *   xalign (float)       - x-alignment of the column 
 *   width-chars (int)      default width
 *   ellipsize (PangoEllipsizeMode  whether to ellipsize when truncating text
 */

G_END_DECLS

#endif
