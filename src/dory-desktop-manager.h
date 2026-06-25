/* dory-desktop-manager.h */

#ifndef _DORY_DESKTOP_MANAGER_H
#define _DORY_DESKTOP_MANAGER_H

#include <glib-object.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>

#include "dory-window.h"

G_BEGIN_DECLS

#define DORY_TYPE_DESKTOP_MANAGER dory_desktop_manager_get_type ()

G_DECLARE_FINAL_TYPE (DoryDesktopManager, dory_desktop_manager, DORY, DESKTOP_MANAGER, GObject)

typedef enum {
    DESKTOP_ARRANGE_VERTICAL,
    DESKTOP_ARRANGE_HORIZONTAL
} DoryDesktopLayoutDirection;

DoryDesktopManager* dory_desktop_manager_get (void);

gboolean dory_desktop_manager_has_desktop_windows (DoryDesktopManager *manager);
gboolean dory_desktop_manager_get_monitor_is_active (DoryDesktopManager *manager,
                                                                   gint  monitor);
gboolean dory_desktop_manager_get_monitor_is_primary (DoryDesktopManager *manager,
                                                                   gint  monitor);

gboolean dory_desktop_manager_get_primary_only (DoryDesktopManager *manager);
void     dory_desktop_manager_get_window_rect_for_monitor (DoryDesktopManager *manager,
                                                           gint                monitor,
                                                           GdkRectangle       *rect);
gboolean dory_desktop_manager_has_good_workarea_info (DoryDesktopManager *manager);

void     dory_desktop_manager_get_margins             (DoryDesktopManager *manager,
                                                       gint                monitor,
                                                       gint               *left,
                                                       gint               *right,
                                                       gint               *top,
                                                       gint               *bottom);

GtkWindow *dory_desktop_manager_get_window_for_monitor  (DoryDesktopManager *manager,
                                                         gint                monitor);
void dory_desktop_manager_get_overlay_info              (DoryDesktopManager *manager,
                                                         gint                monitor,
                                                         GtkActionGroup    **action_group,
                                                         gint               *h_adjust,
                                                         gint               *v_adjust);
void     dory_desktop_manager_show_desktop_overlay    (DoryDesktopManager *manager,
                                                       gint                initial_monitor);
gboolean dory_desktop_manager_get_is_cinnamon         (DoryDesktopManager *manager);

G_END_DECLS

#endif /* _DORY_DESKTOP_MANAGER_H */
