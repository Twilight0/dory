/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Dory
 *
 * Copyright (C) 2005 Red Hat, Inc.
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, MA 02110-1335, USA.
 *
 * Author:  Alexander Larsson <alexl@redhat.com>
 */

#ifndef DORY_WINDOW_BOOKMARKS_H
#define DORY_WINDOW_BOOKMARKS_H

#include <libdory-private/dory-bookmark.h>
#include <dory-window.h>
#include "dory-bookmark-list.h"

void                  dory_bookmarks_exiting                        (void);
void                  dory_window_add_bookmark_for_current_location (DoryWindow *window);
void                  dory_window_edit_bookmarks                    (DoryWindow *window);
void                  dory_window_initialize_bookmarks_menu         (DoryWindow *window);

#endif
