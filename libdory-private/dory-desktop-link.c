/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   dory-desktop-link.c: Class that handles the links on the desktop
    
   Copyright (C) 2003 Red Hat, Inc.
  
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
  
   Author: Alexander Larsson <alexl@redhat.com>
*/

#include <config.h>
#include "dory-desktop-link.h"
#include "dory-desktop-link-monitor.h"
#include "dory-desktop-icon-file.h"
#include "dory-directory-private.h"
#include "dory-desktop-directory.h"
#include "dory-icon-names.h"
#include <glib/gi18n.h>
#include <gio/gio.h>
#include <libdory-private/dory-file-utilities.h>
#include <libdory-private/dory-trash-monitor.h>
#include <libdory-private/dory-global-preferences.h>
#include <string.h>

struct DoryDesktopLinkDetails {
	DoryDesktopLinkType type;
        char *filename;
	char *display_name;
	GFile *activation_location;
	GIcon *icon;

	DoryDesktopIconFile *icon_file;
	
	GObject *signal_handler_obj;
	gulong signal_handler;

	/* Just for mount icons: */
	GMount *mount;
};

G_DEFINE_TYPE(DoryDesktopLink, dory_desktop_link, G_TYPE_OBJECT)

static void
create_icon_file (DoryDesktopLink *link)
{
	link->details->icon_file = dory_desktop_icon_file_new (link);
}

static void
dory_desktop_link_changed (DoryDesktopLink *link)
{
	if (link->details->icon_file != NULL) {
		dory_desktop_icon_file_update (link->details->icon_file);
	}
}

static void
mount_changed_callback (GMount *mount, DoryDesktopLink *link)
{
	g_free (link->details->display_name);
	if (link->details->activation_location) {
		g_object_unref (link->details->activation_location);
	}
	if (link->details->icon) {
		g_object_unref (link->details->icon);
	}
	
	link->details->display_name = g_mount_get_name (mount);
	link->details->activation_location = g_mount_get_default_location (mount);
	link->details->icon = g_mount_get_icon (mount);
	
	dory_desktop_link_changed (link);
}

static void
trash_state_changed_callback (DoryTrashMonitor *trash_monitor,
			      gboolean state,
			      gpointer callback_data)
{
	DoryDesktopLink *link;

	link = DORY_DESKTOP_LINK (callback_data);
	g_assert (link->details->type == DORY_DESKTOP_LINK_TRASH);

	if (link->details->icon) {
		g_object_unref (link->details->icon);
	}
	link->details->icon = dory_trash_monitor_get_icon ();

	dory_desktop_link_changed (link);
}

DoryDesktopLink *
dory_desktop_link_new (DoryDesktopLinkType type)
{
	DoryDesktopLink *link;

	link = DORY_DESKTOP_LINK (g_object_new (DORY_TYPE_DESKTOP_LINK, NULL));

	link->details->type = type;
	switch (type) {
	case DORY_DESKTOP_LINK_HOME:
		link->details->filename = g_strdup ("home");
		link->details->display_name = g_strdup (_("Home"));
		link->details->activation_location = g_file_new_for_path (g_get_home_dir ());
		link->details->icon = g_themed_icon_new (DORY_ICON_HOME);
		
		break;

	case DORY_DESKTOP_LINK_COMPUTER:
		link->details->filename = g_strdup ("computer");
		link->details->display_name = g_strdup (_("Computer"));
		link->details->activation_location = g_file_new_for_uri ("computer:///");
		/* TODO: This might need a different icon: */
		link->details->icon = g_themed_icon_new (DORY_ICON_COMPUTER);

		break;

	case DORY_DESKTOP_LINK_TRASH:
		link->details->filename = g_strdup ("trash");
		link->details->display_name = g_strdup (_("Trash"));
		link->details->activation_location = g_file_new_for_uri (EEL_TRASH_URI);
		link->details->icon = dory_trash_monitor_get_icon ();

		link->details->signal_handler_obj = G_OBJECT (dory_trash_monitor_get ());
		g_signal_connect_object (dory_trash_monitor_get (), "trash_state_changed",
						 G_CALLBACK (trash_state_changed_callback), link, 0);
		break;

	case DORY_DESKTOP_LINK_NETWORK:
		link->details->filename = g_strdup ("network");
		link->details->display_name = g_strdup (_("Network"));
		link->details->activation_location = g_file_new_for_uri ("network:///");
		link->details->icon = g_themed_icon_new (DORY_ICON_NETWORK);
		
		break;

	default:
	case DORY_DESKTOP_LINK_MOUNT:
		g_assert_not_reached();
	}

	create_icon_file (link);

	return link;
}

