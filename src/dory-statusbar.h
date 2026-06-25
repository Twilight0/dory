/* dory-statusbar.h
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
 * Boston, MA 02110-1335, USA.
 *
 * 
 */

#ifndef DORY_STATUSBAR_H
#define DORY_STATUSBAR_H

#include <gtk/gtk.h>
#include <gio/gio.h>
#include "dory-window.h"
#include "dory-window-slot.h"
#include "dory-view.h"

typedef struct _DoryStatusBar      DoryStatusBar;
typedef struct _DoryStatusBarClass DoryStatusBarClass;


#define DORY_TYPE_STATUS_BAR                 (dory_status_bar_get_type ())
#define DORY_STATUS_BAR(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_STATUS_BAR, DoryStatusBar))
#define DORY_STATUS_BAR_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_STATUS_BAR, DoryStatusBarClass))
#define DORY_IS_STATUS_BAR(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_STATUS_BAR))
#define DORY_IS_STATUS_BAR_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_STATUS_BAR))
#define DORY_STATUS_BAR_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_STATUS_BAR, DoryStatusBarClass))

#define DORY_STATUSBAR_ICON_SIZE_NAME "statusbar-icon"
#define DORY_STATUSBAR_ICON_SIZE 11

struct _DoryStatusBar
{
    GtkBox parent;
    DoryWindow *window;
    GtkWidget *real_statusbar;

    GtkWidget *zoom_slider;

    GtkWidget *tree_button;
    GtkWidget *places_button;
    GtkWidget *show_button;
    GtkWidget *hide_button;
    GtkWidget *separator;
};

struct _DoryStatusBarClass
{
    GtkBoxClass parent_class;
};

GType    dory_status_bar_get_type (void) G_GNUC_CONST;

GtkWidget *dory_status_bar_new (DoryWindow *window);

GtkWidget *dory_status_bar_get_real_statusbar (DoryStatusBar *bar);

void       dory_status_bar_sync_button_states (DoryStatusBar *bar);

void       dory_status_bar_sync_zoom_widgets (DoryStatusBar *bar);

#endif /* DORY_STATUSBAR_H */
