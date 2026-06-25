/* dory-simple-button.h */

#ifndef __DORY_SIMPLE_BUTTON_H__
#define __DORY_SIMPLE_BUTTON_H__

#include <glib-object.h>
#include <gtk/gtk.h>
#include "dory-extension-types.h"

G_BEGIN_DECLS

#define DORY_TYPE_SIMPLE_BUTTON dory_simple_button_get_type()

G_DECLARE_FINAL_TYPE (DorySimpleButton, dory_simple_button, DORY, SIMPLE_BUTTON, GtkButton)

DorySimpleButton *dory_simple_button_new (void);
DorySimpleButton *dory_simple_button_new_from_icon_name (const gchar *icon_name, int icon_size);
DorySimpleButton *dory_simple_button_new_from_stock (const gchar *stock_id, int icon_size);
DorySimpleButton *dory_simple_button_new_from_file (const gchar *path, int icon_size);

G_END_DECLS

#endif /* __DORY_SIMPLE_BUTTON_H__ */
