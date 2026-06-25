/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* 
 * Copyright (C) 2004 Red Hat, Inc
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
 * Boston, MA 02110-1335, USA.
 *
 * Author: Alexander Larsson <alexl@redhat.com>
 */

#ifndef DORY_IMAGE_PROPERTIES_PAGE_H
#define DORY_IMAGE_PROPERTIES_PAGE_H

#include <gtk/gtk.h>

#define DORY_TYPE_IMAGE_PROPERTIES_PAGE dory_image_properties_page_get_type()
#define DORY_IMAGE_PROPERTIES_PAGE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_IMAGE_PROPERTIES_PAGE, DoryImagePropertiesPage))
#define DORY_IMAGE_PROPERTIES_PAGE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_IMAGE_PROPERTIES_PAGE, DoryImagePropertiesPageClass))
#define DORY_IS_IMAGE_PROPERTIES_PAGE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_IMAGE_PROPERTIES_PAGE))
#define DORY_IS_IMAGE_PROPERTIES_PAGE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_IMAGE_PROPERTIES_PAGE))
#define DORY_IMAGE_PROPERTIES_PAGE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_IMAGE_PROPERTIES_PAGE, DoryImagePropertiesPageClass))

typedef struct DoryImagePropertiesPageDetails DoryImagePropertiesPageDetails;

typedef struct {
	GtkBox parent;
	DoryImagePropertiesPageDetails *details;
} DoryImagePropertiesPage;

typedef struct {
	GtkBoxClass parent;
} DoryImagePropertiesPageClass;

GType dory_image_properties_page_get_type (void);
void  dory_image_properties_page_register (void);

#endif /* DORY_IMAGE_PROPERTIES_PAGE_H */
