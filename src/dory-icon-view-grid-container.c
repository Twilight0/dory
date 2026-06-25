/* -*- Mode: C; indent-tabs-mode: f; c-basic-offset: 4; tab-width: 4 -*- */

/* fm-icon-container.h - the container widget for file manager icons

   Copyright (C) 2002 Sun Microsystems, Inc.

   The Gnome Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gnome Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
   Boston, MA 02110-1335, USA.

   Author: Michael Meeks <michael@ximian.com>
*/
#include <config.h>

#include "dory-icon-view-grid-container.h"

#include <string.h>
#include <errno.h>
#include <math.h>

#include <glib/gi18n.h>
#include <gio/gio.h>
#include <eel/eel-glib-extensions.h>
#include "dory-icon-private.h"

#define DEBUG_FLAG DORY_DEBUG_DESKTOP
#include <libdory-private/dory-debug.h>

#include <libdory-private/dory-desktop-utils.h>
#include <libdory-private/dory-global-preferences.h>
#include <libdory-private/dory-file-attributes.h>
#include <libdory-private/dory-metadata.h>
#include <libdory-private/dory-thumbnails.h>
#include <libdory-private/dory-desktop-icon-file.h>

static void update_layout_constants (DoryIconContainer *container);

G_DEFINE_TYPE (DoryIconViewGridContainer, dory_icon_view_grid_container, DORY_TYPE_ICON_CONTAINER);

#define GRID_VIEW_MAX_ADDITIONAL_ATTRIBUTES 1
static GQuark attribute_none_q;

static DoryIconView *
get_icon_view (DoryIconContainer *container)
{
	/* Type unsafe comparison for performance */
	return ((DoryIconViewGridContainer *)container)->view;
}


static DoryIconInfo *
dory_icon_view_grid_container_get_icon_images (DoryIconContainer *container,
					      DoryIconData      *data,
					      int                    size,
					      gboolean               for_drag_accept,
					      gboolean              *has_window_open,
                          gboolean               visible)
{
	DoryIconView *icon_view;
	DoryFile *file;
	DoryFileIconFlags flags;
	DoryIconInfo *icon_info;
	GdkPixbuf *pixbuf;
	GIcon *emblemed_icon;
	GEmblem *emblem;
	GList *emblem_icons, *l;
    gint scale;

	file = (DoryFile *) data;

	g_assert (DORY_IS_FILE (file));
	icon_view = get_icon_view (container);
	g_return_val_if_fail (icon_view != NULL, NULL);

	*has_window_open = dory_file_has_open_window (file);

	flags = DORY_FILE_ICON_FLAGS_USE_MOUNT_ICON_AS_EMBLEM |
			DORY_FILE_ICON_FLAGS_USE_THUMBNAILS |
            DORY_FILE_ICON_FLAGS_FORCE_THUMBNAIL_SIZE |
            DORY_FILE_ICON_FLAGS_PIN_HEIGHT_FOR_DESKTOP;

	if (for_drag_accept) {
		flags |= DORY_FILE_ICON_FLAGS_FOR_DRAG_ACCEPT;
	}

	emblem_icons = dory_file_get_emblem_icons (file,
                                               dory_view_get_directory_as_file (DORY_VIEW (icon_view)));

    scale = gtk_widget_get_scale_factor (GTK_WIDGET (icon_view));
	icon_info = dory_file_get_icon (file, size, GET_VIEW_CONSTANT (container, max_text_width_standard), scale, flags);

	/* apply emblems */
	if (emblem_icons != NULL) {
        gint w, h, s;
        gboolean bad_ratio;

        l = emblem_icons;

		pixbuf = dory_icon_info_get_pixbuf (icon_info);

        w = gdk_pixbuf_get_width (pixbuf);
        h = gdk_pixbuf_get_height (pixbuf);

        s = MAX (w, h);
        if (s < size)
            size = s;

        bad_ratio = (int)dory_icon_get_emblem_size_for_icon_size (size) * scale > (int)(w * 0.75) ||
                    (int)dory_icon_get_emblem_size_for_icon_size (size) * scale > (int)(h * 0.75);

        if (bad_ratio)
            goto skip_emblem; /* Would prefer to not use goto, but
                               * I don't want to do these checks on
                               * non-emblemed icons (the majority)
                               * as it would be too costly */

        emblem = g_emblem_new (l->data);

		emblemed_icon = g_emblemed_icon_new (G_ICON (pixbuf), emblem);
		g_object_unref (emblem);

		for (l = l->next; l != NULL; l = l->next) {
			emblem = g_emblem_new (l->data);
			g_emblemed_icon_add_emblem (G_EMBLEMED_ICON (emblemed_icon),
						    emblem);
			g_object_unref (emblem);
		}

		dory_icon_info_clear (&icon_info);
		icon_info = dory_icon_info_lookup (emblemed_icon, size, scale);
        g_object_unref (emblemed_icon);

skip_emblem:
		g_object_unref (pixbuf);

	}

	if (emblem_icons != NULL) {
		g_list_free_full (emblem_icons, g_object_unref);
	}

	return icon_info;
}

static char *
dory_icon_view_grid_container_get_icon_description (DoryIconContainer *container,
						   DoryIconData      *data)
{
	DoryFile *file;
	char *mime_type;
	const char *description;

	file = DORY_FILE (data);
	g_assert (DORY_IS_FILE (file));

	if (DORY_IS_DESKTOP_ICON_FILE (file)) {
		return NULL;
	}

	mime_type = dory_file_get_mime_type (file);
	description = g_content_type_get_description (mime_type);
	g_free (mime_type);
	return g_strdup (description);
}

static void
update_auto_strv_as_quarks (GSettings   *settings,
			    const gchar *key,
			    gpointer     user_data)
{
	GQuark **storage = user_data;
	int i = 0;
	char **value;

	value = g_settings_get_strv (settings, key);

	g_free (*storage);
	*storage = g_new (GQuark, g_strv_length (value) + 1);

	for (i = 0; value[i] != NULL; ++i) {
		(*storage)[i] = g_quark_from_string (value[i]);
	}
	(*storage)[i] = 0;

	g_strfreev (value);
}

static int
quarkv_length (GQuark *attributes)
{
	int i;
	i = 0;
	while (attributes[i] != 0) {
		i++;
	}
	return i;
}

/**
 * dory_icon_view_get_icon_text_attribute_names:
 *
 * Get a list representing which text attributes should be displayed
 * beneath an icon. The result is dependent on zoom level and possibly
 * user configuration. Don't free the result.
 * @view: DoryIconView to query.
 *
 **/
static GQuark *
dory_icon_view_grid_container_get_icon_text_attribute_names (DoryIconContainer *container,
							    int *len)
{
    int piece_count;

    /* For now, limit extra attributes to one line - TODO: make this desktop
     * more flexible at displaying various file info without disturbing grid layout
     */

    piece_count = GRID_VIEW_MAX_ADDITIONAL_ATTRIBUTES;

	*len = MIN (piece_count, quarkv_length (DORY_ICON_VIEW_GRID_CONTAINER (container)->attributes));

	return DORY_ICON_VIEW_GRID_CONTAINER (container)->attributes;
}

