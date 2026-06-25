/* dory-pathbar.h
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
 * Boston, MA 02110-1335, USA.
 *
 * 
 */

#ifndef DORY_PATHBAR_H
#define DORY_PATHBAR_H

#include <gtk/gtk.h>
#include <gio/gio.h>

typedef struct _DoryPathBar      DoryPathBar;
typedef struct _DoryPathBarClass DoryPathBarClass;
typedef struct _DoryPathBarDetails DoryPathBarDetails;

#define DORY_TYPE_PATH_BAR                 (dory_path_bar_get_type ())
#define DORY_PATH_BAR(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_PATH_BAR, DoryPathBar))
#define DORY_PATH_BAR_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_PATH_BAR, DoryPathBarClass))
#define DORY_IS_PATH_BAR(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_PATH_BAR))
#define DORY_IS_PATH_BAR_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_PATH_BAR))
#define DORY_PATH_BAR_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_PATH_BAR, DoryPathBarClass))

struct _DoryPathBar
{
	GtkContainer parent;
	
	DoryPathBarDetails *priv;
};

struct _DoryPathBarClass
{
	GtkContainerClass parent_class;

  	void (* path_clicked)   (DoryPathBar  *path_bar,
				 GFile             *location);
  	void (* path_set)       (DoryPathBar  *path_bar,
				 GFile             *location);
};

GType    dory_path_bar_get_type (void) G_GNUC_CONST;

gboolean dory_path_bar_set_path    (DoryPathBar *path_bar, GFile *file);
GFile *  dory_path_bar_get_path_for_button (DoryPathBar *path_bar,
						GtkWidget       *button);
void     dory_path_bar_clear_buttons (DoryPathBar *path_bar);

#endif /* DORY_PATHBAR_H */
