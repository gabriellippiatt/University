cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(blinky_tute_ext)

#Add Target Sources
target_sources(app PRIVATE 
			src/main.c
			)