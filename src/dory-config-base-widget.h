/* dory-config-base-widget.h */

/*  A base widget class for extension/action/script config widgets.
 *  This is usually part of a DoryPluginManagerWidget
 */

#ifndef __DORY_CONFIG_BASE_WIDGET_H__
#define __DORY_CONFIG_BASE_WIDGET_H__

#include <glib-object.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include "dory-window-private.h"

G_BEGIN_DECLS

#define DORY_TYPE_CONFIG_BASE_WIDGET (dory_config_base_widget_get_type())

#define DORY_CONFIG_BASE_WIDGET(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_CONFIG_BASE_WIDGET, DoryConfigBaseWidget))
#define DORY_CONFIG_BASE_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_CONFIG_BASE_WIDGET, DoryConfigBaseWidgetClass))
#define DORY_IS_CONFIG_BASE_WIDGET(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_CONFIG_BASE_WIDGET))
#define DORY_IS_CONFIG_BASE_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_CONFIG_BASE_WIDGET))
#define DORY_CONFIG_BASE_WIDGET_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_CONFIG_BASE_WIDGET, DoryConfigBaseWidgetClass))

typedef struct _DoryConfigBaseWidget DoryConfigBaseWidget;
typedef struct _DoryConfigBaseWidgetClass DoryConfigBaseWidgetClass;

struct _DoryConfigBaseWidget
{
  GtkBin parent;

  GtkWidget *label;
  GtkWidget *listbox;
  GtkWidget *lbuttonbox;
  GtkWidget *rbuttonbox;
  GtkWidget *enable_button;
  GtkWidget *disable_button;
};

struct _DoryConfigBaseWidgetClass
{
  GtkBinClass parent_class;
};

GType dory_config_base_widget_get_type (void);

GtkWidget *dory_config_base_widget_get_label          (DoryConfigBaseWidget *widget);
GtkWidget *dory_config_base_widget_get_listbox        (DoryConfigBaseWidget *widget);
GtkWidget *dory_config_base_widget_get_enable_button  (DoryConfigBaseWidget *widget);
GtkWidget *dory_config_base_widget_get_disable_button (DoryConfigBaseWidget *widget);

void       dory_config_base_widget_set_default_buttons_sensitive (DoryConfigBaseWidget *widget, gboolean sensitive);

void       dory_config_base_widget_clear_list         (DoryConfigBaseWidget *widget);
DoryWindow *dory_config_base_widget_get_view_window   (DoryConfigBaseWidget *widget, DoryWindow *view_window);

G_END_DECLS

#endif /* __DORY_CONFIG_BASE_WIDGET_H__ */
