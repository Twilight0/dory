/* -*- Mode: C; indent-tabs-mode: f; c-basic-offset: 4; tab-width: 4 -*- */

/* gnome-icon-container.h - Icon container widget.

   Copyright (C) 1999, 2000 Free Software Foundation
   Copyright (C) 2000 Eazel, Inc.

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

   Authors: Ettore Perazzoli <ettore@gnu.org>, Darin Adler <darin@bentspoon.com>
*/

#ifndef DORY_ICON_CONTAINER_H
#define DORY_ICON_CONTAINER_H

#include <eel/eel-canvas.h>
#include <libdory-private/dory-icon-info.h>
#include <libdory-private/dory-icon.h>

#define DORY_TYPE_ICON_CONTAINER dory_icon_container_get_type()
#define DORY_ICON_CONTAINER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_ICON_CONTAINER, DoryIconContainer))
#define DORY_ICON_CONTAINER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_ICON_CONTAINER, DoryIconContainerClass))
#define DORY_IS_ICON_CONTAINER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_ICON_CONTAINER))
#define DORY_IS_ICON_CONTAINER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_ICON_CONTAINER))
#define DORY_ICON_CONTAINER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_ICON_CONTAINER, DoryIconContainerClass))

/* Initial unpositioned icon value */
#define ICON_UNPOSITIONED_VALUE -1

typedef struct {
	int x;
	int y;
	double scale;
    int monitor;
} DoryIconPosition;

typedef enum {
	DORY_ICON_LAYOUT_L_R_T_B,
	DORY_ICON_LAYOUT_R_L_T_B,
	DORY_ICON_LAYOUT_T_B_L_R,
	DORY_ICON_LAYOUT_T_B_R_L
} DoryIconLayoutMode;

typedef enum {
	DORY_ICON_LABEL_POSITION_UNDER,
	DORY_ICON_LABEL_POSITION_BESIDE
} DoryIconLabelPosition;

#define	DORY_ICON_CONTAINER_TYPESELECT_FLUSH_DELAY 1000000

typedef struct DoryIconContainerDetails DoryIconContainerDetails;

typedef struct {
	EelCanvas canvas;
	DoryIconContainerDetails *details;
} DoryIconContainer;

