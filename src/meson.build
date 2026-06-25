dbusBuiltSources = gnome.gdbus_codegen(
  'dory-freedesktop-generated',
  join_paths(meson.project_source_root(), 'data', 'freedesktop-dbus-interfaces.xml'),
  interface_prefix: 'org.freedesktop.',
  namespace: 'DoryFreedesktop',
  object_manager: true
)

doryBuiltSources = gnome.gdbus_codegen(
  'dory-cinnamon-dbus',
  join_paths(meson.project_source_root(), 'data', 'org.Cinnamon.xml'),
  interface_prefix: 'org.',
  namespace: 'Dory',
  object_manager: true
)

doryFileChooserBuiltSources = gnome.gdbus_codegen(
  'dory-file-chooser-generated',
  join_paths(meson.project_source_root(), 'data', 'org.Dory.FileChooser.xml'),
  interface_prefix: 'org.Dory.',
  namespace: 'Dory',
  object_manager: false
)

doryCommon_sources = [
  dbusBuiltSources,
  doryBuiltSources,
  doryFileChooserBuiltSources,
  'dory-action-config-widget.c',
  'dory-application.c',
  'dory-blank-desktop-window.c',
  'dory-bookmark-list.c',
  'dory-bookmarks-window.c',
  'dory-config-base-widget.c',
  'dory-connect-server-dialog-nonmain.c',
  'dory-connect-server-dialog.c',
  'dory-connect-server-operation.c',
  'dory-desktop-icon-grid-view.c',
  'dory-desktop-item-properties.c',
  'dory-desktop-manager.c',
  'dory-desktop-overlay.c',
  'dory-desktop-window.c',
  'dory-error-reporting.c',
  'dory-extension-config-widget.c',
  'dory-file-management-properties.c',
  'dory-filter-bar.c',
  'dory-floating-bar.c',
  'dory-file-chooser-dbus.c',
  'dory-file-chooser-dialog.c',
  'dory-freedesktop-dbus.c',
  'dory-icon-view-container.c',
  'dory-icon-view-grid-container.c',
  'dory-icon-view.c',
  'dory-image-properties-page.c',
  'dory-interesting-folder-bar.c',
  'dory-list-model.c',
  'dory-list-view.c',
  'dory-location-bar.c',
  'dory-location-entry.c',
  'dory-mime-actions.c',
  'dory-navigation-action.c',
  'dory-navigation-state.c',
  'dory-notebook.c',
  'dory-pathbar.c',
  'dory-places-sidebar.c',
  'dory-plugin-manager.c',
  'dory-previewer.c',
  'dory-progress-info-widget.c',
  'dory-progress-ui-handler.c',
  'dory-properties-window.c',
  'dory-query-editor.c',
  'dory-template-config-widget.c',
  'dory-self-check-functions.c',
  'dory-statusbar.c',
  'dory-thumbnail-problem-bar.c',
  'dory-toolbar.c',
  'dory-trash-bar.c',
  'dory-tree-sidebar-model.c',
  'dory-tree-sidebar.c',
  'dory-view-dnd.c',
  'dory-view-factory.c',
  'dory-view.c',
  'dory-window-bookmarks.c',
  'dory-window-manage-views.c',
  'dory-window-menus.c',
  'dory-window-pane.c',
  'dory-window-slot-dnd.c',
  'dory-window-slot.c',
  'dory-window.c',
  'dory-x-content-bar.c',
]

doryWindow_sources = [
  'dory-main-application.c',
  'dory-main.c',
]
doryWindow_headers = [
  'dory-main-application.h',
]

doryDesktop_sources = [
  'dory-desktop-application.c',
  'dory-desktop-icon-view.c',
  'dory-desktop-main.c',
]
doryDesktop_headers = [
  'dory-desktop-application.h',
  'dory-desktop-overlay.h',
  'dory-desktop-icon-view.h',
]


if enableEmptyView
  doryCommon_sources += 'dory-empty-view.c'
endif

dory_deps = [ cinnamon, gail, glib, gtk, math,
  egg, dory_extension, dory_private, xapp ]

if exempi_enabled
  dory_deps += exempi
endif

if libexif_enabled
  dory_deps += libexif
endif

if gtk_layer_shell_enabled
  dory_deps += gtk_layer_shell
  dory_deps += wayland_client
endif

dory = executable('dory',
  doryCommon_sources + doryWindow_sources,
  include_directories: [ rootInclude ],
  c_args: dory_definitions,
  dependencies: dory_deps,
  install: true
)

# doryDesktop = executable('dory-desktop',
#   doryCommon_sources + doryDesktop_sources,
#   include_directories: [ rootInclude],
#   c_args: dory_definitions,
#   dependencies: dory_deps,
#   install: true
# )
# 
# dory_autorun_software = executable('dory-autorun-software',
#   [ 'dory-autorun-software.c' ],
#   include_directories: [ rootInclude, ],
#   c_args: dory_definitions,
#   dependencies: dory_deps,
#   install: true
# )
# 
# dory_connect_server = executable('dory-connect-server',
#   [
#     'dory-bookmark-list.c',
#     'dory-connect-server-dialog.c',
#     'dory-connect-server-dialog-main.c',
#     'dory-connect-server-operation.c'
#   ],
#   include_directories: [ rootInclude, ],
#   c_args: dory_definitions,
#   dependencies: dory_deps,
#   install: true
# )
# 
# dory_open_with = executable('dory-open-with',
#   [ 'dory-open-with-main.c' ],
#   include_directories: [ rootInclude, ],
#   c_args: dory_definitions,
#   dependencies: dory_deps,
#   install: true
# )
# 
# # LibExecs
# dory_extensions_list = executable('dory-extensions-list',
#   [ 'dory-extensions-list.c' ],
#   include_directories: [ rootInclude, ],
#   c_args: dory_definitions,
#   dependencies: [ dory_deps ],
#   install: true,
#   install_dir: libExecPath,
# )
