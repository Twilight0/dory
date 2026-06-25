/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* dory-file-undo-operations.h - Manages undo/redo of file operations
 *
 * Copyright (C) 2007-2011 Amos Brocco
 * Copyright (C) 2010 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
 * Boston, MA 02110-1335, USA.
 *
 * Authors: Amos Brocco <amos.brocco@gmail.com>
 *          Cosimo Cecchi <cosimoc@redhat.com>
 *
 */

#ifndef __DORY_FILE_UNDO_OPERATIONS_H__
#define __DORY_FILE_UNDO_OPERATIONS_H__

#include <gio/gio.h>
#include <gtk/gtk.h>

typedef enum {
	DORY_FILE_UNDO_OP_COPY,
	DORY_FILE_UNDO_OP_DUPLICATE,
	DORY_FILE_UNDO_OP_MOVE,
	DORY_FILE_UNDO_OP_RENAME,
	DORY_FILE_UNDO_OP_CREATE_EMPTY_FILE,
	DORY_FILE_UNDO_OP_CREATE_FILE_FROM_TEMPLATE,
	DORY_FILE_UNDO_OP_CREATE_FOLDER,
	DORY_FILE_UNDO_OP_MOVE_TO_TRASH,
	DORY_FILE_UNDO_OP_RESTORE_FROM_TRASH,
	DORY_FILE_UNDO_OP_CREATE_LINK,
	DORY_FILE_UNDO_OP_RECURSIVE_SET_PERMISSIONS,
	DORY_FILE_UNDO_OP_SET_PERMISSIONS,
	DORY_FILE_UNDO_OP_CHANGE_GROUP,
	DORY_FILE_UNDO_OP_CHANGE_OWNER,
	DORY_FILE_UNDO_OP_NUM_TYPES,
} DoryFileUndoOp;

#define DORY_TYPE_FILE_UNDO_INFO         (dory_file_undo_info_get_type ())
#define DORY_FILE_UNDO_INFO(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), DORY_TYPE_FILE_UNDO_INFO, DoryFileUndoInfo))
#define DORY_FILE_UNDO_INFO_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), DORY_TYPE_FILE_UNDO_INFO, DoryFileUndoInfoClass))
#define DORY_IS_FILE_UNDO_INFO(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), DORY_TYPE_FILE_UNDO_INFO))
#define DORY_IS_FILE_UNDO_INFO_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), DORY_TYPE_FILE_UNDO_INFO))
#define DORY_FILE_UNDO_INFO_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), DORY_TYPE_FILE_UNDO_INFO, DoryFileUndoInfoClass))

typedef struct _DoryFileUndoInfo      DoryFileUndoInfo;
typedef struct _DoryFileUndoInfoClass DoryFileUndoInfoClass;
typedef struct _DoryFileUndoInfoDetails DoryFileUndoInfoDetails;

struct _DoryFileUndoInfo {
	GObject parent;
	DoryFileUndoInfoDetails *priv;
};

struct _DoryFileUndoInfoClass {
	GObjectClass parent_class;

	void (* undo_func) (DoryFileUndoInfo *self,
			    GtkWindow            *parent_window);
	void (* redo_func) (DoryFileUndoInfo *self,
			    GtkWindow            *parent_window);

	void (* strings_func) (DoryFileUndoInfo *self,
			       gchar **undo_label,
			       gchar **undo_description,
			       gchar **redo_label,
			       gchar **redo_description);
};

GType dory_file_undo_info_get_type (void) G_GNUC_CONST;

void dory_file_undo_info_apply_async (DoryFileUndoInfo *self,
					  gboolean undo,
					  GtkWindow *parent_window,
					  GAsyncReadyCallback callback,
					  gpointer user_data);
gboolean dory_file_undo_info_apply_finish (DoryFileUndoInfo *self,
					       GAsyncResult *res,
					       gboolean *user_cancel,
					       GError **error);

void dory_file_undo_info_get_strings (DoryFileUndoInfo *self,
					  gchar **undo_label,
					  gchar **undo_description,
					  gchar **redo_label,
					  gchar **redo_description);

