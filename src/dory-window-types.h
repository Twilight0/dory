/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*- */

/*
 *  dory-window-types: typedefs for window-related types.
 *
 *  Copyright (C) 1999, 2000, 2010 Red Hat, Inc.
 *  Copyright (C) 1999, 2000, 2001 Eazel, Inc.
 *
 *  Dory is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  Dory is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Suite 500, MA 02110-1335, USA.
 *
 *  Authors: Elliot Lee <sopwith@redhat.com>
 *           Darin Adler <darin@bentspoon.com>
 *
 */

#ifndef __DORY_WINDOW_TYPES_H__
#define __DORY_WINDOW_TYPES_H__

typedef struct _DoryWindowPane DoryWindowPane;
typedef struct _DoryWindowPaneClass DoryWindowPaneClass;

typedef struct DoryWindow DoryWindow;

typedef struct DoryWindowSlot DoryWindowSlot;
typedef struct DoryWindowSlotClass DoryWindowSlotClass;

typedef void (* DoryWindowGoToCallback) (DoryWindow *window,
                                             GError *error,
                                             gpointer user_data);

typedef enum {
        DORY_WINDOW_OPEN_FLAG_CLOSE_BEHIND = 1<<0,
        DORY_WINDOW_OPEN_FLAG_NEW_WINDOW = 1<<1,
        DORY_WINDOW_OPEN_FLAG_NEW_TAB = 1<<2,
        DORY_WINDOW_OPEN_FLAG_SEARCH = 1<<3,
        DORY_WINDOW_OPEN_FLAG_MOUNT = 1<<4
} DoryWindowOpenFlags;

#endif /* __DORY_WINDOW_TYPES_H__ */
