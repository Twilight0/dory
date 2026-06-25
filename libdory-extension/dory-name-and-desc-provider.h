/*
 *  dory-name-and-desc-provider.h - Interface for Dory extensions that 
 *  returns the extension's proper name and description for the plugin
 *  manager only - it is not necessary for extension functionality.
 *
 */

#ifndef DORY_NAME_AND_DESC_PROVIDER_H
#define DORY_NAME_AND_DESC_PROVIDER_H

#include <glib-object.h>
#include "dory-extension-types.h"

G_BEGIN_DECLS

#define DORY_TYPE_NAME_AND_DESC_PROVIDER (dory_name_and_desc_provider_get_type ())

G_DECLARE_INTERFACE (DoryNameAndDescProvider, dory_name_and_desc_provider,
                     DORY, NAME_AND_DESC_PROVIDER,
                     GObject)

typedef DoryNameAndDescProviderInterface DoryNameAndDescProviderIface;

struct _DoryNameAndDescProviderInterface {
    GTypeInterface g_iface;

    GList *(*get_name_and_desc) (DoryNameAndDescProvider *provider);
};

/* Interface Functions */
GList *dory_name_and_desc_provider_get_name_and_desc (DoryNameAndDescProvider *provider);

G_END_DECLS

#endif
