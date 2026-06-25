#ifndef _DORY_DESKTOP_OVERLAY_H_
#define _DORY_DESKTOP_OVERLAY_H_

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define DORY_TYPE_DESKTOP_OVERLAY (dory_desktop_overlay_get_type ())

G_DECLARE_FINAL_TYPE (DoryDesktopOverlay, dory_desktop_overlay, DORY, DESKTOP_OVERLAY, GObject)

DoryDesktopOverlay *dory_desktop_overlay_new (void);
void                dory_desktop_overlay_show (DoryDesktopOverlay *overlay,
                                               gint                monitor);
void                dory_desktop_overlay_update_in_place (DoryDesktopOverlay *overlay);
G_END_DECLS

#endif /* _DORY_DESKTOP_OVERLAY_H_ */