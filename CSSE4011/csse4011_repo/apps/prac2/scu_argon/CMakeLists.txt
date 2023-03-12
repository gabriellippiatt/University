# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
set(DTC_OVERLAY_FILE uart.overlay)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(argon)


#Add Include Directories
include_directories(
			../../oslib/scu_drivers/scu_us/
			../../oslib/scu_drivers/scu_uart/
			../../oslib/scu_drivers/scu_ble
			)


#Add Target Sources
target_sources(app PRIVATE 
			src/main.c
			../../oslib/scu_drivers/scu_us/scu_us.c
			../../oslib/scu_drivers/scu_uart/scu_uart.c
			../../oslib/scu_drivers/scu_ble/scu_ble.c
			)