typedef struct {
	EelCanvasClass parent_slot;
    gboolean is_grid_container;

	/* Operations on the container. */
	int          (* button_press) 	          (DoryIconContainer *container,
						   GdkEventButton *event);
	void         (* context_click_background) (DoryIconContainer *container,
						   GdkEventButton *event);
	void         (* middle_click) 		  (DoryIconContainer *container,
						   GdkEventButton *event);

	/* Operations on icons. */
	void         (* activate)	  	  (DoryIconContainer *container,
						   DoryIconData *data);
	void         (* activate_alternate)       (DoryIconContainer *container,
						   DoryIconData *data);
	void         (* activate_previewer)       (DoryIconContainer *container,
						   GList *files,
						   GArray *locations);
	void         (* context_click_selection)  (DoryIconContainer *container,
						   GdkEventButton *event);
	void	     (* move_copy_items)	  (DoryIconContainer *container,
						   const GList *item_uris,
						   GdkPoint *relative_item_points,
						   const char *target_uri,
						   GdkDragAction action,
						   int x,
						   int y);
	void	     (* handle_netscape_url)	  (DoryIconContainer *container,
						   const char *url,
						   const char *target_uri,
						   GdkDragAction action,
						   int x,
						   int y);
	void	     (* handle_uri_list)    	  (DoryIconContainer *container,
						   const char *uri_list,
						   const char *target_uri,
						   GdkDragAction action,
						   int x,
						   int y);
	void	     (* handle_text)		  (DoryIconContainer *container,
						   const char *text,
						   const char *target_uri,
						   GdkDragAction action,
						   int x,
						   int y);
	void	     (* handle_raw)		  (DoryIconContainer *container,
						   char *raw_data,
						   int length,
						   const char *target_uri,
						   const char *direct_save_uri,
						   GdkDragAction action,
						   int x,
						   int y);

	/* Queries on the container for subclass/client.
	 * These must be implemented. The default "do nothing" is not good enough.
	 */
	char *	     (* get_container_uri)	  (DoryIconContainer *container);

	/* Queries on icons for subclass/client.
	 * These must be implemented. The default "do nothing" is not
	 * good enough, these are _not_ signals.
	 */
	DoryIconInfo *(* get_icon_images)     (DoryIconContainer *container,
						   DoryIconData *data,
						   int icon_size,
						   gboolean for_drag_accept,
						   gboolean *has_window_open,
                           gboolean visible);
	void         (* get_icon_text)            (DoryIconContainer *container,
						   DoryIconData *data,
						   char **editable_text,
						   char **additional_text,
                           gboolean *pinned,
                           gboolean *fav_unavailable,
						   gboolean include_invisible);
    void         (* update_icon)              (DoryIconContainer *container,
                                               DoryIcon          *icon,
                                               gboolean           visible);
	char *       (* get_icon_description)     (DoryIconContainer *container,
						   DoryIconData *data);
	int          (* compare_icons)            (DoryIconContainer *container,
						   DoryIconData *icon_a,
						   DoryIconData *icon_b);
	void         (* freeze_updates)           (DoryIconContainer *container);
	void         (* unfreeze_updates)         (DoryIconContainer *container);

    gint         (* get_max_layout_lines_for_pango) (DoryIconContainer *container);
    gint         (* get_max_layout_lines)           (DoryIconContainer *container);
    gint         (* get_additional_text_line_count) (DoryIconContainer *container);

	/* Queries on icons for subclass/client.
	 * These must be implemented => These are signals !
	 * The default "do nothing" is not good enough.
	 */
	gboolean     (* can_accept_item)	  (DoryIconContainer *container,
						   DoryIconData *target, 
						   const char *item_uri);
	char *       (* get_icon_uri)             (DoryIconContainer *container,
						   DoryIconData *data);
	char *       (* get_icon_drop_target_uri) (DoryIconContainer *container,
						   DoryIconData *data);

	/* If icon data is NULL, the layout timestamp of the container should be retrieved.
	 * That is the time when the container displayed a fully loaded directory with
	 * all icon positions assigned.
	 *
	 * If icon data is not NULL, the position timestamp of the icon should be retrieved.
	 * That is the time when the file (i.e. icon data payload) was last displayed in a
	 * fully loaded directory with all icon positions assigned.
	 */
	gboolean     (* get_stored_layout_timestamp) (DoryIconContainer *container,
						      DoryIconData *data,
						      time_t *time);
	/* If icon data is NULL, the layout timestamp of the container should be stored.
	 * If icon data is not NULL, the position timestamp of the container should be stored.
	 */
	gboolean     (* store_layout_timestamp) (DoryIconContainer *container,
						 DoryIconData *data,
						 const time_t *time);

    void         (*lay_down_icons) (DoryIconContainer *container, GList *icons, double start_y);
    void         (*icon_set_position) (DoryIconContainer *container, DoryIcon *icon, double x, double y);
    void         (*move_icon) (DoryIconContainer *container, DoryIcon *icon, int x, int y,
                               double scale, gboolean raise, gboolean snap, gboolean update_position);
    void         (*align_icons) (DoryIconContainer *container);
    void         (*finish_adding_new_icons) (DoryIconContainer *container);
    void         (*reload_icon_positions) (DoryIconContainer *container);
    void         (*icon_get_bounding_box) (DoryIcon *icon,
                                           int *x1_return, int *y1_return,
                                           int *x2_return, int *y2_return,
                                           DoryIconCanvasItemBoundsUsage usage);
    void         (*set_zoom_level)        (DoryIconContainer *container, gint new_level);
	/* Notifications for the whole container. */
	void	     (* band_select_started)	  (DoryIconContainer *container);
	void	     (* band_select_ended)	  (DoryIconContainer *container);
	void         (* selection_changed) 	  (DoryIconContainer *container);
	void         (* layout_changed)           (DoryIconContainer *container);

	/* Notifications for icons. */
	void         (* icon_position_changed)    (DoryIconContainer *container,
						   DoryIconData *data,
						   const DoryIconPosition *position);
	void         (* icon_rename_started)      (DoryIconContainer *container,
						   GtkWidget *renaming_widget);
	void         (* icon_rename_ended)        (DoryIconContainer *container,
						   DoryIconData *data,
						   const char *text);
	void	     (* icon_stretch_started)     (DoryIconContainer *container,
						   DoryIconData *data);
	void	     (* icon_stretch_ended)       (DoryIconContainer *container,
						   DoryIconData *data);
	int	     (* preview)		  (DoryIconContainer *container,
						   DoryIconData *data,
						   gboolean start_flag);
        void         (* icon_added)               (DoryIconContainer *container,
                                                   DoryIconData *data);
        void         (* icon_removed)             (DoryIconContainer *container,
                                                   DoryIconData *data);
        void         (* cleared)                  (DoryIconContainer *container);
	gboolean     (* start_interactive_search) (DoryIconContainer *container);
} DoryIconContainerClass;

