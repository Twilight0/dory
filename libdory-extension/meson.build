
dory_i18n_header = [
  'dory-extension-i18n.h'
]

dory_extension_sources = [
  gresources,
  'dory-column-provider.c',
  'dory-column.c',
  'dory-desktop-preferences.c',
  'dory-extension-private.h',
  'dory-file-info.c',
  'dory-info-provider.c',
  'dory-location-widget-provider.c',
  'dory-menu-item.c',
  'dory-menu-provider.c',
  'dory-name-and-desc-provider.c',
  'dory-property-page-provider.c',
  'dory-property-page.c',
  'dory-menu.c',
  'dory-simple-button.c',
]

dory_extension_headers = [
  'dory-column-provider.h',
  'dory-column.h',
  'dory-desktop-preferences.h',
  'dory-extension-types.h',
  'dory-file-info.h',
  'dory-info-provider.h',
  'dory-location-widget-provider.h',
  'dory-menu-item.h',
  'dory-menu-provider.h',
  'dory-name-and-desc-provider.h',
  'dory-property-page-provider.h',
  'dory-property-page.h',
  'dory-menu.h',
  'dory-simple-button.h',
]

dory_extension_incdir = include_directories('.')

dory_extension_deps = [ glib, gtk ]

dory_extension_lib = shared_library('dory-extension',
  dory_extension_sources + dory_i18n_header,
  dependencies: dory_extension_deps,
  include_directories: [ rootInclude, ],
  install: true,
  version: '1.4.0',
  link_args: [ '-Wl,-no-undefined' ],
)

install_headers(dory_extension_headers,
  subdir: join_paths('dory', 'libdory-extension')
)

dory_extension = declare_dependency(
  include_directories: include_directories('.'),
  link_with: [ dory_extension_lib ],
  dependencies: dory_extension_deps,
)

typelibdir = go_intr.get_variable(pkgconfig: 'typelibdir',
  pkgconfig_define: ['libdir', join_paths(get_option('prefix'), get_option('libdir'))]
)

gnome.generate_gir(dory_extension_lib,
  sources: dory_extension_sources + dory_extension_headers,
  nsversion: '3.0',
  namespace: 'Dory',
  export_packages: 'libdory-extension',
  includes: [ 'Gtk-3.0', 'Gio-2.0', 'GLib-2.0', ],
  include_directories: [ rootInclude, ],
  install_dir_typelib: typelibdir,
  install: true,
)

pkgconfig.generate(filebase: 'libdory-extension',
  name: 'libdory-extension',
  description: 'A library to create Dory view extensions',
  version: meson.project_version(),
  requires: [ 'gio-2.0', 'glib-2.0', 'gtk+-3.0' ],
  libraries: dory_extension_lib,
  subdirs: 'dory',
  variables: 'extensiondir=${libdir}/@0@/@1@'.format('dory', 'extensions-3.0'),
)

