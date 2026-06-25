
dbusBuiltSources = gnome.gdbus_codegen('dory-generated',
  join_paths(meson.project_source_root(), 'data', 'dbus-interfaces.xml'),
  interface_prefix: 'org.Dory.',
  namespace: 'DoryDBus',
  object_manager: true
)

dory_private_sources = [
  dbusBuiltSources,
  'dory-action-manager.c',
  'dory-action.c',
  'dory-bookmark.c',
  'dory-cell-renderer-disk.c',
  'dory-centered-placement-grid.c',
  'dory-clipboard-monitor.c',
  'dory-clipboard.c',
  'dory-column-chooser.c',
  'dory-column-utilities.c',
  'dory-dbus-manager.c',
  'dory-debug.c',
  'dory-default-file-icon.c',
  'dory-desktop-directory-file.c',
  'dory-desktop-directory.c',
  'dory-desktop-icon-file.c',
  'dory-desktop-link-monitor.c',
  'dory-desktop-link.c',
  'dory-desktop-metadata.c',
  'dory-desktop-utils.c',
  'dory-directory-async.c',
  'dory-directory.c',
  'dory-dnd.c',
  'dory-entry.c',
  'fzy-match.c',
  'dory-fzy-utils.c',
  'dory-file-changes-queue.c',
  'dory-file-conflict-dialog.c',
  'dory-file-dnd.c',
  'dory-file-operations.c',
  'dory-file-queue.c',
  'dory-file-undo-manager.c',
  'dory-file-undo-operations.c',
  'dory-file-utilities.c',
  'dory-file.c',
  'dory-global-preferences.c',
  'dory-icon-canvas-item.c',
  'dory-icon-container.c',
  'dory-icon-dnd.c',
  'dory-icon-info.c',
  'dory-job-queue.c',
  'dory-lib-self-check-functions.c',
  'dory-malloc-utils.c',
  'dory-link.c',
  'dory-merged-directory.c',
  'dory-metadata.c',
  'dory-mime-application-chooser.c',
  'dory-module.c',
  'dory-monitor.c',
  'dory-placement-grid.c',
  'dory-places-tree-view.c',
  'dory-program-choosing.c',
  'dory-progress-info-manager.c',
  'dory-progress-info.c',
  'dory-query.c',
  'dory-recent.c',
  'dory-search-directory-file.c',
  'dory-search-directory.c',
  'dory-search-engine-advanced.c',
  'dory-search-engine.c',
  'dory-selection-canvas-item.c',
  'dory-separator-action.c',
  'dory-signaller.c',
  'dory-thumbnails.c',
  'dory-trash-monitor.c',
  'dory-tree-view-drag-dest.c',
  'dory-ui-utilities.c',
  'dory-undo-manager.c',
  'dory-undo-signal-handlers.c',
  'dory-undo-transaction.c',
  'dory-undo.c',
  'dory-vfs-directory.c',
  'dory-vfs-file.c',
  'dory-widget-action.c',
  'dory-widget-menu-item.c',
]

dory_private_deps = [
  cinnamon, eel, gail, gio_unix, glib, gmodule, gtk, json, math, dory_extension, x11, xapp
]

if libexif_enabled
  dory_private_deps += libexif
endif

if tracker_enabled
  dory_private_sources += 'dory-search-engine-tracker.c'
  dory_private_deps += tracker_sparql
endif

if libselinux_enabled
  dory_private_deps += libselinux
endif

if gtk_layer_shell_enabled
  dory_private_deps += gtk_layer_shell
endif

dory_private_lib = static_library('dory-private',
  dory_private_sources,
  dependencies: dory_private_deps,
  include_directories: [ rootInclude, ],
  c_args: dory_definitions,
)

dory_private = declare_dependency(
  include_directories: include_directories('.'),
  link_with: [ dory_private_lib ],
  dependencies: dory_private_deps,
)

install_data('org.dory.gschema.xml',
    install_dir: join_paths(get_option('datadir'), 'glib-2.0', 'schemas')
)