/* copy/move/duplicate/link/restore from trash */
#define DORY_TYPE_FILE_UNDO_INFO_EXT         (dory_file_undo_info_ext_get_type ())
#define DORY_FILE_UNDO_INFO_EXT(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), DORY_TYPE_FILE_UNDO_INFO_EXT, DoryFileUndoInfoExt))
#define DORY_FILE_UNDO_INFO_EXT_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), DORY_TYPE_FILE_UNDO_INFO_EXT, DoryFileUndoInfoExtClass))
#define DORY_IS_FILE_UNDO_INFO_EXT(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), DORY_TYPE_FILE_UNDO_INFO_EXT))
#define DORY_IS_FILE_UNDO_INFO_EXT_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), DORY_TYPE_FILE_UNDO_INFO_EXT))
#define DORY_FILE_UNDO_INFO_EXT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), DORY_TYPE_FILE_UNDO_INFO_EXT, DoryFileUndoInfoExtClass))

typedef struct _DoryFileUndoInfoExt      DoryFileUndoInfoExt;
typedef struct _DoryFileUndoInfoExtClass DoryFileUndoInfoExtClass;
typedef struct _DoryFileUndoInfoExtDetails DoryFileUndoInfoExtDetails;

struct _DoryFileUndoInfoExt {
	DoryFileUndoInfo parent;
	DoryFileUndoInfoExtDetails *priv;
};

struct _DoryFileUndoInfoExtClass {
	DoryFileUndoInfoClass parent_class;
};

GType dory_file_undo_info_ext_get_type (void) G_GNUC_CONST;
DoryFileUndoInfo *dory_file_undo_info_ext_new (DoryFileUndoOp op_type,
						       gint item_count,
						       GFile *src_dir,
						       GFile *target_dir);
void dory_file_undo_info_ext_add_origin_target_pair (DoryFileUndoInfoExt *self,
							 GFile                   *origin,
							 GFile                   *target);

/* create new file/folder */
#define DORY_TYPE_FILE_UNDO_INFO_CREATE         (dory_file_undo_info_create_get_type ())
#define DORY_FILE_UNDO_INFO_CREATE(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), DORY_TYPE_FILE_UNDO_INFO_CREATE, DoryFileUndoInfoCreate))
#define DORY_FILE_UNDO_INFO_CREATE_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), DORY_TYPE_FILE_UNDO_INFO_CREATE, DoryFileUndoInfoCreateClass))
#define DORY_IS_FILE_UNDO_INFO_CREATE(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), DORY_TYPE_FILE_UNDO_INFO_CREATE))
#define DORY_IS_FILE_UNDO_INFO_CREATE_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), DORY_TYPE_FILE_UNDO_INFO_CREATE))
#define DORY_FILE_UNDO_INFO_CREATE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), DORY_TYPE_FILE_UNDO_INFO_CREATE, DoryFileUndoInfoCreateClass))

typedef struct _DoryFileUndoInfoCreate      DoryFileUndoInfoCreate;
typedef struct _DoryFileUndoInfoCreateClass DoryFileUndoInfoCreateClass;
typedef struct _DoryFileUndoInfoCreateDetails DoryFileUndoInfoCreateDetails;

struct _DoryFileUndoInfoCreate {
	DoryFileUndoInfo parent;
	DoryFileUndoInfoCreateDetails *priv;
};

struct _DoryFileUndoInfoCreateClass {
	DoryFileUndoInfoClass parent_class;
};

GType dory_file_undo_info_create_get_type (void) G_GNUC_CONST;
DoryFileUndoInfo *dory_file_undo_info_create_new (DoryFileUndoOp op_type);
void dory_file_undo_info_create_set_data (DoryFileUndoInfoCreate *self,
					      GFile                      *file,
					      const char                 *template,
					      gint                        length);

/* rename */
#define DORY_TYPE_FILE_UNDO_INFO_RENAME         (dory_file_undo_info_rename_get_type ())
#define DORY_FILE_UNDO_INFO_RENAME(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), DORY_TYPE_FILE_UNDO_INFO_RENAME, DoryFileUndoInfoRename))
#define DORY_FILE_UNDO_INFO_RENAME_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), DORY_TYPE_FILE_UNDO_INFO_RENAME, DoryFileUndoInfoRenameClass))
#define DORY_IS_FILE_UNDO_INFO_RENAME(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), DORY_TYPE_FILE_UNDO_INFO_RENAME))
#define DORY_IS_FILE_UNDO_INFO_RENAME_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), DORY_TYPE_FILE_UNDO_INFO_RENAME))
#define DORY_FILE_UNDO_INFO_RENAME_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), DORY_TYPE_FILE_UNDO_INFO_RENAME, DoryFileUndoInfoRenameClass))

typedef struct _DoryFileUndoInfoRename      DoryFileUndoInfoRename;
typedef struct _DoryFileUndoInfoRenameClass DoryFileUndoInfoRenameClass;
typedef struct _DoryFileUndoInfoRenameDetails DoryFileUndoInfoRenameDetails;