/* This callback returns the text, both the editable part, and the
 * part below that is not editable.
 */
static void
dory_icon_view_grid_container_get_icon_text (DoryIconContainer *container,
					    DoryIconData      *data,
					    char                 **editable_text,
					    char                 **additional_text,
                        gboolean              *pinned,
                        gboolean              *fav_unavailable,
					    gboolean               include_invisible)
{
	GQuark *attributes;
	char *text_array[4];
	int i, j, num_attributes;
	DoryIconView *icon_view;
	DoryFile *file;
	gboolean use_additional;

	file = DORY_FILE (data);

	g_assert (DORY_IS_FILE (file));
	g_assert (editable_text != NULL);
	icon_view = get_icon_view (container);
	g_return_if_fail (icon_view != NULL);

	use_additional = (additional_text != NULL);

    /* Strip the suffix for dory object xml files. */
    *editable_text = dory_file_get_display_name (file);

    if (!use_additional) {
        return;
    }

    if (DORY_IS_DESKTOP_ICON_FILE (file) ||
        dory_file_is_dory_link (file)) {
        /* Don't show the normal extra information for desktop icons,
         * or desktop files, it doesn't make sense. */
        *additional_text = NULL;
        return;
    }

	/* Find out what attributes go below each icon. */
	attributes = dory_icon_view_grid_container_get_icon_text_attribute_names (container,
                                                                              &num_attributes);

	/* Get the attributes. */
	j = 0;
	for (i = 0; i < num_attributes; ++i) {
		if (attributes[i] == attribute_none_q) {
			continue;
		}

        text_array[j++] = dory_file_get_string_attribute_with_default_q (file, attributes[i]);
    }

    text_array[j] = NULL;

	/* Return them. */
    if (j == 0) {
        *additional_text = NULL;
    } else if (j == 1) {
		/* Only one item, avoid the strdup + free */
		*additional_text = text_array[0];
	} else {
		*additional_text = g_strjoinv ("\n", text_array);

		for (i = 0; i < j; i++) {
			g_free (text_array[i]);
		}
	}
}

/* Sort as follows:
 *   0) computer link
 *   1) home link
 *   2) network link
 *   3) mount links
 *   4) trash link
 *   5) other
 */
typedef enum {
	SORT_COMPUTER_LINK,
	SORT_HOME_LINK,
	SORT_NETWORK_LINK,
	SORT_MOUNT_LINK,
	SORT_TRASH_LINK,
	SORT_OTHER
} SortCategory;

static SortCategory
get_sort_category (DoryFile *file)
{
	DoryDesktopLink *link;
	SortCategory category;

	category = SORT_OTHER;

	if (DORY_IS_DESKTOP_ICON_FILE (file)) {
		link = dory_desktop_icon_file_get_link (DORY_DESKTOP_ICON_FILE (file));
		if (link != NULL) {
			switch (dory_desktop_link_get_link_type (link)) {
			case DORY_DESKTOP_LINK_COMPUTER:
				category = SORT_COMPUTER_LINK;
				break;
			case DORY_DESKTOP_LINK_HOME:
				category = SORT_HOME_LINK;
				break;
			case DORY_DESKTOP_LINK_MOUNT:
				category = SORT_MOUNT_LINK;
				break;
			case DORY_DESKTOP_LINK_TRASH:
				category = SORT_TRASH_LINK;
				break;
			case DORY_DESKTOP_LINK_NETWORK:
				category = SORT_NETWORK_LINK;
				break;
			default:
				category = SORT_OTHER;
				break;
			}
			g_object_unref (link);
		}
	}

	return category;
}

static int
dory_icon_view_grid_container_compare_icons (DoryIconContainer *container,
                                             DoryIconData      *data_a,
                                             DoryIconData      *data_b)
{
    DoryFile *file_a;
    DoryFile *file_b;
    DoryIconView *icon_view;
    SortCategory category_a, category_b;

    file_a = (DoryFile *) data_a;
    file_b = (DoryFile *) data_b;

    icon_view = DORY_ICON_VIEW (DORY_ICON_VIEW_GRID_CONTAINER (container)->view);
    g_return_val_if_fail (icon_view != NULL, 0);

    category_a = get_sort_category (file_a);
    category_b = get_sort_category (file_b);

    if (category_a == category_b) {
        return dory_icon_view_compare_files (icon_view,
                                             file_a,
                                             file_b);
    }

    if (category_a < category_b) {
        return -1;
    } else {
        return +1;
    }
}

static void
dory_icon_view_grid_container_freeze_updates (DoryIconContainer *container)
{
	DoryIconView *icon_view;
	icon_view = get_icon_view (container);
	g_return_if_fail (icon_view != NULL);
	dory_view_freeze_updates (DORY_VIEW (icon_view));
}

static void
dory_icon_view_grid_container_unfreeze_updates (DoryIconContainer *container)
{
	DoryIconView *icon_view;
	icon_view = get_icon_view (container);
	g_return_if_fail (icon_view != NULL);
	dory_view_unfreeze_updates (DORY_VIEW (icon_view));
}

inline static void
dory_icon_view_grid_container_icon_get_bounding_box (DoryIcon *icon,
               int *x1_return, int *y1_return,
               int *x2_return, int *y2_return,
               DoryIconCanvasItemBoundsUsage usage)
{
    double x1, y1, x2, y2;

    if (usage == BOUNDS_USAGE_FOR_DISPLAY) {
        eel_canvas_item_get_bounds (EEL_CANVAS_ITEM (icon->item),
                        &x1, &y1, &x2, &y2);
    } else if (usage == BOUNDS_USAGE_FOR_LAYOUT) {
        dory_icon_canvas_item_get_bounds_for_layout (icon->item,
                                 &x1, &y1, &x2, &y2);
    } else if (usage == BOUNDS_USAGE_FOR_ENTIRE_ITEM) {
        dory_icon_canvas_item_get_bounds_for_entire_item (icon->item,
                                      &x1, &y1, &x2, &y2);
    } else {
        g_assert_not_reached ();
    }

    if (x1_return != NULL) {
        *x1_return = x1;
    }

    if (y1_return != NULL) {
        *y1_return = y1;
    }

    if (x2_return != NULL) {
        *x2_return = x2;
    }

    if (y2_return != NULL) {
        *y2_return = y2;
    }
}

static gboolean
get_stored_icon_position (DoryIconContainer *container,
                          DoryIconData      *data,
                          DoryIconPosition  *position)
{
    DoryFile *file;
    GdkPoint point;

    g_assert (DORY_IS_ICON_CONTAINER (container));
    g_assert (DORY_IS_FILE (data));
    g_assert (position != NULL);

    file = DORY_FILE (data);

    if (dory_file_get_is_desktop_orphan (file)) {
        return FALSE;
    }

    dory_file_get_position (file, &point);
    position->x = point.x;
    position->y = point.y;
    position->scale = 1.0;

    return position->x > ICON_UNPOSITIONED_VALUE;
}

