project('gnome-battery-charge-manager', 'c',
          version: '0.1.0',
    meson_version: '>= 0.59.0',
  default_options: [ 'warning_level=2',
                     'werror=false',
                     'c_std=gnu11',
                   ],
)

# Add schema
schemas_dir = get_option('prefix') / get_option('datadir') / 'glib-2.0' / 'schemas'
settings_schemas = [ 'data/org.gnome.BatteryChargeManager.gschema.xml' ]
install_data(settings_schemas, install_dir: schemas_dir)

# Add services
user_services_dir = '/etc/systemd/user/'
user_services = [ 'services/gnome-battery-charge-manager.service' ]
install_data(user_services, install_dir: user_services_dir)

system_services_dir = '/etc/systemd/system/'
system_services = [ 'services/udev-power-supply-trigger.service' ]
install_data(system_services, install_dir: system_services_dir)

config_h = configuration_data()
config_h.set_quoted('PACKAGE_VERSION', meson.project_version())
configure_file(
  output: 'gnome_battery_charge_manager-config.h',
  configuration: config_h,
)
add_project_arguments([
  '-I' + meson.project_build_root(),
], language: 'c')

subdir('src')

# Install post-scripts (to add install schema)
meson.add_install_script('glib-compile-schemas', schemas_dir)
meson.add_install_script('services/enable.sh')