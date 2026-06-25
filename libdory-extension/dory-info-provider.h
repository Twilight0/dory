/*
 *  dory-info-provider.h - Interface for Dory extensions that 
 *                             provide info about files.
 *
 *  Copyright (C) 2003 Novell, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin Street, Suite 500, MA 02110-1335, USA.
 * 
 *  Author:  Dave Camp <dave@ximian.com>
 *
 */

/* This interface is implemented by Dory extensions that want to 
 * provide information about files.  Extensions are called when Dory 
 * needs information about a file.  They are passed a DoryFileInfo 
 * object which should be filled with relevant information */

#ifndef DORY_INFO_PROVIDER_H
#define DORY_INFO_PROVIDER_H

#include <glib-object.h>
#include "dory-extension-types.h"
#include "dory-file-info.h"

G_BEGIN_DECLS

#define DORY_TYPE_INFO_PROVIDER           (dory_info_provider_get_type ())

G_DECLARE_INTERFACE (DoryInfoProvider, dory_info_provider,
                     DORY, INFO_PROVIDER,
                     GObject)

typedef DoryInfoProviderInterface DoryInfoProviderIface;

typedef void (*DoryInfoProviderUpdateComplete) (DoryInfoProvider    *provider,
						    DoryOperationHandle *handle,
						    DoryOperationResult  result,
						    gpointer                 user_data);

struct _DoryInfoProviderInterface {
	GTypeInterface g_iface;

	DoryOperationResult (*update_file_info) (DoryInfoProvider     *provider,
						     DoryFileInfo         *file,
						     GClosure                 *update_complete,
						     DoryOperationHandle **handle);
	void                    (*cancel_update)    (DoryInfoProvider     *provider,
						     DoryOperationHandle  *handle);
};

/* pre-G_DECLARE_INTERFACE/G_DEFINE_INTERFACE compatibility */
#define DoryInfoProviderIface DoryInfoProviderInterface

/* Interface Functions */
DoryOperationResult dory_info_provider_update_file_info       (DoryInfoProvider     *provider,
								       DoryFileInfo         *file,
								       GClosure                 *update_complete,
								       DoryOperationHandle **handle);
void                    dory_info_provider_cancel_update          (DoryInfoProvider     *provider,
								       DoryOperationHandle  *handle);

/* Helper functions for implementations */
void                    dory_info_provider_update_complete_invoke (GClosure                 *update_complete,
								       DoryInfoProvider     *provider,
								       DoryOperationHandle  *handle,
								       DoryOperationResult   result);

G_END_DECLS

#endif