struct _DoryFileUndoInfoRename {
	DoryFileUndoInfo parent;
	DoryFileUndoInfoRenameDetails *priv;
};

struct _DoryFileUndoInfoRenameClass {
	DoryFileUndoInfoClass parent_class;
};

GType dory_file_undo_info_rename_get_type (void) G_GNUC_CONST;
DoryFileUndoInfo *dory_file_undo_info_rename_new (void);
void dory_file_undo_info_rename_set_data_pre (DoryFileUndoInfoRename *self,
						  GFile                      *old_file,
						  gchar                      *old_display_name,
						  gchar                      *new_display_name);
void dory_file_undo_info_rename_set_data_post (DoryFileUndoInfoRename *self,
						   GFile                      *new_file);

/* trash */
#define DORY_TYPE_FILE_UNDO_INFO_TRASH         (dory_file_undo_info_trash_get_type ())
#define DORY_FILE_UNDO_INFO_TRASH(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), DORY_TYPE_FILE_UNDO_INFO_TRASH, DoryFileUndoInfoTrash))
#define DORY_FILE_UNDO_INFO_TRASH_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), DORY_TYPE_FILE_UNDO_INFO_TRASH, DoryFileUndoInfoTrashClass))
#define DORY_IS_FILE_UNDO_INFO_TRASH(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), DORY_TYPE_FILE_UNDO_INFO_TRASH))
#define DORY_IS_FILE_UNDO_INFO_TRASH_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), DORY_TYPE_FILE_UNDO_INFO_TRASH))
#define DORY_FILE_UNDO_INFO_TRASH_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), DORY_TYPE_FILE_UNDO_INFO_TRASH, DoryFileUndoInfoTrashClass))

typedef struct _DoryFileUndoInfoTrash      DoryFileUndoInfoTrash;
typedef struct _DoryFileUndoInfoTrashClass DoryFileUndoInfoTrashClass;
typedef struct _DoryFileUndoInfoTrashDetails DoryFileUndoInfoTrashDetails;

struct _DoryFileUndoInfoTrash {
	DoryFileUndoInfo parent;
	DoryFileUndoInfoTrashDetails *priv;
};

struct _DoryFileUndoInfoTrashClass {
	DoryFileUndoInfoClass parent_class;
};

GType dory_file_undo_info_trash_get_type (void) G_GNUC_CONST;
DoryFileUndoInfo *dory_file_undo_info_trash_new (gint item_count);
void dory_file_undo_info_trash_add_file (DoryFileUndoInfoTrash *self,
					     GFile                     *file);

/* recursive permissions */
#define DORY_TYPE_FILE_UNDO_INFO_REC_PERMISSIONS         (dory_file_undo_info_rec_permissions_get_type ())
#define DORY_FILE_UNDO_INFO_REC_PERMISSIONS(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), DORY_TYPE_FILE_UNDO_INFO_REC_PERMISSIONS, DoryFileUndoInfoRecPermissions))
#define DORY_FILE_UNDO_INFO_REC_PERMISSIONS_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), DORY_TYPE_FILE_UNDO_INFO_REC_PERMISSIONS, DoryFileUndoInfoRecPermissionsClass))
#define DORY_IS_FILE_UNDO_INFO_REC_PERMISSIONS(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), DORY_TYPE_FILE_UNDO_INFO_REC_PERMISSIONS))
#define DORY_IS_FILE_UNDO_INFO_REC_PERMISSIONS_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), DORY_TYPE_FILE_UNDO_INFO_REC_PERMISSIONS))
#define DORY_FILE_UNDO_INFO_REC_PERMISSIONS_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), DORY_TYPE_FILE_UNDO_INFO_REC_PERMISSIONS, DoryFileUndoInfoRecPermissionsClass))

typedef struct _DoryFileUndoInfoRecPermissions      DoryFileUndoInfoRecPermissions;
typedef struct _DoryFileUndoInfoRecPermissionsClass DoryFileUndoInfoRecPermissionsClass;
typedef struct _DoryFileUndoInfoRecPermissionsDetails DoryFileUndoInfoRecPermissionsDetails;

struct _DoryFileUndoInfoRecPermissions {
	DoryFileUndoInfo parent;
	DoryFileUndoInfoRecPermissionsDetails *priv;
};

struct _DoryFileUndoInfoRecPermissionsClass {
	DoryFileUndoInfoClass parent_class;
};

