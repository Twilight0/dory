/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Suite 500, Boston, MA 02110-1335, USA.
 *
 */

#ifndef __DORY_INTERESTING_FOLDER_BAR_H
#define __DORY_INTERESTING_FOLDER_BAR_H

#include "dory-view.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define DORY_TYPE_INTERESTING_FOLDER_BAR         (dory_interesting_folder_bar_get_type ())
#define DORY_INTERESTING_FOLDER_BAR(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), DORY_TYPE_INTERESTING_FOLDER_BAR, DoryInterestingFolderBar))
#define DORY_INTERESTING_FOLDER_BAR_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), DORY_TYPE_INTERESTING_FOLDER_BAR, DoryInterestingFolderBarClass))
#define DORY_IS_INTERESTING_FOLDER_BAR(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), DORY_TYPE_INTERESTING_FOLDER_BAR))
#define DORY_IS_INTERESTING_FOLDER_BAR_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), DORY_TYPE_INTERESTING_FOLDER_BAR))
#define DORY_INTERESTING_FOLDER_BAR_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), DORY_TYPE_INTERESTING_FOLDER_BAR, DoryInterestingFolderBarClass))

typedef struct DoryInterestingFolderBarPrivate DoryInterestingFolderBarPrivate;

typedef struct
{
	GtkInfoBar parent;

	DoryInterestingFolderBarPrivate *priv;
} DoryInterestingFolderBar;

typedef struct
{
	GtkInfoBarClass parent_class;
} DoryInterestingFolderBarClass;

typedef enum {
    TYPE_NONE_FOLDER = 1,
    TYPE_ACTIONS_FOLDER,
    TYPE_SCRIPTS_FOLDER,
    TYPE_TEMPLATES_FOLDER
} InterestingFolderType;

GType		 dory_interesting_folder_bar_get_type	(void) G_GNUC_CONST;

GtkWidget       *dory_interesting_folder_bar_new         (DoryView *view, InterestingFolderType type);
GtkWidget       *dory_interesting_folder_bar_new_for_location (DoryView *view, GFile *location);

G_END_DECLS

#endif /* __GS_INTERESTING_FOLDER_BAR_H */