static void
store_new_icon_position (DoryIconContainer *container,
                         DoryIcon          *icon,
                         DoryIconPosition   position)
{
    gboolean dummy;
    time_t now;

    now = time (NULL);

    g_signal_emit_by_name (container, "icon_position_changed",
                           icon->data, &position);
    g_signal_emit_by_name (container, "store_layout_timestamp",
                           icon->data, &now, &dummy);
}

static void
snap_position (DoryIconContainer         *container,
               DoryCenteredPlacementGrid *grid,
               DoryIcon                  *icon,
               gint                       x_in,
               gint                       y_in,
               gint                      *x_out,
               gint                      *y_out)
{
    gint x_new, y_new;
    gboolean found_empty;
    GdkRectangle grid_rect;

    if (dory_icon_container_is_layout_rtl (container)) {
        x_in = dory_icon_container_get_mirror_x_position (container, icon, x_in);
    }

    dory_centered_placement_grid_get_current_position_rect (grid,
                                                            x_in,
                                                            y_in,
                                                            &grid_rect,
                                                            &found_empty);

    if (found_empty) {
        x_new = grid_rect.x;
        y_new = grid_rect.y;
    } else {
        while (!found_empty) {
            dory_centered_placement_grid_get_next_position_rect (grid, &grid_rect, &grid_rect, &found_empty);
        }

        x_new = grid_rect.x;
        y_new = grid_rect.y;
    }

    *x_out = x_new;
    *y_out = y_new;

    if (dory_icon_container_is_layout_rtl (container)) {
        *x_out = dory_icon_container_get_mirror_x_position (container, icon, *x_out);
    }
}

static void
lay_down_icons_desktop (DoryIconContainer *container, GList *icons)
{
    GList *p, *placed_icons, *unplaced_icons;
    int total, new_length, placed;
    DoryIcon *icon;
    int x, y;
    gint current_monitor;

    current_monitor = dory_desktop_utils_get_monitor_for_widget (GTK_WIDGET (container));

    /* Determine which icons have and have not been placed */
    placed_icons = NULL;
    unplaced_icons = NULL;

    total = g_list_length (container->details->icons);
    new_length = g_list_length (icons);

    placed = total - new_length;

    if (placed > 0) {
        DoryCenteredPlacementGrid *grid;
        /* Add only placed icons in list */
        for (p = container->details->icons; p != NULL; p = p->next) {
            icon = p->data;
            if (dory_icon_container_icon_is_positioned (icon) && !icon->has_lazy_position) {
                dory_icon_container_icon_set_position (container, icon, icon->saved_ltr_x, icon->y);
                placed_icons = g_list_prepend (placed_icons, icon);
            } else {
                icon->x = 0;
                icon->y = 0;
                unplaced_icons = g_list_prepend (unplaced_icons, icon);
            }
        }

        placed_icons = g_list_reverse (placed_icons);
        unplaced_icons = g_list_reverse (unplaced_icons);

        grid = dory_centered_placement_grid_new (container, container->details->horizontal);

        if (grid) {
            dory_centered_placement_grid_pre_populate (grid, placed_icons, FALSE);

            /* Place unplaced icons in the best locations */
            for (p = unplaced_icons; p != NULL; p = p->next) {
                icon = p->data;

                dory_centered_placement_grid_icon_position_to_nominal (grid,
                                                                       icon,
                                                                       icon->x,
                                                                       icon->y,
                                                                       &x, &y);

                snap_position (container, grid, icon, x, y, &x, &y);

                dory_centered_placement_grid_nominal_to_icon_position (grid,
                                                                       icon,
                                                                       x, y,
                                                                       &x, &y);

                dory_icon_container_icon_set_position (container, icon, x, y);
                icon->has_lazy_position = FALSE;
                icon->saved_ltr_x = x;
                dory_centered_placement_grid_mark_icon (grid, icon);
            }

            dory_centered_placement_grid_free (grid);
        }

        g_list_free (placed_icons);
        g_list_free (unplaced_icons);
    } else {
        DoryCenteredPlacementGrid *grid;
        GdkRectangle rect;

        /* There are no placed icons, or we have auto layout enabled */

        grid = dory_centered_placement_grid_new (container, container->details->horizontal);

        if (grid != NULL) {
            dory_centered_placement_grid_get_current_position_rect (grid, 0, 0, &rect, NULL);

            while (icons != NULL) {
                DoryIconPosition position;

                icon = icons->data;

                dory_centered_placement_grid_nominal_to_icon_position (grid, icon, rect.x, rect.y, &x, &y);

                dory_icon_container_icon_set_position (container,
                                                       icon,
                                                       x, y);

                icon->saved_ltr_x = icon->x;
                icon->has_lazy_position = FALSE;
                icons = icons->next;

                dory_centered_placement_grid_mark_icon (grid, icon);
                dory_centered_placement_grid_get_next_position_rect (grid, &rect, &rect, NULL);

                position.x = icon->x;
                position.y = icon->y;
                position.scale = 1.0;
                position.monitor = current_monitor;
                store_new_icon_position (container, icon, position);
            }

            dory_centered_placement_grid_free (grid);
        }
    }

    if (!container->details->stored_auto_layout) {
        dory_icon_container_freeze_icon_positions (container);
    }
}

static void
dory_icon_view_grid_container_lay_down_icons (DoryIconContainer *container, GList *icons, double start_y)
{

    lay_down_icons_desktop (container, icons);
}

static gint
order_icons_by_visual_position (gconstpointer a, gconstpointer b, gpointer user_data)
{
    DoryCenteredPlacementGrid *grid;
    DoryIcon *icon_a, *icon_b;
    GdkRectangle rect_a, rect_b;

    grid = (DoryCenteredPlacementGrid *) user_data;

    icon_a = (DoryIcon*)a;
    icon_b = (DoryIcon*)b;

    dory_centered_placement_grid_get_current_position_rect (grid,
                                                            (gint) icon_a->x,
                                                            (gint) icon_a->y,
                                                            &rect_a,
                                                            NULL);

    dory_centered_placement_grid_get_current_position_rect (grid,
                                                            (gint) icon_b->x,
                                                            (gint) icon_b->y,
                                                            &rect_b,
                                                            NULL);

    if (grid->horizontal) {
        return (rect_a.y == rect_b.y) ?
            rect_a.x - rect_b.x : rect_a.y - rect_b.y;
    } else {
        return (rect_a.x == rect_b.x) ?
            rect_a.y - rect_b.y : rect_a.x - rect_b.x;
    }
}

