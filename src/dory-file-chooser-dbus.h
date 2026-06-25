/*
 * dory-file-chooser-dbus: Implementation for the org.Dory FileChooser D-Bus interface
 */

#ifndef __DORY_FILE_CHOOSER_DBUS_H__
#define __DORY_FILE_CHOOSER_DBUS_H__

#include <glib-object.h>

#define DORY_TYPE_FILE_CHOOSER_DBUS dory_file_chooser_dbus_get_type()
#define DORY_FILE_CHOOSER_DBUS(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_FILE_CHOOSER_DBUS, DoryFileChooserDBus))
#define DORY_FILE_CHOOSER_DBUS_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_FILE_CHOOSER_DBUS, DoryFileChooserDBusClass))
#define DORY_IS_FILE_CHOOSER_DBUS(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_FILE_CHOOSER_DBUS))
#define DORY_IS_FILE_CHOOSER_DBUS_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_FILE_CHOOSER_DBUS))
#define DORY_FILE_CHOOSER_DBUS_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_FILE_CHOOSER_DBUS, DoryFileChooserDBusClass))

typedef struct _DoryFileChooserDBus DoryFileChooserDBus;
typedef struct _DoryFileChooserDBusClass DoryFileChooserDBusClass;

GType dory_file_chooser_dbus_get_type (void);
DoryFileChooserDBus * dory_file_chooser_dbus_new (void);

#endif /* __DORY_FILE_CHOOSER_DBUS_H__ */
