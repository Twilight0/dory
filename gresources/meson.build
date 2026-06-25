gresources = gnome.compile_resources(
  'dory-resources', 'dory.gresource.xml',
  source_dir: '.',
  c_name: 'dory',
  install_header: false,
  install: false
)

layout_editor_gresources = gnome.compile_resources(
  'dory-action-layout-editor-resources', 'dory-action-layout-editor.gresource.xml',
  source_dir: '.',
  gresource_bundle: true,
  install: true,
  install_dir: doryDataPath
)
