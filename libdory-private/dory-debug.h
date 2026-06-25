/*
 * dory-debug: debug loggers for dory
 *
 * Copyright (C) 2007 Collabora Ltd.
 * Copyright (C) 2007 Nokia Corporation
 * Copyright (C) 2010 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Based on Empathy's empathy-debug.
 */

#ifndef __DORY_DEBUG_H__
#define __DORY_DEBUG_H__

#include <config.h>
#include <glib.h>

G_BEGIN_DECLS

#ifdef ENABLE_DEBUG

typedef enum {
  DORY_DEBUG_APPLICATION = 1 << 1,
  DORY_DEBUG_BOOKMARKS = 1 << 2,
  DORY_DEBUG_DBUS = 1 << 3,
  DORY_DEBUG_DIRECTORY_VIEW = 1 << 4,
  DORY_DEBUG_FILE = 1 << 5,
  DORY_DEBUG_ICON_CONTAINER = 1 << 6,
  DORY_DEBUG_ICON_VIEW = 1 << 7,
  DORY_DEBUG_LIST_VIEW = 1 << 8,
  DORY_DEBUG_MIME = 1 << 9,
  DORY_DEBUG_PLACES = 1 << 10,
  DORY_DEBUG_PREVIEWER = 1 << 11,
  DORY_DEBUG_SMCLIENT = 1 << 12,
  DORY_DEBUG_WINDOW = 1 << 13,
  DORY_DEBUG_UNDO = 1 << 14,
  DORY_DEBUG_ACTIONS = 1 << 15,
  DORY_DEBUG_DESKTOP = 1 << 16,
  DORY_DEBUG_THUMBNAILS = 1 << 17,
  DORY_DEBUG_SEARCH = 1 << 18,
  DORY_DEBUG_PREFERENCES = 1 << 19
} DebugFlags;

void dory_debug_set_flags (DebugFlags flags);
gboolean dory_debug_flag_is_set (DebugFlags flag);

void dory_debug_valist (DebugFlags flag,
                            const gchar *format, va_list args);

void dory_debug (DebugFlags flag, const gchar *format, ...)
  G_GNUC_PRINTF (2, 3);

void dory_debug_files (DebugFlags flag, GList *files,
                           const gchar *format, ...) G_GNUC_PRINTF (3, 4);

#ifdef DEBUG_FLAG

#define DEBUG(format, ...) \
  dory_debug (DEBUG_FLAG, "%s: %s: " format, G_STRFUNC, G_STRLOC, \
                  ##__VA_ARGS__)

#define DEBUG_FILES(files, format, ...) \
  dory_debug_files (DEBUG_FLAG, files, "%s:" format, G_STRFUNC, \
                        ##__VA_ARGS__)

#define DEBUGGING dory_debug_flag_is_set(DEBUG_FLAG)

#endif /* DEBUG_FLAG */

#else /* ENABLE_DEBUG */

#ifdef DEBUG_FLAG

#define DEBUG(format, ...) \
  G_STMT_START { } G_STMT_END

#define DEBUG_FILES(files, format, ...) \
  G_STMT_START { } G_STMT_END

#define DEBUGGING 0

#endif /* DEBUG_FLAG */

#endif /* ENABLE_DEBUG */

G_END_DECLS

#endif /* __DORY_DEBUG_H__ */
