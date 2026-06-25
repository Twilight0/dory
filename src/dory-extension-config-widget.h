/* dory-extension-config-widget.h */

/*  A widget that displays a list of extensions to enable or disable.
 *  This is usually part of a DoryPluginManagerWidget
 */

#ifndef __DORY_EXTENSION_CONFIG_WIDGET_H__
#define __DORY_EXTENSION_CONFIG_WIDGET_H__

#include <glib-object.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include "dory-config-base-widget.h"

G_BEGIN_DECLS

#define DORY_TYPE_EXTENSION_CONFIG_WIDGET (dory_extension_config_widget_get_type())

#define DORY_EXTENSION_CONFIG_WIDGET(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_EXTENSION_CONFIG_WIDGET, DoryExtensionConfigWidget))
#define DORY_EXTENSION_CONFIG_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_EXTENSION_CONFIG_WIDGET, DoryExtensionConfigWidgetClass))
#define DORY_IS_EXTENSION_CONFIG_WIDGET(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_EXTENSION_CONFIG_WIDGET))
#define DORY_IS_EXTENSION_CONFIG_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_EXTENSION_CONFIG_WIDGET))
#define DORY_EXTENSION_CONFIG_WIDGET_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_EXTENSION_CONFIG_WIDGET, DoryExtensionConfigWidgetClass))

typedef struct _DoryExtensionConfigWidget DoryExtensionConfigWidget;
typedef struct _DoryExtensionConfigWidgetClass DoryExtensionConfigWidgetClass;

struct _DoryExtensionConfigWidget
{
  DoryConfigBaseWidget parent;
  GtkWidget *restart_button;

  GList *current_extensions;
  GList *initial_extension_ids;

  gulong bl_handler;
};

struct _DoryExtensionConfigWidgetClass
{
  DoryConfigBaseWidgetClass parent_class;
};

GType dory_extension_config_widget_get_type (void);

GtkWidget  *dory_extension_config_widget_new                   (void);

G_END_DECLS

#endif /* __DORY_EXTENSION_CONFIG_WIDGET_H__ */
