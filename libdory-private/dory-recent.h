

#ifndef __DORY_RECENT_H__
#define __DORY_RECENT_H__

#include <gtk/gtk.h>
#include <libdory-private/dory-file.h>
#include <gio/gio.h>

void dory_recent_add_file (DoryFile *file,
			       GAppInfo *application);

#endif