static void
update_visual_selection_state (DoryIconContainer *container)
{
    DoryCenteredPlacementGrid *grid;

    grid = dory_centered_placement_grid_new (container, container->details->horizontal);

    if (grid) {
        container->details->icons = g_list_sort_with_data (container->details->icons,
                                                           order_icons_by_visual_position,
                                                           grid);

        dory_centered_placement_grid_free (grid);
    }
}

/* x, y are the top-left coordinates of the icon. */
static void
dory_icon_view_grid_container_icon_set_position (DoryIconContainer *container,
                                            DoryIcon          *icon,
                                            double             x,
                                            double             y)
{
    double pixels_per_unit;
    int container_left, container_top, container_right, container_bottom;
    int x1, x2, y1, y2;
    int container_x, container_y, container_width, container_height;
    EelDRect icon_bounds;
    GtkAllocation alloc;
    int item_width, item_height;
    int height_above, width_left;
    int min_x, max_x, min_y, max_y;

    if (icon->x == x && icon->y == y) {
        return;
    }

    if (icon == dory_icon_container_get_icon_being_renamed (container)) {
        dory_icon_container_end_renaming_mode (container, TRUE);
    }

    gtk_widget_get_allocation (GTK_WIDGET (container), &alloc);
    container_x = alloc.x;
    container_y = alloc.y;
    container_width = alloc.width - container->details->left_margin - container->details->right_margin;
    container_height = alloc.height - container->details->top_margin - container->details->bottom_margin;
    pixels_per_unit = EEL_CANVAS (container)->pixels_per_unit;
    /* Clip the position of the icon within our desktop bounds */
    container_left = container_x / pixels_per_unit;
    container_top =  container_y / pixels_per_unit;
    container_right = container_left + container_width / pixels_per_unit;
    container_bottom = container_top + container_height / pixels_per_unit;

    dory_icon_container_icon_get_bounding_box (container, icon, &x1, &y1, &x2, &y2,
                                               BOUNDS_USAGE_FOR_LAYOUT);
    item_width = x2 - x1;
    item_height = y2 - y1;

    icon_bounds = dory_icon_canvas_item_get_icon_rectangle (icon->item);

    /* determine icon rectangle relative to item rectangle */
    height_above = icon_bounds.y0 - y1;
    width_left = icon_bounds.x0 - x1;

    min_x = container_left + width_left;
    max_x = container_right - item_width + width_left;
    x = CLAMP (x, min_x, max_x);

    min_y = container_top + height_above;
    max_y = container_bottom - item_height + height_above;
    y = CLAMP (y, min_y, max_y);

    if (icon->x == ICON_UNPOSITIONED_VALUE) {
        icon->x = 0;
    }
    if (icon->y == ICON_UNPOSITIONED_VALUE) {
        icon->y = 0;
    }

    eel_canvas_item_move (EEL_CANVAS_ITEM (icon->item),
                x - icon->x,
                y - icon->y);

    icon->x = x;
    icon->y = y;
}

static void
dory_icon_view_grid_container_move_icon (DoryIconContainer *container,
                   DoryIcon *icon,
                   int x, int y,
                   double scale,
                   gboolean raise,
                   gboolean snap,
                   gboolean update_position)
{
    DoryIconContainerDetails *details;
    gboolean emit_signal;
    DoryIconPosition position;
    gint current_monitor;

    current_monitor = dory_desktop_utils_get_monitor_for_widget (GTK_WIDGET (container));

    details = container->details;

    emit_signal = FALSE;

    if (icon == dory_icon_container_get_icon_being_renamed (container)) {
        dory_icon_container_end_renaming_mode (container, TRUE);
    }

    if (!details->auto_layout) {
        if (x != icon->x || y != icon->y) {
            dory_icon_container_icon_set_position (container, icon, x, y);
            emit_signal = update_position;
        }

        dory_icon_view_set_sort_reversed (get_icon_view (container), FALSE, TRUE);

        icon->saved_ltr_x = dory_icon_container_is_layout_rtl (container) ? dory_icon_container_get_mirror_x_position (container, icon, icon->x) : icon->x;
    }

    if (emit_signal) {
        position.x = icon->saved_ltr_x;
        position.y = icon->y;
        position.scale = scale;
        position.monitor = current_monitor;
        g_signal_emit_by_name (container, "icon_position_changed", icon->data, &position);
    }

    if (raise) {
        dory_icon_container_icon_raise (container, icon);
    }

    if (!container->details->auto_layout) {
        update_visual_selection_state (container);
    }
}

static void
dory_icon_view_grid_container_update_icon (DoryIconContainer *container,
                                           DoryIcon          *icon,
                                           gboolean           visible)
{
    DoryIconContainerDetails *details;
    guint icon_size;
    DoryIconInfo *icon_info;
    GdkPixbuf *pixbuf;
    char *editable_text, *additional_text;
    gboolean has_open_window;
    gint scale_factor;
    EelIRect old_size, new_size;
    gint old_width, new_width;

    if (icon == NULL) {
        return;
    }

    details = container->details;

    dory_icon_canvas_item_get_icon_canvas_rectangle (icon->item, &old_size);

    /* Get the appropriate images for the file. */
    icon_size = container->details->forced_icon_size;

    DEBUG ("Icon size for desktop grid, getting for size %d", icon_size);

    /* Get the icons. */
    icon_info = dory_icon_container_get_icon_images (container, icon->data, icon_size,
                                                     icon == details->drop_target,
                                                     &has_open_window, TRUE);

    scale_factor = gtk_widget_get_scale_factor (GTK_WIDGET (container));
    pixbuf = dory_icon_info_get_desktop_pixbuf_at_size (icon_info,
                                                        icon_size * scale_factor,
                                                        GET_VIEW_CONSTANT (container, max_text_width_standard));

    dory_icon_info_unref (icon_info);

    dory_icon_container_get_icon_text (container,
                           icon->data,
                           &editable_text,
                           &additional_text,
                           NULL,
                           NULL,
                           FALSE);

    /* If name of icon being renamed was changed from elsewhere, end renaming mode.
     * Alternatively, we could replace the characters in the editable text widget
     * with the new name, but that could cause timing problems if the user just
     * happened to be typing at that moment.
     */
    if (icon == dory_icon_container_get_icon_being_renamed (container) &&
        g_strcmp0 (editable_text,
               dory_icon_canvas_item_get_editable_text (icon->item)) != 0) {
        dory_icon_container_end_renaming_mode (container, FALSE);
    }

    eel_canvas_item_set (EEL_CANVAS_ITEM (icon->item),
                 "editable_text", editable_text,
                 "additional_text", additional_text,
                 "highlighted_for_drop", icon == details->drop_target,
                 NULL);

    dory_icon_canvas_item_set_image (icon->item, pixbuf);

    dory_icon_canvas_item_get_icon_canvas_rectangle (icon->item, &new_size);

    old_width = old_size.x1 - old_size.x0;
    new_width = new_size.x1 - new_size.x0;

    if (old_width != 0 && old_width != new_width) {
        gint diff;
        diff = (new_width - old_width) / 2;
        dory_icon_container_move_icon (container, icon, (int)icon->x - diff, (int)icon->y, 1.0, FALSE, TRUE, TRUE);
        dory_icon_canvas_item_invalidate_label_size (icon->item);
        gtk_widget_queue_draw (GTK_WIDGET(container));
    }

    /* Let the pixbufs go. */
    g_object_unref (pixbuf);

    g_free (editable_text);
    g_free (additional_text);
}

