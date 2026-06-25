/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Suite 500, Boston, MA 02110-1335, USA.
 *
 */

#include "config.h"

#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "dory-interesting-folder-bar.h"
#include "dory-application.h"

#include "dory-view.h"
#include <libdory-private/dory-file-operations.h>
#include <libdory-private/dory-file-utilities.h>
#include <libdory-private/dory-file.h>
#include <libdory-private/dory-trash-monitor.h>
#include <libdory-private/dory-action-manager.h>

#define DORY_INTERESTING_FOLDER_BAR_GET_PRIVATE(o)\
	(G_TYPE_INSTANCE_GET_PRIVATE ((o), DORY_TYPE_INTERESTING_FOLDER_BAR, DoryInterestingFolderBarPrivate))

enum {
	PROP_VIEW = 1,
    PROP_TYPE,
	NUM_PROPERTIES
};

enum {
    INTERESTING_FOLDER_BAR_ACTION_OPEN_DOC = 1,
    INTERESTING_FOLDER_BAR_SCRIPT_OPEN_DOC,
    INTERESTING_FOLDER_BAR_TEMPLATE_OPEN_DOC
};

struct DoryInterestingFolderBarPrivate
{
	DoryView *view;
    InterestingFolderType type;
	gulong selection_handler_id;
};

G_DEFINE_TYPE (DoryInterestingFolderBar, dory_interesting_folder_bar, GTK_TYPE_INFO_BAR);

static void
interesting_folder_bar_response_cb (GtkInfoBar *infobar,
                                          gint  response_id,
                                      gpointer  user_data)
{
    DoryInterestingFolderBar *bar;
    GFile *f = NULL;

    bar = DORY_INTERESTING_FOLDER_BAR (infobar);

    switch (response_id) {
        case INTERESTING_FOLDER_BAR_ACTION_OPEN_DOC:
            f = g_file_new_for_path (DORY_DATADIR "/action-info.md");
            if (g_file_query_exists (f, NULL))
                dory_view_activate_file (bar->priv->view, dory_file_get (f), DORY_WINDOW_OPEN_FLAG_NEW_WINDOW);
            g_object_unref (f);
            break;
        case INTERESTING_FOLDER_BAR_SCRIPT_OPEN_DOC:
            f = g_file_new_for_path (DORY_DATADIR "/script-info.md");
            if (g_file_query_exists (f, NULL))
                dory_view_activate_file (bar->priv->view, dory_file_get (f), DORY_WINDOW_OPEN_FLAG_NEW_WINDOW);
            g_object_unref (f);
            break;
        default:
            break;
    }
}

