/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   dory-progress-info.h: file operation progress info.
 
   Copyright (C) 2007 Red Hat, Inc.
  
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
  
   Author: Alexander Larsson <alexl@redhat.com>
*/

#ifndef DORY_PROGRESS_INFO_H
#define DORY_PROGRESS_INFO_H

#include <glib-object.h>
#include <gio/gio.h>

#define DORY_TYPE_PROGRESS_INFO         (dory_progress_info_get_type ())
#define DORY_PROGRESS_INFO(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), DORY_TYPE_PROGRESS_INFO, DoryProgressInfo))
#define DORY_PROGRESS_INFO_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), DORY_TYPE_PROGRESS_INFO, DoryProgressInfoClass))
#define DORY_IS_PROGRESS_INFO(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), DORY_TYPE_PROGRESS_INFO))
#define DORY_IS_PROGRESS_INFO_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), DORY_TYPE_PROGRESS_INFO))
#define DORY_PROGRESS_INFO_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), DORY_TYPE_PROGRESS_INFO, DoryProgressInfoClass))

typedef struct _DoryProgressInfo      DoryProgressInfo;
typedef struct _DoryProgressInfoClass DoryProgressInfoClass;

GType dory_progress_info_get_type (void) G_GNUC_CONST;

/* Signals:
   "changed" - status or details changed
   "progress-changed" - the percentage progress changed (or we pulsed if in activity_mode
   "started" - emited on job start
   "finished" - emitted when job is done
   
   All signals are emitted from idles in main loop.
   All methods are threadsafe.
 */

DoryProgressInfo *dory_progress_info_new (void);

GList *       dory_get_all_progress_info (void);

char *        dory_progress_info_get_status      (DoryProgressInfo *info);
char *        dory_progress_info_get_details     (DoryProgressInfo *info);
char *        dory_progress_info_get_initial_details (DoryProgressInfo *info);
double        dory_progress_info_get_progress    (DoryProgressInfo *info);
GCancellable *dory_progress_info_get_cancellable (DoryProgressInfo *info);
void          dory_progress_info_cancel          (DoryProgressInfo *info);
gboolean      dory_progress_info_get_is_started  (DoryProgressInfo *info);
gboolean      dory_progress_info_get_is_finished (DoryProgressInfo *info);
gboolean      dory_progress_info_get_is_paused   (DoryProgressInfo *info);

void          dory_progress_info_queue           (DoryProgressInfo *info);
void          dory_progress_info_start           (DoryProgressInfo *info);
void          dory_progress_info_finish          (DoryProgressInfo *info);
void          dory_progress_info_pause           (DoryProgressInfo *info);
void          dory_progress_info_resume          (DoryProgressInfo *info);
void          dory_progress_info_set_status      (DoryProgressInfo *info,
						      const char           *status);
void          dory_progress_info_take_status     (DoryProgressInfo *info,
						      char                 *status);
void          dory_progress_info_set_details     (DoryProgressInfo *info,
						      const char           *details);
void          dory_progress_info_take_initial_details (DoryProgressInfo *info,
                              char                 *initial_details);
void          dory_progress_info_take_details    (DoryProgressInfo *info,
						      char                 *details);
void          dory_progress_info_set_progress    (DoryProgressInfo *info,
						      double                current,
						      double                total);
void          dory_progress_info_pulse_progress  (DoryProgressInfo *info);

gdouble       dory_progress_info_get_elapsed_time (DoryProgressInfo *info);


#endif /* DORY_PROGRESS_INFO_H */
