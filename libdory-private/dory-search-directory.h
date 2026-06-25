/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   dory-search-directory.h: Subclass of DoryDirectory to implement
   a virtual directory consisting of the search directory and the search
   icons
 
   Copyright (C) 2005 Novell, Inc
  
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
*/

#ifndef DORY_SEARCH_DIRECTORY_H
#define DORY_SEARCH_DIRECTORY_H

#include <libdory-private/dory-directory.h>
#include <libdory-private/dory-query.h>

#define DORY_TYPE_SEARCH_DIRECTORY dory_search_directory_get_type()
#define DORY_SEARCH_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_SEARCH_DIRECTORY, DorySearchDirectory))
#define DORY_SEARCH_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_SEARCH_DIRECTORY, DorySearchDirectoryClass))
#define DORY_IS_SEARCH_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_SEARCH_DIRECTORY))
#define DORY_IS_SEARCH_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_SEARCH_DIRECTORY))
#define DORY_SEARCH_DIRECTORY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_SEARCH_DIRECTORY, DorySearchDirectoryClass))

typedef struct DorySearchDirectoryDetails DorySearchDirectoryDetails;

typedef struct {
	DoryDirectory parent_slot;
	DorySearchDirectoryDetails *details;
} DorySearchDirectory;

typedef struct {
	DoryDirectoryClass parent_slot;
} DorySearchDirectoryClass;

GType   dory_search_directory_get_type             (void);

char   *dory_search_directory_generate_new_uri     (void);

DorySearchDirectory *dory_search_directory_new_from_saved_search (const char *uri);

gboolean       dory_search_directory_is_saved_search (DorySearchDirectory *search);
gboolean       dory_search_directory_is_modified     (DorySearchDirectory *search);
void           dory_search_directory_save_search     (DorySearchDirectory *search);
void           dory_search_directory_save_to_file    (DorySearchDirectory *search,
							  const char              *save_file_uri);

DoryQuery *dory_search_directory_get_query       (DorySearchDirectory *search);
void           dory_search_directory_set_query       (DorySearchDirectory *search,
							  DoryQuery           *query);

#endif /* DORY_SEARCH_DIRECTORY_H */
