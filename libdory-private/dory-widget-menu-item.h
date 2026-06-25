/* GTK - The GIMP Toolkit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/.
 */

#ifndef __DORY_WIDGET_MENU_ITEM_H__
#define __DORY_WIDGET_MENU_ITEM_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define DORY_TYPE_WIDGET_MENU_ITEM            (dory_widget_menu_item_get_type ())
#define DORY_WIDGET_MENU_ITEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_WIDGET_MENU_ITEM, DoryWidgetMenuItem))
#define DORY_WIDGET_MENU_ITEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_WIDGET_MENU_ITEM, DoryWidgetMenuItemClass))
#define DORY_IS_WIDGET_MENU_ITEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_WIDGET_MENU_ITEM))
#define DORY_IS_WIDGET_MENU_ITEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_WIDGET_MENU_ITEM))
#define DORY_WIDGET_MENU_ITEM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_WIDGET_MENU_ITEM, DoryWidgetMenuItemClass))


typedef struct _DoryWidgetMenuItem       DoryWidgetMenuItem;
typedef struct _DoryWidgetMenuItemClass  DoryWidgetMenuItemClass;

struct _DoryWidgetMenuItem
{
  GtkMenuItem menu_item;

  GtkWidget *child_widget;
  gint action_slot;

  /* GtkActivatable */
  GtkAction *related_action;
  gboolean use_action_appearance;
};

/**
 * DoryWidgetMenuItemClass:
 * @parent_class: The parent class.
 */
struct _DoryWidgetMenuItemClass
{
  GtkMenuItemClass parent_class;
};

GType      dory_widget_menu_item_get_type    (void) G_GNUC_CONST;
GtkWidget* dory_widget_menu_item_new             (GtkWidget *widget);

void       dory_widget_menu_item_set_child_widget (DoryWidgetMenuItem *widget_menu_item,
                                                   GtkWidget          *widget);

GtkWidget* dory_widget_menu_item_get_child_widget (DoryWidgetMenuItem *widget_menu_item);

G_END_DECLS

#endif /* __DORY_WIDGET_MENU_ITEM_H__ */
