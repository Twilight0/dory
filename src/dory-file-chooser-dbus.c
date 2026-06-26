/*
 * dory-file-chooser-dbus: D-Bus service provider for org.Dory.FileChooser
 */

#include <config.h>
#include <gio/gio.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include "dory-application.h"
#include "dory-file-chooser-dbus.h"
#include "dory-file-chooser-dialog.h"
#include "dory-file-chooser-generated.h"

struct _DoryFileChooserDBus {
    GObject parent;
    guint owner_id;
    DoryFileChooser *skeleton;
};

struct _DoryFileChooserDBusClass {
    GObjectClass parent_class;
};

G_DEFINE_TYPE (DoryFileChooserDBus, dory_file_chooser_dbus, G_TYPE_OBJECT);

typedef struct {
    GDBusMethodInvocation *invocation;
    GtkWidget *dialog;
    DoryFileChooser *skeleton;
} ResponseData;

static void
on_open_dialog_response (GtkDialog *dialog, gint response_id, gpointer user_data)
{
    ResponseData *data = user_data;
    GPtrArray *results = g_ptr_array_new_with_free_func (g_free);

    g_message ("Dory Open Dialog Response ID: %d", response_id);

    if (response_id == GTK_RESPONSE_ACCEPT || response_id == GTK_RESPONSE_OK) {
        GSList *uris = dory_file_chooser_dialog_get_selected_uris (dialog);
        GSList *l;
        for (l = uris; l != NULL; l = l->next) {
            g_message ("Dory Dialog selected URI: %s", (const gchar *)l->data);
            g_ptr_array_add (results, g_strdup (l->data));
        }
        g_slist_free_full (uris, g_free);
    }

    g_ptr_array_add (results, NULL); // NULL terminator for const gchar *const * parameter

    dory_file_chooser_complete_open_file (data->skeleton, data->invocation, (const gchar *const *)results->pdata);
    
    g_ptr_array_unref (results);
    gtk_widget_destroy (GTK_WIDGET (dialog));
    g_application_release (g_application_get_default ());
    g_free (data);
}

static gboolean
handle_open_file_cb (DoryFileChooser *object,
                     GDBusMethodInvocation *invocation,
                     const gchar *title,
                     const gchar *const *filters,
                     gboolean multiselect,
                     gboolean directory,
                     const gchar *initial_folder,
                     gpointer user_data)
{
    GtkWidget *dialog;

    g_message ("handle_open_file_cb: title='%s', multiselect=%d, directory=%d, initial_folder='%s'",
               title ? title : "", multiselect, directory, initial_folder ? initial_folder : "");

    dialog = dory_file_chooser_dialog_new (title,
                                          directory ? GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER : GTK_FILE_CHOOSER_ACTION_OPEN,
                                          multiselect,
                                          initial_folder,
                                          NULL);

    ResponseData *data = g_new0 (ResponseData, 1);
    data->invocation = invocation;
    data->dialog = dialog;
    data->skeleton = object;

    g_signal_connect (dialog, "response", G_CALLBACK (on_open_dialog_response), data);
    gtk_widget_show_all (dialog);
    g_application_hold (g_application_get_default ());

    return TRUE;
}

static void
on_save_dialog_response (GtkDialog *dialog, gint response_id, gpointer user_data)
{
    ResponseData *data = user_data;
    gchar *result = NULL;

    if (response_id == GTK_RESPONSE_ACCEPT || response_id == GTK_RESPONSE_OK) {
        result = dory_file_chooser_dialog_get_selected_uri (dialog);
        if (result && *result) {
            g_autoptr(GFile) file = g_file_new_for_uri (result);
            if (g_file_query_exists (file, NULL)) {
                g_autofree gchar *basename = g_file_get_basename (file);
                GtkWidget *msg_dialog = gtk_message_dialog_new (GTK_WINDOW (dialog),
                                                                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                GTK_MESSAGE_QUESTION,
                                                                GTK_BUTTONS_YES_NO,
                                                                _("A file named \"%s\" already exists. Do you want to replace it?"),
                                                                basename);
                gtk_window_set_title (GTK_WINDOW (msg_dialog), _("Replace File"));
                gint res = gtk_dialog_run (GTK_DIALOG (msg_dialog));
                gtk_widget_destroy (msg_dialog);

                if (res != GTK_RESPONSE_YES) {
                    g_free (result);
                    return;
                }
            }
        }
    }

    dory_file_chooser_complete_save_file (data->skeleton, data->invocation, result ? result : "");

    g_free (result);
    gtk_widget_destroy (GTK_WIDGET (dialog));
    g_application_release (g_application_get_default ());
    g_free (data);
}

typedef struct {
    GDBusMethodInvocation *invocation;
    GtkWidget *dialog;
    DoryFileChooser *skeleton;
    gchar **suggested_names;
} SaveFilesResponseData;

