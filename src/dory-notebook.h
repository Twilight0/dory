/*
 *  Copyright © 2002 Christophe Fergeau
 *  Copyright © 2003 Marco Pesenti Gritti
 *  Copyright © 2003, 2004 Christian Persch
 *    (ephy-notebook.c)
 *
 *  Copyright © 2008 Free Software Foundation, Inc.
 *    (dory-notebook.c)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *  $Id: dory-notebook.h 8210 2008-04-11 20:05:25Z chpe $
 */

#ifndef DORY_NOTEBOOK_H
#define DORY_NOTEBOOK_H

#include <glib.h>
#include <gtk/gtk.h>
#include "dory-window-slot.h"

G_BEGIN_DECLS

#define DORY_TYPE_NOTEBOOK		(dory_notebook_get_type ())
#define DORY_NOTEBOOK(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), DORY_TYPE_NOTEBOOK, DoryNotebook))
#define DORY_NOTEBOOK_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), DORY_TYPE_NOTEBOOK, DoryNotebookClass))
#define DORY_IS_NOTEBOOK(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), DORY_TYPE_NOTEBOOK))
#define DORY_IS_NOTEBOOK_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), DORY_TYPE_NOTEBOOK))
#define DORY_NOTEBOOK_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), DORY_TYPE_NOTEBOOK, DoryNotebookClass))

typedef struct _DoryNotebookClass	DoryNotebookClass;
typedef struct _DoryNotebook		DoryNotebook;

struct _DoryNotebook
{
	GtkNotebook parent;
};

struct _DoryNotebookClass
{
        GtkNotebookClass parent_class;

	/* Signals */
	void	 (* tab_close_request)  (DoryNotebook *notebook,
					 DoryWindowSlot *slot);
};

GType		dory_notebook_get_type		(void);

int		dory_notebook_add_tab	(DoryNotebook *nb,
						 DoryWindowSlot *slot,
						 int position,
						 gboolean jump_to);
gint		dory_notebook_find_tab_num_at_pos (DoryNotebook *nb,
						   gint 	 abs_x,
						   gint 	 abs_y);
	
void		dory_notebook_set_show_tabs	(DoryNotebook *nb,
						 gboolean show_tabs);

void		dory_notebook_set_dnd_enabled (DoryNotebook *nb,
						   gboolean enabled);
void		dory_notebook_sync_tab_label (DoryNotebook *nb,
						  DoryWindowSlot *slot);
void		dory_notebook_sync_loading   (DoryNotebook *nb,
						  DoryWindowSlot *slot);

void		dory_notebook_reorder_child_relative (DoryNotebook *notebook,
						      int	    page_num,
						      int 	    offset);
void		dory_notebook_set_current_page_relative (DoryNotebook *notebook,
							     int offset);

gboolean        dory_notebook_can_reorder_child_relative (DoryNotebook *notebook,
							  int	    	page_num,
							  int 	    	offset);
gboolean        dory_notebook_can_set_current_page_relative (DoryNotebook *notebook,
								 int offset);

G_END_DECLS

#endif /* DORY_NOTEBOOK_H */

