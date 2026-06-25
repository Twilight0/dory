/*
 * dory-freedesktop-dbus: Implementation for the org.freedesktop DBus file-management interfaces
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
 * Authors: Akshay Gupta <kitallis@gmail.com>
 *          Federico Mena Quintero <federico@gnome.org>
 */


#ifndef __DORY_FREEDESKTOP_DBUS_H__
#define __DORY_FREEDESKTOP_DBUS_H__

#include <glib-object.h>

#define DORY_TYPE_FREEDESKTOP_DBUS dory_freedesktop_dbus_get_type()
#define DORY_FREEDESKTOP_DBUS(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_FREEDESKTOP_DBUS, DoryFreedesktopDBus))
#define DORY_FREEDESKTOP_DBUS_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_FREEDESKTOP_DBUS, DoryFreedesktopDBusClass))
#define DORY_IS_FREEDESKTOP_DBUS(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_FREEDESKTOP_DBUS))
#define DORY_IS_FREEDESKTOP_DBUS_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_FREEDESKTOP_DBUS))
#define DORY_FREEDESKTOP_DBUS_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_FREEDESKTOP_DBUS, DoryFreedesktopDBusClass))

typedef struct _DoryFreedesktopDBus DoryFreedesktopDBus;
typedef struct _DoryFreedesktopDBusClass DoryFreedesktopDBusClass;

GType dory_freedesktop_dbus_get_type (void);
DoryFreedesktopDBus * dory_freedesktop_dbus_new (void);

void dory_freedesktop_dbus_set_open_locations (DoryFreedesktopDBus *fdb, const gchar **locations);

#endif /* __DORY_FREEDESKTOP_DBUS_H__ */