DoryDesktopLink *
dory_desktop_link_new_from_mount (GMount *mount)
{
	DoryDesktopLink *link;
	GVolume *volume;
	char *name, *filename;

	link = DORY_DESKTOP_LINK (g_object_new (DORY_TYPE_DESKTOP_LINK, NULL));

	link->details->type = DORY_DESKTOP_LINK_MOUNT;

	link->details->mount = g_object_ref (mount);

	/* We try to use the drive name to get somewhat stable filenames
	   for metadata */
	volume = g_mount_get_volume (mount);
	if (volume != NULL) {
		name = g_volume_get_name (volume);
		g_object_unref (volume);
	} else {
		name = g_mount_get_name (mount);
	}

	/* Replace slashes in name */
	filename = g_strconcat (g_strdelimit (name, "/", '-'), ".volume", NULL);
	link->details->filename =
		dory_desktop_link_monitor_make_filename_unique (dory_desktop_link_monitor_get (),
								    filename);
	g_free (filename);
	g_free (name);
	
	link->details->display_name = g_mount_get_name (mount);
	
	link->details->activation_location = g_mount_get_default_location (mount);
	link->details->icon = g_mount_get_icon (mount);
	
	link->details->signal_handler_obj = G_OBJECT (mount);
	link->details->signal_handler =
		g_signal_connect (mount, "changed",
				  G_CALLBACK (mount_changed_callback), link);
	
	create_icon_file (link);

	return link;
}

GMount *
dory_desktop_link_get_mount (DoryDesktopLink *link)
{
	if (link->details->mount) {
		return g_object_ref (link->details->mount);
	}
	return NULL;
}

DoryDesktopLinkType
dory_desktop_link_get_link_type (DoryDesktopLink *link)
{
	return link->details->type;
}

DoryFile *
dory_desktop_link_get_file (DoryDesktopLink *link)
{
    return DORY_FILE (link->details->icon_file);
}

char *
dory_desktop_link_get_file_name (DoryDesktopLink *link)
{
	return g_strdup (link->details->filename);
}

char *
dory_desktop_link_get_display_name (DoryDesktopLink *link)
{
	return g_strdup (link->details->display_name);
}

GIcon *
dory_desktop_link_get_icon (DoryDesktopLink *link)
{
	if (link->details->icon != NULL) {
		return g_object_ref (link->details->icon);
	}
	return NULL;
}

GFile *
dory_desktop_link_get_activation_location (DoryDesktopLink *link)
{
	if (link->details->activation_location) {
		return g_object_ref (link->details->activation_location);
	}
	return NULL;
}

char *
dory_desktop_link_get_activation_uri (DoryDesktopLink *link)
{
	if (link->details->activation_location) {
		return g_file_get_uri (link->details->activation_location);
	}
	return NULL;
}


gboolean
dory_desktop_link_get_date (DoryDesktopLink *link,
				DoryDateType     date_type,
				time_t               *date)
{
	return FALSE;
}

gboolean
dory_desktop_link_can_rename (DoryDesktopLink     *link)
{
	return !(link->details->type == DORY_DESKTOP_LINK_HOME ||
		link->details->type == DORY_DESKTOP_LINK_TRASH ||
		link->details->type == DORY_DESKTOP_LINK_NETWORK ||
		link->details->type == DORY_DESKTOP_LINK_COMPUTER ||
        link->details->type == DORY_DESKTOP_LINK_MOUNT);
}

gboolean
dory_desktop_link_rename (DoryDesktopLink     *link,
			      const char              *name)
{
	/* Do we want volume renaming? */
	return TRUE;
}

static void
dory_desktop_link_init (DoryDesktopLink *link)
{
	link->details = G_TYPE_INSTANCE_GET_PRIVATE (link,
						     DORY_TYPE_DESKTOP_LINK,
						     DoryDesktopLinkDetails);
}

static void
desktop_link_finalize (GObject *object)
{
	DoryDesktopLink *link;

	link = DORY_DESKTOP_LINK (object);

	if (link->details->signal_handler != 0) {
		g_signal_handler_disconnect (link->details->signal_handler_obj,
					     link->details->signal_handler);
	}

	if (link->details->icon_file != NULL) {
		dory_desktop_icon_file_remove (link->details->icon_file);
		dory_file_unref (DORY_FILE (link->details->icon_file));
		link->details->icon_file = NULL;
	}

	if (link->details->type == DORY_DESKTOP_LINK_MOUNT) {
		g_object_unref (link->details->mount);
	}

	g_free (link->details->filename);
	g_free (link->details->display_name);
	if (link->details->activation_location) {
		g_object_unref (link->details->activation_location);
	}
	if (link->details->icon) {
		g_object_unref (link->details->icon);
	}

	G_OBJECT_CLASS (dory_desktop_link_parent_class)->finalize (object);
}

static void
dory_desktop_link_class_init (DoryDesktopLinkClass *klass)
{
	GObjectClass *object_class;

	object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = desktop_link_finalize;

	g_type_class_add_private (object_class, sizeof(DoryDesktopLinkDetails));
}
