/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-
 
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

#ifndef DORY_WIDGET_ACTION_H
#define DORY_WIDGET_ACTION_H

#include <gtk/gtk.h>
#include <glib.h>

#define DORY_TYPE_WIDGET_ACTION dory_widget_action_get_type()
#define DORY_WIDGET_ACTION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_WIDGET_ACTION, DoryWidgetAction))
#define DORY_WIDGET_ACTION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_WIDGET_ACTION, DoryWidgetActionClass))
#define DORY_IS_WIDGET_ACTION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_WIDGET_ACTION))
#define DORY_IS_WIDGET_ACTION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_WIDGET_ACTION))
#define DORY_WIDGET_ACTION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_WIDGET_ACTION, DoryWidgetActionClass))

typedef struct _DoryWidgetAction DoryWidgetAction;
typedef struct _DoryWidgetActionClass DoryWidgetActionClass;

struct _DoryWidgetAction {
    GtkAction parent;
    GtkWidget *widget_a;
    GtkWidget *widget_b;
    gboolean a_used;
    gboolean b_used;
};

struct _DoryWidgetActionClass {
	GtkActionClass parent_class;
};

enum {
  ACTION_SLOT_A = 0,
  ACTION_SLOT_B
};

GType         dory_widget_action_get_type             (void);
GtkAction    *dory_widget_action_new                  (const gchar *name,
                                                       GtkWidget *widget_a,
                                                       GtkWidget *widget_b);
void          dory_widget_action_activate             (DoryWidgetAction *action);
GtkWidget *   dory_widget_action_get_widget_a (DoryWidgetAction *action);
void          dory_widget_action_set_widget_a (DoryWidgetAction *action, GtkWidget *widget);
GtkWidget *   dory_widget_action_get_widget_b (DoryWidgetAction *action);
void          dory_widget_action_set_widget_b (DoryWidgetAction *action, GtkWidget *widget);
#endif /* DORY_WIDGET_ACTION_H */
