#ifndef _DORY_DESKTOP_PREFERENCES_H_
#define _DORY_DESKTOP_PREFERENCES_H_

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define DORY_TYPE_DESKTOP_PREFERENCES (dory_desktop_preferences_get_type ())

G_DECLARE_FINAL_TYPE (DoryDesktopPreferences, dory_desktop_preferences, DORY, DESKTOP_PREFERENCES, GtkBin)

DoryDesktopPreferences *dory_desktop_preferences_new (void);

G_END_DECLS

#endif /* _DORY_DESKTOP_PREFERENCES_H_ */