GType dory_file_undo_info_rec_permissions_get_type (void) G_GNUC_CONST;
DoryFileUndoInfo *dory_file_undo_info_rec_permissions_new (GFile   *dest,
								   guint32 file_permissions,
								   guint32 file_mask,
								   guint32 dir_permissions,
								   guint32 dir_mask);
void dory_file_undo_info_rec_permissions_add_file (DoryFileUndoInfoRecPermissions *self,
						       GFile                              *file,
						       guint32                             permission);

/* single file change permissions */
#define DORY_TYPE_FILE_UNDO_INFO_PERMISSIONS         (dory_file_undo_info_permissions_get_type ())
#define DORY_FILE_UNDO_INFO_PERMISSIONS(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), DORY_TYPE_FILE_UNDO_INFO_PERMISSIONS, DoryFileUndoInfoPermissions))
#define DORY_FILE_UNDO_INFO_PERMISSIONS_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), DORY_TYPE_FILE_UNDO_INFO_PERMISSIONS, DoryFileUndoInfoPermissionsClass))
#define DORY_IS_FILE_UNDO_INFO_PERMISSIONS(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), DORY_TYPE_FILE_UNDO_INFO_PERMISSIONS))
#define DORY_IS_FILE_UNDO_INFO_PERMISSIONS_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), DORY_TYPE_FILE_UNDO_INFO_PERMISSIONS))
#define DORY_FILE_UNDO_INFO_PERMISSIONS_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), DORY_TYPE_FILE_UNDO_INFO_PERMISSIONS, DoryFileUndoInfoPermissionsClass))

typedef struct _DoryFileUndoInfoPermissions      DoryFileUndoInfoPermissions;
typedef struct _DoryFileUndoInfoPermissionsClass DoryFileUndoInfoPermissionsClass;
typedef struct _DoryFileUndoInfoPermissionsDetails DoryFileUndoInfoPermissionsDetails;

struct _DoryFileUndoInfoPermissions {
	DoryFileUndoInfo parent;
	DoryFileUndoInfoPermissionsDetails *priv;
};

struct _DoryFileUndoInfoPermissionsClass {
	DoryFileUndoInfoClass parent_class;
};

GType dory_file_undo_info_permissions_get_type (void) G_GNUC_CONST;
DoryFileUndoInfo *dory_file_undo_info_permissions_new (GFile   *file,
							       guint32  current_permissions,
							       guint32  new_permissions);

/* group and owner change */
#define DORY_TYPE_FILE_UNDO_INFO_OWNERSHIP         (dory_file_undo_info_ownership_get_type ())
#define DORY_FILE_UNDO_INFO_OWNERSHIP(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), DORY_TYPE_FILE_UNDO_INFO_OWNERSHIP, DoryFileUndoInfoOwnership))
#define DORY_FILE_UNDO_INFO_OWNERSHIP_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), DORY_TYPE_FILE_UNDO_INFO_OWNERSHIP, DoryFileUndoInfoOwnershipClass))
#define DORY_IS_FILE_UNDO_INFO_OWNERSHIP(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), DORY_TYPE_FILE_UNDO_INFO_OWNERSHIP))
#define DORY_IS_FILE_UNDO_INFO_OWNERSHIP_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), DORY_TYPE_FILE_UNDO_INFO_OWNERSHIP))
#define DORY_FILE_UNDO_INFO_OWNERSHIP_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), DORY_TYPE_FILE_UNDO_INFO_OWNERSHIP, DoryFileUndoInfoOwnershipClass))

typedef struct _DoryFileUndoInfoOwnership      DoryFileUndoInfoOwnership;
typedef struct _DoryFileUndoInfoOwnershipClass DoryFileUndoInfoOwnershipClass;
typedef struct _DoryFileUndoInfoOwnershipDetails DoryFileUndoInfoOwnershipDetails;

struct _DoryFileUndoInfoOwnership {
	DoryFileUndoInfo parent;
	DoryFileUndoInfoOwnershipDetails *priv;
};

struct _DoryFileUndoInfoOwnershipClass {
	DoryFileUndoInfoClass parent_class;
};

GType dory_file_undo_info_ownership_get_type (void) G_GNUC_CONST;
DoryFileUndoInfo *dory_file_undo_info_ownership_new (DoryFileUndoOp  op_type,
							     GFile              *file,
							     const char         *current_data,
							     const char         *new_data);

#endif /* __DORY_FILE_UNDO_OPERATIONS_H__ */
