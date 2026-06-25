/* dory-script-config-widget.h */

/*  A widget that displays a list of scripts to enable or disable.
 *  This is usually part of a DoryPluginManagerWidget
 */

#ifndef __DORY_TEMPLATE_CONFIG_WIDGET_H__
#define __DORY_TEMPLATE_CONFIG_WIDGET_H__

#include <glib-object.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include "dory-config-base-widget.h"

G_BEGIN_DECLS

#define DORY_TYPE_TEMPLATE_CONFIG_WIDGET (dory_template_config_widget_get_type())

#define DORY_TEMPLATE_CONFIG_WIDGET(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_TEMPLATE_CONFIG_WIDGET, DoryTemplateConfigWidget))
#define DORY_TEMPLATE_CONFIG_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_TEMPLATE_CONFIG_WIDGET, DoryTemplateConfigWidgetClass))
#define DORY_IS_TEMPLATE_CONFIG_WIDGET(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_TEMPLATE_CONFIG_WIDGET))
#define DORY_IS_TEMPLATE_CONFIG_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_TEMPLATE_CONFIG_WIDGET))
#define DORY_TEMPLATE_CONFIG_WIDGET_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_TEMPLATE_CONFIG_WIDGET, DoryTemplateConfigWidgetClass))

typedef struct _DoryTemplateConfigWidget DoryTemplateConfigWidget;
typedef struct _DoryTemplateConfigWidgetClass DoryTemplateConfigWidgetClass;

struct _DoryTemplateConfigWidget
{
  DoryConfigBaseWidget parent;

  GList *templates;

  GList *dir_monitors;
  GtkWidget *remove_button;
  GtkWidget *rename_button;
  GtkWidget *edit_button;
};

struct _DoryTemplateConfigWidgetClass
{
  DoryConfigBaseWidgetClass parent_class;
};

GType dory_template_config_widget_get_type (void);

GtkWidget  *dory_template_config_widget_new                   (void);

G_END_DECLS

#endif /* __DORY_TEMPLATE_CONFIG_WIDGET_H__ */
