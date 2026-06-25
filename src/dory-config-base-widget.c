/* dory-config-base-widget.c */

/*  A base widget class for extension/action/script config widgets.
 *  This is usually part of a DoryPluginManagerWidget
 */

#include <config.h>
#include "dory-config-base-widget.h"
#include <glib.h>

G_DEFINE_TYPE (DoryConfigBaseWidget, dory_config_base_widget, GTK_TYPE_BIN);

static void
dory_config_base_widget_class_init (DoryConfigBaseWidgetClass *klass)
{
}

static void
dory_config_base_widget_init (DoryConfigBaseWidget *self)
{
    GtkStyleContext *context;
    GtkWidget *box;
    GtkWidget *w;
    GtkWidget *tb;
    GtkWidget *label;
    GtkWidget *toolbar_item;

    GtkWidget *frame = gtk_frame_new (NULL);
    gtk_container_add (GTK_CONTAINER (self), frame);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);

    context = gtk_widget_get_style_context (frame);
    gtk_style_context_add_class (context, "view");

    box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

    gtk_container_add (GTK_CONTAINER (frame), box);

    tb = gtk_toolbar_new ();
    context = gtk_widget_get_style_context (tb);
    gtk_style_context_add_class (context, "primary-toolbar");
    gtk_box_pack_start (GTK_BOX (box), tb, FALSE, FALSE, 0);

    toolbar_item = GTK_WIDGET (gtk_tool_item_new ());
    gtk_container_add (GTK_CONTAINER (tb), toolbar_item);

    w = gtk_label_new (NULL);
    gtk_container_add (GTK_CONTAINER (toolbar_item), w);
    self->label = w;

    w = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (w), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start (GTK_BOX (box), w, TRUE, TRUE, 0);

    self->listbox = gtk_list_box_new ();
    gtk_list_box_set_selection_mode (GTK_LIST_BOX (self->listbox), GTK_SELECTION_NONE);
    gtk_container_add (GTK_CONTAINER (w), self->listbox);

    tb = gtk_toolbar_new ();
    context = gtk_widget_get_style_context (tb);
    gtk_style_context_add_class (context, GTK_STYLE_CLASS_TOOLBAR);
    gtk_box_pack_start (GTK_BOX (box), tb, FALSE, FALSE, 0);

    toolbar_item = GTK_WIDGET (gtk_tool_item_new ());
    gtk_tool_item_set_expand (GTK_TOOL_ITEM (toolbar_item), TRUE);
    gtk_container_add (GTK_CONTAINER (tb), toolbar_item);

    w = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add (GTK_CONTAINER (toolbar_item), w);
    context = gtk_widget_get_style_context(w);
    gtk_style_context_add_class (context, "linked");
    self->lbuttonbox = w;

    label = gtk_label_new (_("Disable all"));
    gtk_label_set_ellipsize (GTK_LABEL (label), PANGO_ELLIPSIZE_END);

    self->disable_button = gtk_button_new ();
    gtk_container_add (GTK_CONTAINER (w), self->disable_button);
    gtk_container_add (GTK_CONTAINER (self->disable_button), label);
    gtk_widget_show_all (self->disable_button);
    gtk_widget_set_no_show_all (self->disable_button, TRUE);

    label = gtk_label_new (_("Enable all"));
    gtk_label_set_ellipsize (GTK_LABEL (label), PANGO_ELLIPSIZE_END);

    self->enable_button = gtk_button_new ();
    gtk_container_add (GTK_CONTAINER (w), self->enable_button);
    gtk_container_add (GTK_CONTAINER (self->enable_button), label);
    gtk_widget_show_all (self->enable_button);
    gtk_widget_set_no_show_all (self->enable_button, TRUE);

    toolbar_item = GTK_WIDGET (gtk_tool_item_new ());
    gtk_tool_item_set_expand (GTK_TOOL_ITEM (toolbar_item), FALSE);
    gtk_container_add (GTK_CONTAINER (tb), toolbar_item);

    w = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add (GTK_CONTAINER (toolbar_item), w);
    context = gtk_widget_get_style_context(w);
    gtk_style_context_add_class (context, "linked");
    self->rbuttonbox = w;

    gtk_widget_show_all (GTK_WIDGET (self));
}

/**
 * dory_config_base_widget_get_label:
 * @widget: a #DoryConfigBaseWidget
 *
 * Returns: (transfer none): the label #GtkWidget
 */

GtkWidget *
dory_config_base_widget_get_label (DoryConfigBaseWidget *widget)
{
    return widget->label;
}

/**
 * dory_config_base_widget_get_listbox:
 * @widget: a #DoryConfigBaseWidget
 *
 * Returns: (transfer none): the listbox #GtkWidget
 */

GtkWidget *
dory_config_base_widget_get_listbox (DoryConfigBaseWidget *widget)
{
    return widget->listbox;
}

/**
 * dory_config_base_widget_get_enable_button:
 * @widget: a #DoryConfigBaseWidget
 *
 * Returns: (transfer none): the enable button #GtkWidget
 */

GtkWidget *
dory_config_base_widget_get_enable_button (DoryConfigBaseWidget *widget)
{
    return widget->enable_button;
}

/**
 * dory_config_base_widget_get_disable_button:
 * @widget: a #DoryConfigBaseWidget
 *
 * Returns: (transfer none): the disable button #GtkWidget
 */

GtkWidget *
dory_config_base_widget_get_disable_button (DoryConfigBaseWidget *widget)
{
    return widget->disable_button;
}

/**
 * dory_config_base_widget_set_default_buttons_sensitive:
 * @widget: a #DoryConfigBaseWidget
 * @sensitive: TRUE or FALSE
 *
 * Set the enable/disable buttons sensitive or not
 */

void
dory_config_base_widget_set_default_buttons_sensitive (DoryConfigBaseWidget *widget, gboolean sensitive)
{
    gtk_widget_set_sensitive (widget->enable_button, sensitive);
    gtk_widget_set_sensitive (widget->disable_button, sensitive);
}

/**
 * dory_config_base_widget_clear_list:
 * @widget: a #DoryConfigBaseWidget
 * 
 * Clear the listbox and destroy all children
 */

void
dory_config_base_widget_clear_list (DoryConfigBaseWidget *widget)
{
    gtk_container_foreach (GTK_CONTAINER (widget->listbox), (GtkCallback) gtk_widget_destroy, NULL);
}