static void
dory_icon_view_grid_container_align_icons (DoryIconContainer *container)
{
    DoryCenteredPlacementGrid *grid;
    GList *unplaced_icons, *old_list;
    GList *l;
    gint current_monitor;

    grid = dory_centered_placement_grid_new (container, container->details->horizontal);

    if (grid == NULL) {
        return;
    }

    current_monitor = dory_desktop_utils_get_monitor_for_widget (GTK_WIDGET (container));

    unplaced_icons = g_list_copy (container->details->icons);

    unplaced_icons = g_list_sort_with_data (unplaced_icons,
                                            order_icons_by_visual_position,
                                            grid);

    if (dory_icon_container_is_layout_rtl (container)) {
        unplaced_icons = g_list_reverse (unplaced_icons);
    }

    for (l = unplaced_icons; l != NULL; l = l->next) {
        DoryIcon *icon;
        DoryIconPosition position;
        int x, y;

        icon = l->data;

        dory_centered_placement_grid_icon_position_to_nominal (grid,
                                                               icon,
                                                               icon->saved_ltr_x, icon->y,
                                                               &x, &y);

        snap_position (container, grid, icon, x, y, &x, &y);

        dory_centered_placement_grid_nominal_to_icon_position (grid,
                                                               icon,
                                                               x, y,
                                                               &x, &y);

        dory_icon_container_icon_set_position (container, icon, x, y);
        icon->saved_ltr_x = icon->x;
        dory_centered_placement_grid_mark_icon (grid, icon);

        position.x = icon->x;
        position.y = icon->y;
        position.scale = 1.0;
        position.monitor = current_monitor;

        store_new_icon_position (container, icon, position);
    }

    dory_centered_placement_grid_free (grid);

    old_list = container->details->icons;
    container->details->icons = unplaced_icons;

    g_list_free (old_list);

    if (dory_icon_container_is_layout_rtl (container)) {
        dory_icon_container_set_rtl_positions (container);
    }

    update_visual_selection_state (container);
}

static void
dory_icon_view_grid_container_reload_icon_positions (DoryIconContainer *container)
{
    GList *p, *no_position_icons;
    DoryIcon *icon;
    gboolean have_stored_position;
    DoryIconPosition position;

    g_assert (!container->details->auto_layout);

    dory_icon_container_resort (container);

    no_position_icons = NULL;

    /* Place all the icons with positions. */
    for (p = container->details->icons; p != NULL; p = p->next) {
        icon = p->data;

        have_stored_position = get_stored_icon_position (container,
                                                         icon->data,
                                                         &position);

        if (have_stored_position) {
            dory_icon_container_icon_set_position (container, icon, position.x, position.y);
        } else {
            no_position_icons = g_list_prepend (no_position_icons, icon);
        }
    }
    no_position_icons = g_list_reverse (no_position_icons);

    /* Place all the other icons. */
    DORY_ICON_CONTAINER_GET_CLASS (container)->lay_down_icons (container, no_position_icons, 0);
    g_list_free (no_position_icons);
}

static gboolean
assign_icon_position (DoryIconContainer *container,
                      DoryIcon *icon)
{
    gboolean have_stored_position;
    DoryIconPosition position;

    /* Get the stored position. */
    have_stored_position = FALSE;
    position.scale = 1.0;

    have_stored_position = get_stored_icon_position (container,
                                                     icon->data,
                                                     &position);

    icon->scale = position.scale;
    if (!container->details->auto_layout) {
        if (have_stored_position) {
            dory_icon_container_icon_set_position (container, icon, position.x, position.y);
            icon->saved_ltr_x = icon->x;
        } else {
            return FALSE;
        }
    }
    return TRUE;
}

static void
dory_icon_view_grid_container_finish_adding_new_icons (DoryIconContainer *container)
{
    GList *p, *new_icons, *no_position_icons, *semi_position_icons;
    DoryIcon *icon;
    gint current_monitor;

    current_monitor = dory_desktop_utils_get_monitor_for_widget (GTK_WIDGET (container));

    update_layout_constants (container);
    update_visual_selection_state (container);

    new_icons = container->details->new_icons;
    container->details->new_icons = NULL;

    /* Position most icons (not unpositioned manual-layout icons). */
    new_icons = g_list_reverse (new_icons);
    no_position_icons = semi_position_icons = NULL;

    for (p = new_icons; p != NULL; p = p->next) {
        icon = p->data;

        dory_icon_container_update_icon (container, icon);

        if (!container->details->auto_layout &&
            (icon->has_lazy_position || dory_icon_container_icon_is_new_for_monitor (container, icon, current_monitor))) {
            assign_icon_position (container, icon);
            semi_position_icons = g_list_prepend (semi_position_icons, icon);
        } else if (!assign_icon_position (container, icon)) {
            no_position_icons = g_list_prepend (no_position_icons, icon);
        }

        dory_icon_container_finish_adding_icon (container, icon);
    }

    g_list_free (new_icons);

    if (semi_position_icons != NULL) {
        DoryCenteredPlacementGrid *grid;

        g_assert (!container->details->auto_layout);

        semi_position_icons = g_list_reverse (semi_position_icons);

        /* This is currently only used on the desktop.
         * Thus, we pass FALSE for tight, like lay_down_icons_tblr */
        grid = dory_centered_placement_grid_new (container, container->details->horizontal);

        if (grid == NULL) {
           return;
        }

        dory_centered_placement_grid_pre_populate (grid, container->details->icons, TRUE);

        for (p = semi_position_icons; p != NULL; p = p->next) {
            DoryIconPosition position;
            int x, y;

            icon = p->data;

            dory_centered_placement_grid_icon_position_to_nominal (grid,
                                                                   icon,
                                                                   icon->x,
                                                                   icon->y,
                                                                   &x, &y);

            snap_position (container, grid, icon, x, y, &x, &y);

            dory_centered_placement_grid_nominal_to_icon_position (grid,
                                                                   icon,
                                                                   x, y,
                                                                   &x, &y);

            dory_icon_container_icon_set_position (container, icon, x, y);

            position.x = icon->x;
            position.y = icon->y;
            position.scale = icon->scale;
            position.monitor = current_monitor;
            dory_centered_placement_grid_mark_icon (grid, icon);
            store_new_icon_position (container, icon, position);

            /* ensure that next time we run this code, the formerly semi-positioned
             * icons are treated as being positioned. */
            icon->has_lazy_position = FALSE;
        }

        dory_centered_placement_grid_free (grid);
        g_list_free (semi_position_icons);
    }

    /* Position the unpositioned manual layout icons. */
    if (no_position_icons != NULL) {
        g_assert (!container->details->auto_layout);

        dory_icon_container_sort_icons (container, &no_position_icons);

        DORY_ICON_CONTAINER_GET_CLASS (container)->lay_down_icons (container, no_position_icons, 0);

        g_list_free (no_position_icons);
    }

    if (container->details->store_layout_timestamps_when_finishing_new_icons) {
        dory_icon_container_store_layout_timestamps_now (container);
        container->details->store_layout_timestamps_when_finishing_new_icons = FALSE;
    }
}

