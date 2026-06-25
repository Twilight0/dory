/*
 *  dory-menu.h - Menus exported by DoryMenuProvider objects.
 *
 *  Copyright (C) 2005 Raffaele Sandrini
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
 *  Author:  Raffaele Sandrini <rasa@gmx.ch>
 *
 */

#include <config.h>
#include "dory-menu.h"
#include "dory-extension-i18n.h"

#include <glib.h>

typedef struct {
	GList *item_list;
} DoryMenuPrivate;

struct _DoryMenu
{
    GObject parent_class;

    DoryMenuPrivate *priv;
};

G_DEFINE_TYPE_WITH_PRIVATE (DoryMenu, dory_menu, G_TYPE_OBJECT)

/**
 * SECTION:dory-menu
 * @Title: DoryMenu
 * @Short_description: A menu added to Dory's context menus by an extension
 *
 * Menu items and submenus can be added to Dory's selected item and background
 * context menus by a #DoryMenuProvider.  Separators and embedded widgets are also
 * possible (see #DorySimpleButton.)
 **/

void
dory_menu_append_item (DoryMenu *menu, DoryMenuItem *item)
{
	g_return_if_fail (menu != NULL);
	g_return_if_fail (item != NULL);
	
	menu->priv->item_list = g_list_append (menu->priv->item_list, g_object_ref (item));
}

/**
 * dory_menu_get_items:
 * @menu: a #DoryMenu
 *
 * Returns: (element-type DoryMenuItem) (transfer full): the provided #DoryMenuItem list
 */
GList *
dory_menu_get_items (DoryMenu *menu)
{
	GList *item_list;

	g_return_val_if_fail (menu != NULL, NULL);
	
	item_list = g_list_copy (menu->priv->item_list);
	g_list_foreach (item_list, (GFunc)g_object_ref, NULL);
	
	return item_list;
}

/**
 * dory_menu_item_list_free:
 * @item_list: (element-type DoryMenuItem): a list of #DoryMenuItem
 *
 */
void
dory_menu_item_list_free (GList *item_list)
{
	g_return_if_fail (item_list != NULL);
	
	g_list_foreach (item_list, (GFunc)g_object_unref, NULL);
	g_list_free (item_list);
}

/* Type initialization */

static void
dory_menu_finalize (GObject *object)
{
	DoryMenu *menu = DORY_MENU (object);

	if (menu->priv->item_list) {
        dory_menu_item_list_free (menu->priv->item_list);
	}

	G_OBJECT_CLASS (dory_menu_parent_class)->finalize (object);
}

static void
dory_menu_init (DoryMenu *menu)
{
	menu->priv = G_TYPE_INSTANCE_GET_PRIVATE (menu, DORY_TYPE_MENU, DoryMenuPrivate);

    menu->priv->item_list = NULL;
}

static void
dory_menu_class_init (DoryMenuClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    object_class->finalize = dory_menu_finalize;
}

/* public constructors */

DoryMenu *
dory_menu_new (void)
{
	DoryMenu *obj;
	
	obj = DORY_MENU (g_object_new (DORY_TYPE_MENU, NULL));
	
	return obj;
}
