/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Dory
 *
 * Copyright (C) 2011 Red Hat, Inc.
 *
 * Dory is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Dory is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
 * Boston, MA 02110-1335, USA.
 *
 * Author: Cosimo Cecchi <cosimoc@redhat.com>
 */

#ifndef __DORY_PROGRESS_INFO_MANAGER_H__
#define __DORY_PROGRESS_INFO_MANAGER_H__

#include <glib-object.h>

#include <libdory-private/dory-progress-info.h>

#define DORY_TYPE_PROGRESS_INFO_MANAGER dory_progress_info_manager_get_type()
#define DORY_PROGRESS_INFO_MANAGER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_PROGRESS_INFO_MANAGER, DoryProgressInfoManager))
#define DORY_PROGRESS_INFO_MANAGER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_PROGRESS_INFO_MANAGER, DoryProgressInfoManagerClass))
#define DORY_IS_PROGRESS_INFO_MANAGER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_PROGRESS_INFO_MANAGER))
#define DORY_IS_PROGRESS_INFO_MANAGER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_PROGRESS_INFO_MANAGER))
#define DORY_PROGRESS_INFO_MANAGER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_PROGRESS_INFO_MANAGER, DoryProgressInfoManagerClass))

typedef struct _DoryProgressInfoManager DoryProgressInfoManager;
typedef struct _DoryProgressInfoManagerClass DoryProgressInfoManagerClass;
typedef struct _DoryProgressInfoManagerPriv DoryProgressInfoManagerPriv;

struct _DoryProgressInfoManager {
  GObject parent;

  /* private */
  DoryProgressInfoManagerPriv *priv;
};

struct _DoryProgressInfoManagerClass {
  GObjectClass parent_class;
};

GType dory_progress_info_manager_get_type (void);

DoryProgressInfoManager* dory_progress_info_manager_new (void);

void dory_progress_info_manager_add_new_info (DoryProgressInfoManager *self,
                                                  DoryProgressInfo *info);
GList *dory_progress_info_manager_get_all_infos (DoryProgressInfoManager *self);

G_END_DECLS

#endif /* __DORY_PROGRESS_INFO_MANAGER_H__ */