static void
on_save_files_dialog_response (GtkDialog *dialog, gint response_id, gpointer user_data)
{
    SaveFilesResponseData *data = user_data;
    GPtrArray *results = g_ptr_array_new_with_free_func (g_free);

    if (response_id == GTK_RESPONSE_ACCEPT || response_id == GTK_RESPONSE_OK) {
        gchar *folder_uri = dory_file_chooser_dialog_get_selected_uri (dialog);
        if (folder_uri && *folder_uri) {
            g_autoptr(GFile) folder = g_file_new_for_uri (folder_uri);

            for (int i = 0; data->suggested_names[i] != NULL; i++) {
                g_autoptr(GFile) child = g_file_get_child (folder, data->suggested_names[i]);
                gchar *child_uri = g_file_get_uri (child);

                if (g_file_query_exists (child, NULL)) {
                    g_autofree gchar *basename = g_file_get_basename (child);
                    GtkWidget *msg_dialog = gtk_message_dialog_new (GTK_WINDOW (dialog),
                                                                    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                    GTK_MESSAGE_QUESTION,
                                                                    GTK_BUTTONS_YES_NO,
                                                                    _("A file named \"%s\" already exists. Do you want to replace it?"),
                                                                    basename);
                    gtk_window_set_title (GTK_WINDOW (msg_dialog), _("Replace File"));
                    gint res = gtk_dialog_run (GTK_DIALOG (msg_dialog));
                    gtk_widget_destroy (msg_dialog);

                    if (res != GTK_RESPONSE_YES) {
                        g_free (child_uri);
                        g_free (folder_uri);
                        g_ptr_array_unref (results);
                        return;
                    }
                }

                g_ptr_array_add (results, child_uri);
            }
            g_free (folder_uri);
        }
    }

    g_ptr_array_add (results, NULL);

    dory_file_chooser_complete_save_files (data->skeleton, data->invocation,
                                          (const gchar *const *)results->pdata);

    g_ptr_array_unref (results);
    g_strfreev (data->suggested_names);
    gtk_widget_destroy (GTK_WIDGET (dialog));
    g_application_release (g_application_get_default ());
    g_free (data);
}

static gboolean
handle_save_file_cb (DoryFileChooser *object,
                     GDBusMethodInvocation *invocation,
                     const gchar *title,
                     const gchar *initial_folder,
                     const gchar *suggested_name,
                     gpointer user_data)
{
    GtkWidget *dialog;

    dialog = dory_file_chooser_dialog_new (title,
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          FALSE,
                                          initial_folder,
                                          suggested_name);

    ResponseData *data = g_new0 (ResponseData, 1);
    data->invocation = invocation;
    data->dialog = dialog;
    data->skeleton = object;

    g_signal_connect (dialog, "response", G_CALLBACK (on_save_dialog_response), data);
    gtk_widget_show_all (dialog);
    g_application_hold (g_application_get_default ());

    return TRUE;
}

static gboolean
handle_save_files_cb (DoryFileChooser *object,
                      GDBusMethodInvocation *invocation,
                      const gchar *title,
                      const gchar *initial_folder,
                      const gchar *const *suggested_names,
                      gpointer user_data)
{
    GtkWidget *dialog;
    const gchar *first_name = NULL;

    if (suggested_names && suggested_names[0]) {
        first_name = suggested_names[0];
    }

    dialog = dory_file_chooser_dialog_new (title,
                                           GTK_FILE_CHOOSER_ACTION_SAVE,
                                           FALSE,
                                           initial_folder,
                                           first_name);

    SaveFilesResponseData *data = g_new0 (SaveFilesResponseData, 1);
    data->invocation = invocation;
    data->dialog = dialog;
    data->skeleton = object;
    data->suggested_names = g_strdupv ((gchar **)suggested_names);

    g_signal_connect (dialog, "response", G_CALLBACK (on_save_files_dialog_response), data);
    gtk_widget_show_all (dialog);
    g_application_hold (g_application_get_default ());

    return TRUE;
}

static void
bus_acquired_cb (GDBusConnection *conn,
                 const gchar     *name,
                 gpointer         user_data)
{
    DoryFileChooserDBus *self = user_data;

    self->skeleton = dory_file_chooser_skeleton_new ();

    g_signal_connect (self->skeleton, "handle-open-file",
                      G_CALLBACK (handle_open_file_cb), self);
    g_signal_connect (self->skeleton, "handle-save-file",
                      G_CALLBACK (handle_save_file_cb), self);
    g_signal_connect (self->skeleton, "handle-save-files",
                      G_CALLBACK (handle_save_files_cb), self);

    g_dbus_interface_skeleton_export (G_DBUS_INTERFACE_SKELETON (self->skeleton), conn, "/org/Dory/FileChooser", NULL);
}

static void
name_acquired_cb (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
}

static void
name_lost_cb (GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{
}

static void
dory_file_chooser_dbus_dispose (GObject *object)
{
    DoryFileChooserDBus *self = (DoryFileChooserDBus *) object;

    if (self->owner_id != 0) {
        g_bus_unown_name (self->owner_id);
        self->owner_id = 0;
    }

    if (self->skeleton != NULL) {
        g_dbus_interface_skeleton_unexport (G_DBUS_INTERFACE_SKELETON (self->skeleton));
        g_object_unref (self->skeleton);
        self->skeleton = NULL;
    }

    G_OBJECT_CLASS (dory_file_chooser_dbus_parent_class)->dispose (object);
}

static void
dory_file_chooser_dbus_class_init (DoryFileChooserDBusClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    object_class->dispose = dory_file_chooser_dbus_dispose;
}

static void
dory_file_chooser_dbus_init (DoryFileChooserDBus *self)
{
    self->owner_id = g_bus_own_name (G_BUS_TYPE_SESSION,
                                    "org.Dory.FileChooser",
                                    G_BUS_NAME_OWNER_FLAGS_NONE,
                                    bus_acquired_cb,
                                    name_acquired_cb,
                                    name_lost_cb,
                                    self,
                                    NULL);
}

DoryFileChooserDBus *
dory_file_chooser_dbus_new (void)
{
    return g_object_new (dory_file_chooser_dbus_get_type (), NULL);
}
