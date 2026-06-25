/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 * dory-malloc-utils.h - allocator tuning and heap-trim debounce.
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
 */

#ifndef DORY_MALLOC_UTILS_H
#define DORY_MALLOC_UTILS_H

#include <glib.h>

G_BEGIN_DECLS

void dory_malloc_setup        (void);
void dory_schedule_heap_trim  (void);

G_END_DECLS

#endif /* DORY_MALLOC_UTILS_H */
