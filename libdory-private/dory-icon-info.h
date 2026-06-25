#ifndef DORY_ICON_INFO_H
#define DORY_ICON_INFO_H

#include <glib-object.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk/gdk.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

/* Names for Dory's different zoom levels, from tiniest items to largest items */
typedef enum {
    DORY_ZOOM_LEVEL_NULL = -1,
	DORY_ZOOM_LEVEL_SMALLEST = 0,
	DORY_ZOOM_LEVEL_SMALLER,
	DORY_ZOOM_LEVEL_SMALL,
	DORY_ZOOM_LEVEL_STANDARD,
	DORY_ZOOM_LEVEL_LARGE,
	DORY_ZOOM_LEVEL_LARGER,
	DORY_ZOOM_LEVEL_LARGEST
} DoryZoomLevel;

#define DORY_ZOOM_LEVEL_N_ENTRIES (DORY_ZOOM_LEVEL_LARGEST + 1)

/* Nominal icon sizes for each Dory zoom level.
 * This scheme assumes that icons are designed to
 * fit in a square space, though each image needn't
 * be square. Since individual icons can be stretched,
 * each icon is not constrained to this nominal size.
 */

#define DORY_COMPACT_FORCED_ICON_SIZE 16

#define DORY_LIST_ICON_SIZE_SMALLEST 16
#define DORY_LIST_ICON_SIZE_SMALLER  16
#define DORY_LIST_ICON_SIZE_SMALL    24
#define DORY_LIST_ICON_SIZE_STANDARD 32
#define DORY_LIST_ICON_SIZE_LARGE    48
#define DORY_LIST_ICON_SIZE_LARGER   72
#define DORY_LIST_ICON_SIZE_LARGEST  96

#define DORY_ICON_SIZE_SMALLEST 24
#define DORY_ICON_SIZE_SMALLER  32
#define DORY_ICON_SIZE_SMALL    48
#define DORY_ICON_SIZE_STANDARD 64
#define DORY_ICON_SIZE_LARGE    96
#define DORY_ICON_SIZE_LARGER   128
#define DORY_ICON_SIZE_LARGEST  256

#define DORY_DESKTOP_ICON_SIZE_SMALLER 24
#define DORY_DESKTOP_ICON_SIZE_SMALL 32
#define DORY_DESKTOP_ICON_SIZE_STANDARD 48
#define DORY_DESKTOP_ICON_SIZE_LARGE 64
#define DORY_DESKTOP_ICON_SIZE_LARGER 96

#define DORY_DESKTOP_TEXT_WIDTH_SMALLER 64
#define DORY_DESKTOP_TEXT_WIDTH_SMALL 84
#define DORY_DESKTOP_TEXT_WIDTH_STANDARD 110
#define DORY_DESKTOP_TEXT_WIDTH_LARGE 150
#define DORY_DESKTOP_TEXT_WIDTH_LARGER 200

#define DORY_ICON_TEXT_WIDTH_SMALLEST  0
#define DORY_ICON_TEXT_WIDTH_SMALLER   64
#define DORY_ICON_TEXT_WIDTH_SMALL     84
#define DORY_ICON_TEXT_WIDTH_STANDARD  110
#define DORY_ICON_TEXT_WIDTH_LARGE     96
#define DORY_ICON_TEXT_WIDTH_LARGER    128
#define DORY_ICON_TEXT_WIDTH_LARGEST   256

/* Maximum size of an icon that the icon factory will ever produce */
#define DORY_ICON_MAXIMUM_SIZE     320

typedef struct {
    gint ref_count;
    gboolean sole_owner;
    gint64 last_use_time;
    GdkPixbuf *pixbuf;

    char *icon_name;
    gint orig_scale;
} DoryIconInfo;

DoryIconInfo *    dory_icon_info_ref                          (DoryIconInfo      *icon);
void              dory_icon_info_unref                        (DoryIconInfo      *icon);
void              dory_icon_info_clear                        (DoryIconInfo     **info);
DoryIconInfo *    dory_icon_info_new_for_pixbuf               (GdkPixbuf         *pixbuf,
                                                               int                scale);
DoryIconInfo *    dory_icon_info_lookup                       (GIcon             *icon,
                                                               int                size,
                                                               int                scale);
DoryIconInfo *    dory_icon_info_lookup_from_name             (const char        *name,
                                                               int                size,
                                                               int                scale);
DoryIconInfo *    dory_icon_info_lookup_from_path             (const char        *path,
                                                               int                size,
                                                               int                scale);
gboolean              dory_icon_info_is_fallback                  (DoryIconInfo  *icon);
GdkPixbuf *           dory_icon_info_get_pixbuf                   (DoryIconInfo  *icon);
GdkPixbuf *           dory_icon_info_get_pixbuf_nodefault         (DoryIconInfo  *icon);
GdkPixbuf *           dory_icon_info_get_pixbuf_nodefault_at_size (DoryIconInfo  *icon,
								       gsize              forced_size);
GdkPixbuf *           dory_icon_info_get_pixbuf_at_size           (DoryIconInfo  *icon,
								       gsize              forced_size);
GdkPixbuf *           dory_icon_info_get_desktop_pixbuf_at_size (DoryIconInfo  *icon,
                                                                 gsize          max_height,
                                                                 gsize          max_width);
const char *          dory_icon_info_get_used_name                (DoryIconInfo  *icon);

void                  dory_icon_info_clear_caches                 (void);

/* Relationship between zoom levels and icons sizes. */
guint dory_get_icon_size_for_zoom_level          (DoryZoomLevel  zoom_level);
guint dory_get_icon_text_width_for_zoom_level    (DoryZoomLevel  zoom_level);

guint dory_get_list_icon_size_for_zoom_level     (DoryZoomLevel  zoom_level);

guint dory_get_desktop_icon_size_for_zoom_level  (DoryZoomLevel  zoom_level);
guint dory_get_desktop_text_width_for_zoom_level (DoryZoomLevel  zoom_level);

gint  dory_get_icon_size_for_stock_size          (GtkIconSize        size);
guint dory_icon_get_emblem_size_for_icon_size    (guint              size);

GIcon * dory_user_special_directory_get_gicon (GUserDirectory directory);


G_END_DECLS

#endif /* DORY_ICON_INFO_H */

