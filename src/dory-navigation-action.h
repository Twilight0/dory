/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*- */

/*
 *  Dory
 *
 *  Copyright (C) 2004 Red Hat, Inc.
 *  Copyright (C) 2003 Marco Pesenti Gritti
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
 *
 *  Based on ephy-navigation-action.h from Epiphany
 *
 *  Authors: Alexander Larsson <alexl@redhat.com>
 *           Marco Pesenti Gritti
 *
 */

#ifndef DORY_NAVIGATION_ACTION_H
#define DORY_NAVIGATION_ACTION_H

#include <gtk/gtk.h>

#define DORY_TYPE_NAVIGATION_ACTION            (dory_navigation_action_get_type ())
#define DORY_NAVIGATION_ACTION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_NAVIGATION_ACTION, DoryNavigationAction))
#define DORY_NAVIGATION_ACTION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_NAVIGATION_ACTION, DoryNavigationActionClass))
#define DORY_IS_NAVIGATION_ACTION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_NAVIGATION_ACTION))
#define DORY_IS_NAVIGATION_ACTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), DORY_TYPE_NAVIGATION_ACTION))
#define DORY_NAVIGATION_ACTION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), DORY_TYPE_NAVIGATION_ACTION, DoryNavigationActionClass))

typedef struct _DoryNavigationAction       DoryNavigationAction;
typedef struct _DoryNavigationActionClass  DoryNavigationActionClass;
typedef struct DoryNavigationActionPrivate DoryNavigationActionPrivate;

typedef enum
{
    DORY_NAVIGATION_DIRECTION_BACK,
    DORY_NAVIGATION_DIRECTION_FORWARD,
    DORY_NAVIGATION_DIRECTION_UP,
    DORY_NAVIGATION_DIRECTION_RELOAD,
    DORY_NAVIGATION_DIRECTION_HOME,
    DORY_NAVIGATION_DIRECTION_COMPUTER,
    DORY_NAVIGATION_DIRECTION_EDIT,

} DoryNavigationDirection;

struct _DoryNavigationAction
{
	GtkAction parent;
	
	/*< private >*/
	DoryNavigationActionPrivate *priv;
};

struct _DoryNavigationActionClass
{
	GtkActionClass parent_class;
};

GType    dory_navigation_action_get_type   (void);

#endif
