
docConf = configuration_data()
docConf.set('DORY_EXTENSION_VERSION_INFO', DORY_EXTENSION_VERSION_INFO)

configure_file(
  input : 'version.xml.in',
  output: 'version.xml',
  configuration: docConf
)

gnome.gtkdoc('libdory-extension',
  main_xml: 'libdory-extension-docs.xml',
  src_dir: join_paths(meson.project_source_root(), 'libdory-extension'),
  dependencies: dory_extension,
  install: true,
  gobject_typesfile: 'libdory-extension.types',
  install_dir: 'libdory-extension',
  scan_args: [
    '--rebuild-types',
    '--ignore-headers="config.h"' ]
)
