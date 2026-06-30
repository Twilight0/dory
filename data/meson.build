
# ################################################################################
dataConf = configuration_data()
dataConf.set('VERSION', meson.project_version())

desktopFiles = [
  configure_file(
    input : 'dory-autorun-software.desktop.in',
    output: 'dory-autorun-software.desktop',
    configuration: dataConf,
  ),

  configure_file(
    input : 'dory-autostart.desktop.in',
    output: 'dory-autostart.desktop',
    configuration: dataConf,
  ),

  configure_file(
    input : 'dory.desktop.in',
    output: 'dory.desktop',
    configuration: dataConf,
  )
]

install_data(desktopFiles,
  install_dir: join_paths(get_option('datadir'), 'applications')
)

# ################################################################################
dataConf = configuration_data()
dataConf.set('bindir', join_paths(get_option('prefix'), get_option('bindir')))

dbusFiles = [
  configure_file(
    input : 'org.Dory.FileChooser.service.in',
    output: 'org.Dory.FileChooser.service',
    configuration: dataConf,
  ),
  configure_file(
    input : 'dory.service.in',
    output: 'dory.service',
    configuration: dataConf,
  ),
  configure_file(
    input : 'dory.FileManager1.service.in',
    output: 'dory.FileManager1.service',
    configuration: dataConf,
  ),
]

install_data(dbusFiles,
  install_dir: join_paths(get_option('datadir'), 'dbus-1', 'services')
)

# ################################################################################
# 
mimeFile_translated = custom_target('dory.xml',
  input : ['dory.xml.in'],
  output: 'dory.xml',
  command: ['intltool-merge',
    '--quiet',
    '--xml-style',
    '--utf8',
    po_subdir, '@INPUT@', '@OUTPUT@'
  ],
  install: true,
  install_dir: join_paths(get_option('datadir'), 'mime', 'packages'),
  build_by_default: true,
)

################################################################################
dataConf_root = configuration_data()
dataConf_root.set('bindir', join_paths(get_option('prefix'), get_option('bindir')))

rootPolicyFile = configure_file(
  input : 'org.dory.root.policy.in',
  output: 'org.dory.root.policy',
  configuration: dataConf_root,
)

install_data(rootPolicyFile,
  install_dir: join_paths(get_option('datadir'), 'polkit-1', 'actions')
)

foreach lang_file: ['dory_action.lang', 'dory_search_helper.lang']
  foreach sv_dir: ['gtksourceview-2.0', 'gtksourceview-3.0', 'gtksourceview-4']
    install_data(lang_file,
      install_dir: join_paths(get_option('datadir'), sv_dir, 'language-specs')
    )
  endforeach
endforeach


################################################################################

subdir('icons')
