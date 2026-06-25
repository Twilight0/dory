/* dory-plugin-manager.h */

/*  A GtkWidget that can be inserted into a UI that provides a simple interface for
 *  managing the loading of extensions, actions and scripts
 */

#ifndef __DORY_PLUGIN_MANAGER_H__
#define __DORY_PLUGIN_MANAGER_H__

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define DORY_TYPE_PLUGIN_MANAGER (dory_plugin_manager_get_type())

G_DECLARE_FINAL_TYPE (DoryPluginManager, dory_plugin_manager, DORY, PLUGIN_MANAGER, GtkBin)

DoryPluginManager       *dory_plugin_manager_new                   (void);

G_END_DECLS

#endif /* __DORY_PLUGIN_MANAGER_H__ */
