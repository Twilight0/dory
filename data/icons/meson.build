publicIcons = [
  'hicolor/apps/16x16/dory.png',
  'hicolor/apps/22x22/dory.png',
  'hicolor/apps/24x24/dory.png',
  'hicolor/apps/32x32/dory.png',
  'hicolor/apps/scalable/dory.png',
  'hicolor/actions/16x16/dory-eject.png',
  'hicolor/actions/16x16/menu-bullet.png',
  'hicolor/actions/16x16/menu-sort-up.png',
  'hicolor/actions/16x16/menu-sort-up-free.png',
  'hicolor/actions/16x16/menu-sort-down.png',
  'hicolor/actions/16x16/menu-sort-down-free.png',
  'hicolor/actions/16x16/menu-sort-right.png',
  'hicolor/actions/16x16/menu-sort-right-free.png',
  'hicolor/actions/16x16/menu-sort-left.png',
  'hicolor/actions/16x16/menu-sort-left-free.png',
  'hicolor/actions/16x16/menu-none.png',
  'hicolor/actions/32x32/dory-eject.png',
  'hicolor/actions/scalable/dory-location-symbolic.svg',
  'hicolor/actions/scalable/dory-location-symbolic-rtl.svg',
  'hicolor/actions/scalable/dory-sidebar-hide-symbolic.svg',
  'hicolor/actions/scalable/dory-sidebar-hide-symbolic-rtl.svg',
  'hicolor/actions/scalable/dory-sidebar-show-symbolic.svg',
  'hicolor/actions/scalable/dory-sidebar-show-symbolic-rtl.svg',
  'hicolor/actions/scalable/dory-sidebar-tree-symbolic.svg',
  'hicolor/actions/scalable/dory-sidebar-tree-symbolic-rtl.svg',
  'hicolor/actions/scalable/dory-sidebar-places-symbolic.svg',
  'hicolor/actions/scalable/dory-auto-arrange-symbolic.svg',
  'hicolor/actions/scalable/dory-desktop-scale-symbolic.svg',
  'hicolor/actions/scalable/dory-horizontal-layout-symbolic.svg',
  'hicolor/actions/scalable/dory-horizontal-layout-wide-symbolic.svg',
  'hicolor/actions/scalable/dory-vertical-layout-symbolic.svg',
  'hicolor/actions/scalable/dory-vertical-layout-wide-symbolic.svg',
  'hicolor/actions/scalable/dory-recursive-search-symbolic.svg',
  'hicolor/actions/scalable/dory-recursive-search-symbolic-rtl.svg',
  'hicolor/status/scalable/dory-progress-0-symbolic.svg',
  'hicolor/status/scalable/dory-progress-10-symbolic.svg',
  'hicolor/status/scalable/dory-progress-20-symbolic.svg',
  'hicolor/status/scalable/dory-progress-30-symbolic.svg',
  'hicolor/status/scalable/dory-progress-40-symbolic.svg',
  'hicolor/status/scalable/dory-progress-50-symbolic.svg',
  'hicolor/status/scalable/dory-progress-60-symbolic.svg',
  'hicolor/status/scalable/dory-progress-70-symbolic.svg',
  'hicolor/status/scalable/dory-progress-80-symbolic.svg',
  'hicolor/status/scalable/dory-progress-90-symbolic.svg',
  'hicolor/status/scalable/dory-progress-100-symbolic.svg',
]
privateIcons = [
  'hicolor/emblems/16x16/emblem-note.png',
  'hicolor/emblems/24x24/emblem-note.png',
  'hicolor/emblems/48x48/emblem-note.png',
]
noInstallIcons = []

foreach icon : publicIcons
  nameParts = icon.split('/')
  theme   = nameParts[0]
  context = nameParts[1]
  size    = nameParts[2]
  file    = nameParts[3]
  install_data(icon,
    install_dir: join_paths(get_option('datadir'), 'icons', theme, size, context)
  )
endforeach


foreach icon : privateIcons
  nameParts = icon.split('/')
  theme   = nameParts[0]
  context = nameParts[1]
  size    = nameParts[2]
  file    = nameParts[3]
  install_data(icon,
    install_dir: join_paths(get_option('datadir'), 'dory', 'icons', theme, size, context)
  )
endforeach

