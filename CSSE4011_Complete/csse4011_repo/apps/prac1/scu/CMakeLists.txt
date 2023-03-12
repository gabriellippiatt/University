# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
set(DTC_OVERLAY_FILE speaker.overlay)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(scu)


#Add Include Directories
include_directories(
			../../oslib/scu_drivers/scu_sensors/
			../../oslib/scu_drivers/scu_ble/
			../../oslib/scu_drivers/scu_button/
			../../oslib/scu_drivers/scu_led/
			../../oslib/scu_drivers/scu_speaker/
			)


#Add Target Sources
target_sources(app PRIVATE 
			src/main.c
			../../oslib/scu_drivers/scu_sensors/scu_sensors.c
			../../oslib/scu_drivers/scu_ble/scu_ble.c
			../../oslib/scu_drivers/scu_button/scu_button.c
			../../oslib/scu_drivers/scu_led/scu_led.c
			../../oslib/scu_drivers/scu_speaker/scu_speaker.c
			)