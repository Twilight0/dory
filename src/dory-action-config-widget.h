/* dory-action-config-widget.h */

/*  A widget that displays a list of actions to enable or disable.
 *  This is usually part of a DoryPluginManagerWidget
 */

#ifndef __DORY_ACTION_CONFIG_WIDGET_H__
#define __DORY_ACTION_CONFIG_WIDGET_H__

#include <glib-object.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include "dory-config-base-widget.h"

G_BEGIN_DECLS

#define DORY_TYPE_ACTION_CONFIG_WIDGET (dory_action_config_widget_get_type())

#define DORY_ACTION_CONFIG_WIDGET(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_ACTION_CONFIG_WIDGET, DoryActionConfigWidget))
#define DORY_ACTION_CONFIG_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_ACTION_CONFIG_WIDGET, DoryActionConfigWidgetClass))
#define DORY_IS_ACTION_CONFIG_WIDGET(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_ACTION_CONFIG_WIDGET))
#define DORY_IS_ACTION_CONFIG_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_ACTION_CONFIG_WIDGET))
#define DORY_ACTION_CONFIG_WIDGET_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_ACTION_CONFIG_WIDGET, DoryActionConfigWidgetClass))

typedef struct _DoryActionConfigWidget DoryActionConfigWidget;
typedef struct _DoryActionConfigWidgetClass DoryActionConfigWidgetClass;

struct _DoryActionConfigWidget
{
  DoryConfigBaseWidget parent;

  GList *actions;

  GList *dir_monitors;
  gulong bl_handler;
};

struct _DoryActionConfigWidgetClass
{
  DoryConfigBaseWidgetClass parent_class;
};

GType dory_action_config_widget_get_type (void);

GtkWidget  *dory_action_config_widget_new                   (void);

G_END_DECLS

#endif /* __DORY_ACTION_CONFIG_WIDGET_H__ */