/* GtkObject */
GType             dory_icon_container_get_type                      (void);
GtkWidget *       dory_icon_container_new                           (void);


/* adding, removing, and managing icons */
void              dory_icon_container_clear                         (DoryIconContainer  *view);
gboolean          dory_icon_container_icon_is_new_for_monitor       (DoryIconContainer *container,
                                                                     DoryIcon          *icon,
                                                                     gint               current_monitor);
gboolean          dory_icon_container_add                           (DoryIconContainer  *view,
									 DoryIconData       *data);
void              dory_icon_container_layout_now                    (DoryIconContainer *container);
gboolean          dory_icon_container_remove                        (DoryIconContainer  *view,
									 DoryIconData       *data);
void              dory_icon_container_for_each                      (DoryIconContainer  *view,
									 DoryIconCallback    callback,
									 gpointer                callback_data);
void              dory_icon_container_request_update                (DoryIconContainer  *view,
									 DoryIconData       *data);
void              dory_icon_container_invalidate_labels             (DoryIconContainer  *container);
void              dory_icon_container_request_update_all            (DoryIconContainer  *container);
void              dory_icon_container_reveal                        (DoryIconContainer  *container,
									 DoryIconData       *data);
gboolean          dory_icon_container_is_empty                      (DoryIconContainer  *container);
DoryIconData *dory_icon_container_get_first_visible_icon        (DoryIconContainer  *container);
void              dory_icon_container_scroll_to_icon                (DoryIconContainer  *container,
									 DoryIconData       *data);

void              dory_icon_container_begin_loading                 (DoryIconContainer  *container);
void              dory_icon_container_end_loading                   (DoryIconContainer  *container,
									 gboolean                all_icons_added);

/* control the layout */
gboolean          dory_icon_container_is_auto_layout                (DoryIconContainer  *container);
void              dory_icon_container_set_auto_layout               (DoryIconContainer  *container,
									 gboolean                auto_layout);

gboolean          dory_icon_container_is_keep_aligned               (DoryIconContainer  *container);
void              dory_icon_container_set_keep_aligned              (DoryIconContainer  *container,
									 gboolean                keep_aligned);
void              dory_icon_container_set_layout_mode               (DoryIconContainer  *container,
									 DoryIconLayoutMode  mode);
void              dory_icon_container_set_horizontal_layout (DoryIconContainer *container,
                                                             gboolean           horizontal);
gboolean          dory_icon_container_get_horizontal_layout (DoryIconContainer *container);
void              dory_icon_container_set_grid_adjusts (DoryIconContainer *container,
                                                        gint               h_adjust,
                                                        gint               v_adjust);

void              dory_icon_container_set_label_position            (DoryIconContainer  *container,
									 DoryIconLabelPosition pos);
void              dory_icon_container_sort                          (DoryIconContainer  *container);
void              dory_icon_container_freeze_icon_positions         (DoryIconContainer  *container);

gint               dory_icon_container_get_max_layout_lines           (DoryIconContainer  *container);
gint               dory_icon_container_get_max_layout_lines_for_pango (DoryIconContainer  *container);

void              dory_icon_container_set_highlighted_for_clipboard (DoryIconContainer  *container,
									 GList                  *clipboard_icon_data);

/* operations on all icons */
void              dory_icon_container_unselect_all                  (DoryIconContainer  *view);
void              dory_icon_container_select_all                    (DoryIconContainer  *view);
void              dory_icon_container_select_first                  (DoryIconContainer  *container);


/* operations on the selection */
void              dory_icon_container_update_selection              (DoryIconContainer *container);
GList     *       dory_icon_container_get_selection                 (DoryIconContainer  *view);
GList     *       dory_icon_container_peek_selection                (DoryIconContainer  *view);
gint              dory_icon_container_get_selection_count           (DoryIconContainer  *container);
void			  dory_icon_container_invert_selection				(DoryIconContainer  *view);
void              dory_icon_container_set_selection                 (DoryIconContainer  *view,
									 GList                  *selection);