static gboolean
should_place_before (gint          x,
                     gint          y,
                     GdkRectangle *rect,
                     gboolean      horizontal)
{
    gint half_x, half_y;

    half_x = rect->x + (rect->width / 2);
    half_y = rect->y + (rect->height / 2);

    if (horizontal) {
        return x < half_x;
    } else {
        return y < half_y;
    }

    return FALSE;
}

static void
draw_insert_stroke (cairo_t *cr,
                    gint     draw_x,
                    gint     draw_y,
                    gint     draw_distance_x,
                    gint     draw_distance_y)
{
    GdkRGBA shadow_rgba = { 0, 0, 0, 1.0 };
    GdkRGBA fore_rgba = { 1.0, 1.0, 1.0, 1.0};

    cairo_set_antialias (cr, CAIRO_ANTIALIAS_BEST);

    cairo_save (cr);

    cairo_set_line_width (cr, 1.0);
    cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);

    gdk_cairo_set_source_rgba (cr, &shadow_rgba);

    cairo_move_to (cr, draw_x, draw_y);
    cairo_line_to (cr, draw_x + draw_distance_x, draw_y + draw_distance_y);
    cairo_stroke (cr);

    cairo_restore (cr);

    draw_x--;
    draw_y--;

    cairo_save (cr);

    cairo_set_line_width (cr, 1.0);
    cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);

    gdk_cairo_set_source_rgba (cr, &fore_rgba);

    cairo_move_to (cr, draw_x, draw_y);
    cairo_line_to (cr, draw_x + draw_distance_x, draw_y + draw_distance_y);
    cairo_stroke (cr);

    cairo_restore (cr);
}

static void
dory_icon_view_grid_container_draw_background (EelCanvas *canvas,
                                               cairo_t   *cr)
{
    DoryIconContainer *container;
    DoryIcon *target_icon;
    // GtkAllocation allocation;
    gboolean before;

    container = DORY_ICON_CONTAINER (canvas);

    if (DEBUGGING) {
        DoryCenteredPlacementGrid *grid;

        grid = dory_centered_placement_grid_new (container, container->details->horizontal);

        if (grid != NULL) {
            GdkRectangle grid_rect;
            gint count;
            GdkRGBA grid_color = { 1.0, 1.0, 1.0, 0.4};


            count = 0;

            cairo_save (cr);
            gdk_cairo_set_source_rgba (cr, &grid_color);
            cairo_set_line_width (cr, 1.0);

            dory_centered_placement_grid_get_current_position_rect (grid, 0, 0, &grid_rect, NULL);

            while (count < grid->num_rows * grid->num_columns) {
                GdkRectangle draw_rect;

                draw_rect.x = grid_rect.x + container->details->left_margin;
                draw_rect.y = grid_rect.y + container->details->top_margin;
                draw_rect.width = grid_rect.width;
                draw_rect.height = grid_rect.height;

                gdk_cairo_rectangle (cr, &draw_rect);

                dory_centered_placement_grid_get_next_position_rect (grid, &grid_rect, &grid_rect, NULL);

                count++;
            }
        }

        dory_centered_placement_grid_free (grid);

        cairo_stroke (cr);
        cairo_restore (cr);
    }

    if (!container->details->insert_dnd_mode ||
        container->details->dnd_info->drag_info.data_type != DORY_ICON_DND_GNOME_ICON_LIST) {
        return;
    }

    target_icon = container->details->drop_target;

    if (target_icon == NULL) {
        DoryCenteredPlacementGrid *grid;
        GdkRectangle grid_rect;
        gint grid_cs_x, grid_cs_y;
        gint draw_x, draw_y, draw_distance_x, draw_distance_y;
        gboolean is_free;

        grid = container->details->dnd_grid;

        /* Canvas draw vfunc encompasses the margins.  The placement grid coord system
         * does not - grid x and y are relative to the inside corner of the top and left canvas
         * margins.  So we have to shift and then unshift by the margins here.  The grid functions
         * will handle any grid padding calculations (grid->borders). */

        grid_cs_x = container->details->current_dnd_x - container->details->left_margin;
        grid_cs_y = container->details->current_dnd_y - container->details->top_margin;

        dory_centered_placement_grid_get_current_position_rect (grid,
                                                                grid_cs_x,
                                                                grid_cs_y,
                                                                &grid_rect,
                                                                &is_free);

        grid_rect.x += container->details->left_margin;
        grid_rect.y += container->details->top_margin;

        if (DEBUGGING) {
            GdkRGBA active_rect_color = { 0.5, 0.5, 0.5, 0.4};

            cairo_save (cr);
            gdk_cairo_set_source_rgba (cr, &active_rect_color);

            gdk_cairo_rectangle (cr, &grid_rect);
            cairo_fill (cr);

            cairo_restore (cr);
        }

        before = should_place_before (container->details->current_dnd_x,
                                      container->details->current_dnd_y,
                                      &grid_rect,
                                      container->details->horizontal);

        if (container->details->horizontal) {
            if (!is_free && before) {
                draw_x = grid_rect.x;
                draw_y = grid_rect.y + (grid_rect.height * .2);
                draw_distance_x = 0;
                draw_distance_y = grid_rect.height * .6;

                draw_insert_stroke (cr, draw_x, draw_y, draw_distance_x, draw_distance_y);
            }

            if (!is_free && !before) {
                draw_x = grid_rect.x + grid_rect.width;
                draw_y = grid_rect.y + (grid_rect.height * .2);
                draw_distance_x = 0;
                draw_distance_y = grid_rect.height * .6;

                draw_insert_stroke (cr, draw_x, draw_y, draw_distance_x, draw_distance_y);
            }
        } else {
            if (!is_free && before) {
                draw_x = grid_rect.x + (grid_rect.width * .2);
                draw_y = grid_rect.y;
                draw_distance_x = grid_rect.width * .6;
                draw_distance_y = 0;

                draw_insert_stroke (cr, draw_x, draw_y, draw_distance_x, draw_distance_y);
            }

            if (!is_free && !before) {
                draw_x = grid_rect.x + (grid_rect.width * .2);
                draw_y = grid_rect.y + grid_rect.height;
                draw_distance_x = grid_rect.width * .6;
                draw_distance_y = 0;

                draw_insert_stroke (cr, draw_x, draw_y, draw_distance_x, draw_distance_y);
            }
        }
    }
}