static void
dory_interesting_folder_bar_set_property (GObject      *object,
				 guint         prop_id,
				 const GValue *value,
				 GParamSpec   *pspec)
{
	DoryInterestingFolderBar *bar;

	bar = DORY_INTERESTING_FOLDER_BAR (object);

	switch (prop_id) {
	case PROP_VIEW:
		bar->priv->view = g_value_get_object (value);
		break;
    case PROP_TYPE:
        bar->priv->type = g_value_get_int (value);
        break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
dory_interesting_folder_bar_constructed (GObject *obj)
{
    G_OBJECT_CLASS (dory_interesting_folder_bar_parent_class)->constructed (obj);

    DoryInterestingFolderBar *bar = DORY_INTERESTING_FOLDER_BAR (obj);

    GtkWidget *content_area, *action_area, *w;
    GtkWidget *label;

    content_area = gtk_info_bar_get_content_area (GTK_INFO_BAR (bar));
    action_area = gtk_info_bar_get_action_area (GTK_INFO_BAR (bar));

    gtk_orientable_set_orientation (GTK_ORIENTABLE (action_area),
                    GTK_ORIENTATION_HORIZONTAL);

    switch (bar->priv->type) {
        case TYPE_ACTIONS_FOLDER:
            label = gtk_label_new (_("Actions: Action files can be added to this folder and will appear in the menu."));
            w = gtk_info_bar_add_button (GTK_INFO_BAR (bar),
                                         _("More info"),
                                         INTERESTING_FOLDER_BAR_ACTION_OPEN_DOC);
            gtk_widget_set_tooltip_text (w, _("View a sample action file with documentation"));
            break;
        case TYPE_SCRIPTS_FOLDER:
            label = gtk_label_new (_("Scripts: All executable files in this folder will appear in the "
                                     "Scripts menu."));
            w = gtk_info_bar_add_button (GTK_INFO_BAR (bar),
                                         _("More info"),
                                         INTERESTING_FOLDER_BAR_SCRIPT_OPEN_DOC);
            gtk_widget_set_tooltip_text (w, _("View additional information about creating scripts"));
            break;
        case TYPE_TEMPLATES_FOLDER:
            label = gtk_label_new (_("The files in this folder are used as templates in the 'Create New Document' section of your context menu."));
            break;
        case TYPE_NONE_FOLDER:
        default:
            label = gtk_label_new ("undefined");
            break;
    }

    gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);
    gtk_style_context_add_class (gtk_widget_get_style_context (label),
                     "dory-cluebar-label");
    gtk_widget_show (label);
    gtk_container_add (GTK_CONTAINER (content_area), label);

    g_signal_connect (bar, "response",
              G_CALLBACK (interesting_folder_bar_response_cb), bar);
}

static void
dory_interesting_folder_bar_class_init (DoryInterestingFolderBarClass *klass)
{
	GObjectClass *object_class;

	object_class = G_OBJECT_CLASS (klass);

	object_class->set_property = dory_interesting_folder_bar_set_property;
    object_class->constructed = dory_interesting_folder_bar_constructed;

	g_object_class_install_property (object_class,
					 PROP_VIEW,
					 g_param_spec_object ("view",
							      "view",
							      "the DoryView",
							      DORY_TYPE_VIEW,
							      G_PARAM_WRITABLE |
							      G_PARAM_CONSTRUCT_ONLY |
							      G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (object_class,
                     PROP_TYPE,
                     g_param_spec_int ("type",
                                  "type",
                                  "the InterestingFolderType",
                                  TYPE_NONE_FOLDER,
                                  TYPE_TEMPLATES_FOLDER,
                                  TYPE_NONE_FOLDER,
                                  G_PARAM_WRITABLE |
                                  G_PARAM_CONSTRUCT_ONLY |
                                  G_PARAM_STATIC_STRINGS));

	g_type_class_add_private (klass, sizeof (DoryInterestingFolderBarPrivate));
}

static void
dory_interesting_folder_bar_init (DoryInterestingFolderBar *bar)
{
	bar->priv = DORY_INTERESTING_FOLDER_BAR_GET_PRIVATE (bar);
    bar->priv->type = TYPE_NONE_FOLDER;
}

GtkWidget *
dory_interesting_folder_bar_new (DoryView *view, InterestingFolderType type)
{
return g_object_new (DORY_TYPE_INTERESTING_FOLDER_BAR,
                     "view", view,
                     "type", type,
                     NULL);
}

GtkWidget *
dory_interesting_folder_bar_new_for_location (DoryView *view, GFile *location)
{
    InterestingFolderType type = TYPE_NONE_FOLDER;
    gchar *path = NULL;
    GFile *tmp_loc = NULL;

    path = dory_action_manager_get_user_directory_path ();
    tmp_loc = g_file_new_for_path (path);
    g_free (path);

    if (g_file_equal (location, tmp_loc)) {
        type = TYPE_ACTIONS_FOLDER;
    }
    g_object_unref (tmp_loc);

    if (type == TYPE_NONE_FOLDER) {
        path = dory_get_scripts_directory_path ();
        tmp_loc = g_file_new_for_path (path);
        g_free (path);

        if (g_file_equal (location, tmp_loc)) {
            type = TYPE_SCRIPTS_FOLDER;
        }
        g_object_unref (tmp_loc);
    }

    if (type == TYPE_NONE_FOLDER) {
        if (dory_should_use_templates_directory ()) {
            path = dory_get_templates_directory ();
            tmp_loc = g_file_new_for_path (path);
            g_free (path);

            if (g_file_equal (location, tmp_loc)) {
                type = TYPE_TEMPLATES_FOLDER;
            }
            g_object_unref (tmp_loc);
        }
    }

    return type == TYPE_NONE_FOLDER ? NULL : dory_interesting_folder_bar_new (view, type);
}
