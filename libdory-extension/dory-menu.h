/*
 *  dory-menu.h - Menus exported by DoryMenuProvider objects.
 *
 *  Copyright (C) 2005 Raffaele Sandrini
 *  Copyright (C) 2003 Novell, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin Street, Suite 500, MA 02110-1335, USA.
 * 
 *  Author:  Dave Camp <dave@ximian.com>
 *           Raffaele Sandrini <rasa@gmx.ch>
 *
 */

#ifndef DORY_MENU_H
#define DORY_MENU_H

#include <glib-object.h>
#include <gtk/gtk.h>
#include "dory-extension-types.h"

G_BEGIN_DECLS

/* DoryMenu defines */
#define DORY_TYPE_MENU         dory_menu_get_type ()
G_DECLARE_FINAL_TYPE (DoryMenu, dory_menu, DORY, MENU, GObject)
/* DoryMenuItem defines */
#define DORY_TYPE_MENU_ITEM    dory_menu_item_get_type()
G_DECLARE_FINAL_TYPE (DoryMenuItem, dory_menu_item, DORY, MENU_ITEM, GObject)

/* DoryMenu methods */
DoryMenu *	dory_menu_new	(void);

void	dory_menu_append_item	(DoryMenu      *menu,
					 DoryMenuItem  *item);
GList*	dory_menu_get_items		(DoryMenu *menu);
void	dory_menu_item_list_free	(GList *item_list);

/* DoryMenuItem methods */
DoryMenuItem *dory_menu_item_new           (const char       *name,
						    const char       *label,
						    const char       *tip,
						    const char       *icon);

DoryMenuItem *dory_menu_item_new_widget (const char *name,
                                         GtkWidget  *widget_a,
                                         GtkWidget  *widget_b);

DoryMenuItem *dory_menu_item_new_separator (const char *name);

void dory_menu_item_set_widget_a (DoryMenuItem *item, GtkWidget *widget);
void dory_menu_item_set_widget_b (DoryMenuItem *item, GtkWidget *widget);

void              dory_menu_item_activate      (DoryMenuItem *item);
void              dory_menu_item_set_submenu   (DoryMenuItem *item,
						    DoryMenu     *menu);
/* DoryMenuItem has the following properties:
 *   name (string)        - the identifier for the menu item
 *   label (string)       - the user-visible label of the menu item
 *   tip (string)         - the tooltip of the menu item 
 *   icon (string)        - the name of the icon to display in the menu item
 *   sensitive (boolean)  - whether the menu item is sensitive or not
 *   priority (boolean)   - used for toolbar items, whether to show priority
 *                          text.
 *   menu (DoryMenu)  - The menu belonging to this item. May be null.
 *   widget (GtkWidget) - The optional widget to use in place of a normal menu entr
 */

G_END_DECLS

#endif /* DORY_MENU_H */