#define BASE_SNAP_SIZE_X 130.0
#define BASE_SNAP_SIZE_Y 100.0
#define BASE_MAX_TEXT_WIDTH 120.0

/* from dory-icon-canvas-item.c */
#define LABEL_OFFSET 1

static gint
get_vertical_adjustment (DoryIconContainer *container,
                         gint               icon_size)
{
    PangoLayout *layout;
    PangoContext *context;
    PangoFontDescription *desc;
    gint ellipsis_limit;
    gint height;

    ellipsis_limit = g_settings_get_int (dory_desktop_preferences, DORY_PREFERENCES_DESKTOP_TEXT_ELLIPSIS_LIMIT);

    layout = gtk_widget_create_pango_layout (GTK_WIDGET (container), "Test");

    if (container->details->font && g_strcmp0 (container->details->font, "") != 0) {
        desc = pango_font_description_from_string (container->details->font);
    } else {
        context = gtk_widget_get_pango_context (GTK_WIDGET (container));
        desc = pango_font_description_copy (pango_context_get_font_description (context));
    }

    if (pango_font_description_get_size (desc) > 0) {
        pango_font_description_set_size (desc,
                                         pango_font_description_get_size (desc) +
                                         container->details->font_size_table [container->details->zoom_level]);
    }

    pango_layout_set_font_description (layout, desc);

    pango_font_description_free (desc);

    pango_layout_get_pixel_size (layout,
                                 NULL,
                                 &height);

    g_object_unref (layout);

    height *= ellipsis_limit;

    height += icon_size + GET_VIEW_CONSTANT (container, icon_pad_bottom) + LABEL_OFFSET;

    return height / 2;
}

static void
update_layout_constants (DoryIconContainer *container)
{
    gint icon_size, ellipsis_pref;
    DoryViewLayoutConstants *constants;
    gdouble scale, h_adjust, v_adjust;

    update_auto_strv_as_quarks (dory_icon_view_preferences,
                                DORY_PREFERENCES_ICON_VIEW_CAPTIONS,
                                &(DORY_ICON_VIEW_GRID_CONTAINER (container)->attributes));

    ellipsis_pref = g_settings_get_int (dory_desktop_preferences, DORY_PREFERENCES_DESKTOP_TEXT_ELLIPSIS_LIMIT);
    DORY_ICON_VIEW_GRID_CONTAINER (container)->text_ellipsis_limit = ellipsis_pref;

    icon_size = dory_get_desktop_icon_size_for_zoom_level (container->details->zoom_level);

    scale = (double) icon_size / DORY_DESKTOP_ICON_SIZE_STANDARD;

    h_adjust = container->details->h_adjust / 100.0;
    v_adjust = container->details->v_adjust / 100.0;

    constants = container->details->view_constants;

    constants->snap_size_x = BASE_SNAP_SIZE_X * scale * h_adjust;
    constants->snap_size_y = BASE_SNAP_SIZE_Y * scale * v_adjust;
    constants->max_text_width_standard = BASE_MAX_TEXT_WIDTH * scale * h_adjust;

    constants->icon_vertical_adjust = MIN (get_vertical_adjustment (container, icon_size), constants->snap_size_y / 2);
    /* This isn't what this is intended for, but it's a simple way vs. overriding what
     * icon_get_size() uses to get the icon size in dory-icon-container.c (it should use
     * dory_get_desktop_icon_size_for_zoom_level) */
    dory_icon_container_set_forced_icon_size (container, icon_size);

    gtk_widget_queue_draw (GTK_WIDGET (container));
}

static void
dory_icon_view_grid_container_set_zoom_level (DoryIconContainer *container, gint new_level)
{
    DoryIconContainerDetails *details;
    int pinned_level;

    details = container->details;

    dory_icon_container_end_renaming_mode (container, TRUE);

    pinned_level = new_level;
    if (pinned_level < DORY_ZOOM_LEVEL_SMALLEST) {
        pinned_level = DORY_ZOOM_LEVEL_SMALLEST;
    } else if (pinned_level > DORY_ZOOM_LEVEL_LARGEST) {
        pinned_level = DORY_ZOOM_LEVEL_LARGEST;
    }

    if (pinned_level == details->zoom_level) {
        return;
    }

    details->zoom_level = pinned_level;

    eel_canvas_set_pixels_per_unit (EEL_CANVAS (container), 1.0);
}

static void
desktop_text_ellipsis_limit_changed_callback (DoryIconContainer *container)
{
    dory_icon_container_invalidate_labels (container);
    dory_icon_container_request_update_all (container);
}

static gint
get_layout_adjust_for_additional_attributes (DoryIconContainer *container)
{
    GQuark *attrs;
    gint length;

    attrs = dory_icon_view_grid_container_get_icon_text_attribute_names (container, &length);

    if (length == 0) {
        return 0;
    }

    if (attrs[0] == attribute_none_q) {
        return 0;
    }

    return GRID_VIEW_MAX_ADDITIONAL_ATTRIBUTES;
}

static gint
dory_icon_view_grid_container_get_max_layout_lines_for_pango (DoryIconContainer  *container)
{
    int limit;

    limit = MAX (1,   DORY_ICON_VIEW_GRID_CONTAINER (container)->text_ellipsis_limit
                    - get_layout_adjust_for_additional_attributes (container));

    if (limit <= 0) {
        return G_MININT;
    }

    return -limit;
}

static gint
dory_icon_view_grid_container_get_max_layout_lines (DoryIconContainer  *container)
{
    int limit;

    limit = MAX (1,   DORY_ICON_VIEW_GRID_CONTAINER (container)->text_ellipsis_limit
                    - get_layout_adjust_for_additional_attributes (container));

    if (limit <= 0) {
        return G_MAXINT;
    }

    return limit;
}

static gint
dory_icon_view_grid_container_get_additional_text_line_count (DoryIconContainer *container)
{
    return quarkv_length (DORY_ICON_VIEW_GRID_CONTAINER (container)->attributes);
}

static void
captions_changed_callback (DoryIconContainer *container)
{
    update_auto_strv_as_quarks (dory_icon_view_preferences,
                                DORY_PREFERENCES_ICON_VIEW_CAPTIONS,
                                &(DORY_ICON_VIEW_GRID_CONTAINER (container)->attributes));

    dory_icon_container_invalidate_labels (container);
    dory_icon_container_request_update_all (container);
}

static gchar *
on_get_tooltip_text (DoryIconContainer *container,
                     DoryFile          *file,
                     gpointer           user_data)
{
    gchar *tooltip_text = NULL;

    if (container->details->show_desktop_tooltips) {
        tooltip_text = dory_file_construct_tooltip (file, container->details->tooltip_flags, NULL);
    }

    return tooltip_text;
}

static void
dory_icon_view_grid_container_icon_added (DoryIconViewGridContainer *container,
                                          DoryIconData              *icon_data,
                                          gpointer                   data)
{
}

