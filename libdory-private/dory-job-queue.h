/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
   dory-job-queue.h - file operation queue

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public
   License along with this program; if not, write to the
   Free Software Foundation, Inc., 51 Franklin Street - Suite 500,
   Boston, MA 02110-1335, USA.
*/

#ifndef __DORY_JOB_QUEUE_H__
#define __DORY_JOB_QUEUE_H__

#include <glib-object.h>

#include <libdory-private/dory-progress-info.h>

#define DORY_TYPE_JOB_QUEUE dory_job_queue_get_type()
#define DORY_JOB_QUEUE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), DORY_TYPE_JOB_QUEUE, DoryJobQueue))
#define DORY_JOB_QUEUE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), DORY_TYPE_JOB_QUEUE, DoryJobQueueClass))
#define DORY_IS_JOB_QUEUE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DORY_TYPE_JOB_QUEUE))
#define DORY_IS_JOB_QUEUE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), DORY_TYPE_JOB_QUEUE))
#define DORY_JOB_QUEUE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), DORY_TYPE_JOB_QUEUE, DoryJobQueueClass))

typedef struct _DoryJobQueue DoryJobQueue;
typedef struct _DoryJobQueueClass DoryJobQueueClass;
typedef struct _DoryJobQueuePriv DoryJobQueuePriv;

struct _DoryJobQueue {
  GObject parent;

  /* private */
  DoryJobQueuePriv *priv;
};

struct _DoryJobQueueClass {
  GObjectClass parent_class;
};

GType dory_job_queue_get_type (void);

DoryJobQueue *dory_job_queue_get (void);

void dory_job_queue_add_new_job (DoryJobQueue *self,
                                 GIOSchedulerJobFunc job_func,
                                 gpointer user_data,
                                 GCancellable *cancellable,
                                 DoryProgressInfo *info,
                                 gboolean start_immediately);

void dory_job_queue_start_next_job (DoryJobQueue *self);

void dory_job_queue_start_job_by_info (DoryJobQueue     *self,
                                       DoryProgressInfo *info);

GList *dory_job_queue_get_all_jobs (DoryJobQueue *self);

G_END_DECLS

#endif /* __DORY_JOB_QUEUE_H__ */
