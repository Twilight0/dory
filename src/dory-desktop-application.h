/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * dory-application: main Dory application class.
 *
 * Copyright (C) 2000 Red Hat, Inc.
 * Copyright (C) 2010 Cosimo Cecchi <cosimoc@gnome.org>
 *
 * Dory is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Dory is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
 * Boston, MA 02110-1335, USA.
 */

#ifndef __DORY_DESKTOP_APPLICATION_H__
#define __DORY_DESKTOP_APPLICATION_H__

#include <gdk/gdk.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

#include <libdory-private/dory-undo-manager.h>

#include "dory-window.h"
#include "dory-application.h"

#define DORY_TYPE_DESKTOP_APPLICATION dory_desktop_application_get_type()
#define DORY_DESKTOP_APPLICATION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_DESKTOP_APPLICATION, DoryDesktopApplication))
#define DORY_DESKTOP_APPLICATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_DESKTOP_APPLICATION, DoryDesktopApplicationClass))
#define DORY_IS_DESKTOP_APPLICATION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_DESKTOP_APPLICATION))
#define DORY_IS_DESKTOP_APPLICATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_DESKTOP_APPLICATION))
#define DORY_DESKTOP_APPLICATION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_DESKTOP_APPLICATION, DoryDesktopApplicationClass))

typedef struct _DoryDesktopApplicationPriv DoryDesktopApplicationPriv;

typedef struct {
	DoryApplication parent;

	DoryDesktopApplicationPriv *priv;
} DoryDesktopApplication;

typedef struct {
	DoryApplicationClass parent_class;
} DoryDesktopApplicationClass;

GType dory_desktop_application_get_type (void);

DoryApplication *dory_desktop_application_get_singleton (void);

#endif /* __DORY_DESKTOP_APPLICATION_H__ */