static void
dory_icon_view_grid_container_icon_removed (DoryIconViewGridContainer *container,
                                            DoryIconData              *icon_data,
                                            gpointer                   data)
{
}

DoryIconContainer *
dory_icon_view_grid_container_construct (DoryIconViewGridContainer *icon_container,
                                         DoryIconView              *view,
                                         gboolean                   is_desktop)
{
    AtkObject *atk_obj;
    DoryViewLayoutConstants *constants = DORY_ICON_CONTAINER (icon_container)->details->view_constants;

    g_return_val_if_fail (DORY_IS_ICON_VIEW (view), NULL);

    icon_container->view = view;
    dory_icon_container_set_is_desktop (DORY_ICON_CONTAINER (icon_container), is_desktop);

    atk_obj = gtk_widget_get_accessible (GTK_WIDGET (icon_container));
    atk_object_set_name (atk_obj, _("Icon View"));

    constants = DORY_ICON_CONTAINER (icon_container)->details->view_constants;

    constants->icon_pad_left = 4;
    constants->icon_pad_right = 4;
    constants->icon_pad_top = 4;
    constants->icon_pad_bottom = 4;
    constants->container_pad_left = 4;
    constants->container_pad_right = 4;
    constants->container_pad_top = 4;
    constants->container_pad_bottom = 4;
    constants->standard_icon_grid_width = 155; // Not used
    constants->text_beside_icon_grid_width = 205; // Not used
    constants->desktop_pad_horizontal = 10; // Not used
    constants->desktop_pad_vertical = 10; // Not used
    constants->snap_size_x = BASE_SNAP_SIZE_X;
    constants->snap_size_y = BASE_SNAP_SIZE_Y;
    constants->max_text_width_standard = BASE_MAX_TEXT_WIDTH;
    constants->max_text_width_beside = 90; // Not used
    constants->max_text_width_beside_top_to_bottom = 150; // Not used
    constants->icon_vertical_adjust = 20;

    g_signal_connect_swapped (dory_desktop_preferences,
                              "changed::" DORY_PREFERENCES_DESKTOP_TEXT_ELLIPSIS_LIMIT,
                              G_CALLBACK (desktop_text_ellipsis_limit_changed_callback),
                              DORY_ICON_CONTAINER (icon_container));

    g_signal_connect_swapped (dory_icon_view_preferences,
                              "changed::" DORY_PREFERENCES_ICON_VIEW_CAPTIONS,
                              G_CALLBACK (captions_changed_callback),
                              DORY_ICON_CONTAINER (icon_container));

    g_signal_connect (icon_container,
                      "get-tooltip-text",
                      G_CALLBACK (on_get_tooltip_text),
                      NULL);

    return DORY_ICON_CONTAINER (icon_container);
}

static void
finalize (GObject *object)
{
    g_signal_handlers_disconnect_by_func (dory_desktop_preferences,
                                          desktop_text_ellipsis_limit_changed_callback,
                                          object);

    g_signal_handlers_disconnect_by_func (dory_icon_view_preferences,
                                          captions_changed_callback,
                                          object);

    G_OBJECT_CLASS (dory_icon_view_grid_container_parent_class)->finalize (object);
}

static void
dory_icon_view_grid_container_class_init (DoryIconViewGridContainerClass *klass)
{
	DoryIconContainerClass *ic_class;

	ic_class = &klass->parent_class;

	attribute_none_q = g_quark_from_static_string ("none");

    G_OBJECT_CLASS (klass)->finalize = finalize;

    ic_class->is_grid_container = TRUE;

	ic_class->get_icon_text = dory_icon_view_grid_container_get_icon_text;
	ic_class->get_icon_images = dory_icon_view_grid_container_get_icon_images;
	ic_class->get_icon_description = dory_icon_view_grid_container_get_icon_description;
    ic_class->get_max_layout_lines_for_pango = dory_icon_view_grid_container_get_max_layout_lines_for_pango;
    ic_class->get_max_layout_lines = dory_icon_view_grid_container_get_max_layout_lines;
    ic_class->get_additional_text_line_count = dory_icon_view_grid_container_get_additional_text_line_count;

	ic_class->compare_icons = dory_icon_view_grid_container_compare_icons;
	ic_class->freeze_updates = dory_icon_view_grid_container_freeze_updates;
	ic_class->unfreeze_updates = dory_icon_view_grid_container_unfreeze_updates;
    ic_class->lay_down_icons = dory_icon_view_grid_container_lay_down_icons;
    ic_class->icon_set_position = dory_icon_view_grid_container_icon_set_position;
    ic_class->move_icon = dory_icon_view_grid_container_move_icon;
    ic_class->update_icon = dory_icon_view_grid_container_update_icon;
    ic_class->align_icons = dory_icon_view_grid_container_align_icons;
    ic_class->reload_icon_positions = dory_icon_view_grid_container_reload_icon_positions;
    ic_class->finish_adding_new_icons = dory_icon_view_grid_container_finish_adding_new_icons;
    ic_class->icon_get_bounding_box = dory_icon_view_grid_container_icon_get_bounding_box;
    ic_class->set_zoom_level = dory_icon_view_grid_container_set_zoom_level;

    g_signal_override_class_handler ("icon_added",
                                     DORY_TYPE_ICON_VIEW_GRID_CONTAINER,
                                     G_CALLBACK (dory_icon_view_grid_container_icon_added));

    g_signal_override_class_handler ("icon_removed",
                                     DORY_TYPE_ICON_VIEW_GRID_CONTAINER,
                                     G_CALLBACK (dory_icon_view_grid_container_icon_removed));

    EEL_CANVAS_CLASS (klass)->draw_background = dory_icon_view_grid_container_draw_background;
}

static void
dory_icon_view_grid_container_init (DoryIconViewGridContainer *icon_container)
{
	gtk_style_context_add_class (gtk_widget_get_style_context (GTK_WIDGET (icon_container)),
				     GTK_STYLE_CLASS_VIEW);

    DORY_ICON_CONTAINER (icon_container)->details->font_size_table[DORY_ZOOM_LEVEL_SMALL] = -1 * PANGO_SCALE;
    DORY_ICON_CONTAINER (icon_container)->details->font_size_table[DORY_ZOOM_LEVEL_LARGE] =  1 * PANGO_SCALE;

    icon_container->text_ellipsis_limit = 2;
}

DoryIconContainer *
dory_icon_view_grid_container_new (DoryIconView *view,
                                   gboolean      is_desktop)
{
    return dory_icon_view_grid_container_construct (g_object_new (DORY_TYPE_ICON_VIEW_GRID_CONTAINER, NULL),
                                                    view,
                                                    is_desktop);
}

void
dory_icon_view_grid_container_set_sort_desktop (DoryIconViewGridContainer *container,
					       gboolean         desktop)
{
	container->sort_for_desktop = desktop;
}
