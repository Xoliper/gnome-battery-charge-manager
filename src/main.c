/* main.c
 *
 * Copyright 2022 Xoliper
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE

#include <libudev.h>
#include <gio/gio.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <stdlib.h>

gboolean get_bat_charge_threshold_switch_state(GSettings* settings)
{
  gboolean switch_state;
  GVariant * bcte_variant = g_settings_get_value(settings, "battery-charge-threshold-enabled");
  g_variant_get(bcte_variant, "b", &switch_state);
  return switch_state;
}

int
main ()
{

  //---------------------------------------------
  //Setup thresholds
  //---------------------------------------------

  const char off_value[4] = {'1', '0', '0', '\0'};
  const char on_value[3] = {'8', '0', '\0'};

  //---------------------------------------------
  //Get gsettings value
  //---------------------------------------------

  //GSettings * settings = g_settings_new("org.gnome.settings-daemon.plugins.power");
  GSettings * settings = g_settings_new("org.gnome.BatteryChargeManager");
  if(settings == NULL){
    printf("Cannot open org.gnome.BatteryChargeManager scheme.\n");
    exit(-1);
  }

  //---------------------------------------------
  //Get access to sysfs charge_control_end_threshold value
  //---------------------------------------------

  struct udev *udev;
  struct udev_enumerate *enumerate;
  struct udev_list_entry *devices, *dev_list_entry;
  struct udev_device *dev;

  udev = udev_new();
  if (!udev) {
    printf("Cannot open udev handler.\n");
    exit(-2);
  }

  //Create a list of the devices in the 'power_supply' subsystem.
  enumerate = udev_enumerate_new(udev);
  udev_enumerate_add_match_subsystem(enumerate, "power_supply");
  udev_enumerate_scan_devices(enumerate);
  devices = udev_enumerate_get_list_entry(enumerate);

  //---------------------------------------------
  // Main loop
  //---------------------------------------------

  gboolean current_switch_state = FALSE;
  while(1){

    current_switch_state = get_bat_charge_threshold_switch_state(settings);

    //Go through all supported devices
    udev_list_entry_foreach(dev_list_entry, devices) {

      const char* path = udev_list_entry_get_name(dev_list_entry);
      dev = udev_device_new_from_syspath(udev, path);

      //Check if supported this device supports this attribute by reading it.
      const char* ccet = udev_device_get_sysattr_value(dev, "charge_control_end_threshold");
      if(ccet != NULL){
        short value = atoi(ccet);
        //udev_device_set_sysattr_value needs evelated privileges.
        if(current_switch_state == TRUE && value != 80){
          udev_device_set_sysattr_value(dev, "charge_control_end_threshold", &on_value[0]);
        } else if(current_switch_state == FALSE && value != 100){
          udev_device_set_sysattr_value(dev, "charge_control_end_threshold", &off_value[0]);
        }
      }
    }
    sleep(5);
  }

  // Free the enumerator object
  udev_enumerate_unref(enumerate);
  udev_unref(udev);

  return 0;

}
