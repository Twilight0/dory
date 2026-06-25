/*
 * dory-file-chooser-dialog: Custom layout file picker dialog header
 */

#ifndef __DORY_FILE_CHOOSER_DIALOG_H__
#define __DORY_FILE_CHOOSER_DIALOG_H__

#include <gtk/gtk.h>

GtkWidget * dory_file_chooser_dialog_new (const gchar *title,
                                          GtkFileChooserAction action,
                                          gboolean select_multiple,
                                          const gchar *initial_folder_uri,
                                          const gchar *suggested_name);

GSList * dory_file_chooser_dialog_get_selected_uris (GtkDialog *dialog);
gchar *  dory_file_chooser_dialog_get_selected_uri  (GtkDialog *dialog);

#endif /* __DORY_FILE_CHOOSER_DIALOG_H__ */
