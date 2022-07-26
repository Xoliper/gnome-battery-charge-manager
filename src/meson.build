# Source files
gnome_battery_charge_manager_sources = [
  'main.c',
]

# Dependences
gnome_battery_charge_manager_deps = [
  dependency('gio-2.0', version: '>= 2.72.3'),
  dependency('libudev', version: '>= 251'),
]

# Executable build
executable('gnome-battery-charge-manager', gnome_battery_charge_manager_sources,
  dependencies: gnome_battery_charge_manager_deps,
  install: true,
  install_dir : '/bin/'
)