GArray    *       dory_icon_container_get_selected_icon_locations   (DoryIconContainer  *view);
gboolean          dory_icon_container_has_stretch_handles           (DoryIconContainer  *container);
gboolean          dory_icon_container_is_stretched                  (DoryIconContainer  *container);
void              dory_icon_container_show_stretch_handles          (DoryIconContainer  *container);
void              dory_icon_container_unstretch                     (DoryIconContainer  *container);
void              dory_icon_container_start_renaming_selected_item  (DoryIconContainer  *container,
									 gboolean                select_all);
/* options */
DoryZoomLevel dory_icon_container_get_zoom_level                (DoryIconContainer  *view);
void              dory_icon_container_set_zoom_level                (DoryIconContainer  *view,
									 int                     new_zoom_level);
void              dory_icon_container_set_single_click_mode         (DoryIconContainer  *container,
									 gboolean                single_click_mode);
void              dory_icon_container_set_click_to_rename_enabled (DoryIconContainer *container,
                                                                             gboolean enabled);
void              dory_icon_container_enable_linger_selection       (DoryIconContainer  *view,
									 gboolean                enable);
gboolean          dory_icon_container_get_is_fixed_size             (DoryIconContainer  *container);
void              dory_icon_container_set_is_fixed_size             (DoryIconContainer  *container,
									 gboolean                is_fixed_size);
gboolean          dory_icon_container_get_is_desktop                (DoryIconContainer  *container);
void              dory_icon_container_set_is_desktop                (DoryIconContainer  *container,
									 gboolean                is_desktop);
gboolean          dory_icon_container_get_show_desktop_tooltips     (DoryIconContainer *container);
void              dory_icon_container_set_show_desktop_tooltips     (DoryIconContainer *container,
                                                                              gboolean  show_tooltips);
void              dory_icon_container_set_filter_highlight          (DoryIconContainer  *container,
									 const char             *filter_text);
const char       *dory_icon_container_get_filter_highlight          (DoryIconContainer  *container);

void              dory_icon_container_reset_scroll_region           (DoryIconContainer  *container);
void              dory_icon_container_set_font                      (DoryIconContainer  *container,
									 const char             *font); 
void              dory_icon_container_set_font_size_table           (DoryIconContainer  *container,
									 const int               font_size_table[DORY_ZOOM_LEVEL_LARGEST + 1]);
void              dory_icon_container_set_margins                   (DoryIconContainer  *container,
									 int                     left_margin,
									 int                     right_margin,
									 int                     top_margin,
									 int                     bottom_margin);
void              dory_icon_container_set_use_drop_shadows          (DoryIconContainer  *container,
									 gboolean                use_drop_shadows);
char*             dory_icon_container_get_icon_description          (DoryIconContainer  *container,
                                                                         DoryIconData       *data);
gboolean          dory_icon_container_get_allow_moves               (DoryIconContainer  *container);
void              dory_icon_container_set_allow_moves               (DoryIconContainer  *container,
									 gboolean                allow_moves);
void		  dory_icon_container_set_forced_icon_size		(DoryIconContainer  *container,
									 int                     forced_icon_size);
void		  dory_icon_container_set_all_columns_same_width	(DoryIconContainer  *container,
									 gboolean                all_columns_same_width);

gboolean	  dory_icon_container_is_layout_rtl			(DoryIconContainer  *container);
gboolean	  dory_icon_container_is_layout_vertical		(DoryIconContainer  *container);

gboolean          dory_icon_container_get_store_layout_timestamps   (DoryIconContainer  *container);
void              dory_icon_container_set_store_layout_timestamps   (DoryIconContainer  *container,
									 gboolean                store_layout);

void              dory_icon_container_widget_to_file_operation_position (DoryIconContainer *container,
									     GdkPoint              *position);

void         dory_icon_container_setup_tooltip_preference_callback (DoryIconContainer *container);
void         dory_icon_container_update_tooltip_text (DoryIconContainer  *container,
                                                      DoryIconCanvasItem *item);
gint         dory_icon_container_get_additional_text_line_count (DoryIconContainer *container);
void         dory_icon_container_set_ok_to_load_deferred_attrs (DoryIconContainer *container,
                                                                gboolean           ok);
#endif /* DORY_ICON_CONTAINER_H */
