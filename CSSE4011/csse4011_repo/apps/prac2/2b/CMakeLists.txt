# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(-2)


#Add Include Directories
include_directories(
			../../oslib/scu_drivers/scu_ble/
			)


#Add Target Sources
target_sources(app PRIVATE 
			src/main.c
			../../oslib/scu_drivers/scu_ble/scu_ble.c
			)