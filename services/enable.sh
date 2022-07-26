#!/bin/bash

systemctl --user enable gnome-battery-charge-manager.service
systemctl enable udev-power-supply-trigger.service
