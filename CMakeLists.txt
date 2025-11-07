cmake_minimum_required(VERSION 3.13)

set(target "whale_eeprom_module")
project(${target} C CXX ASM)

add_subdirectory(./libs/rp2x_cat24m01)

add_library(${target} INTERFACE)

target_sources(${target} INTERFACE
	submodules/whale_eeprom.c
)

target_include_directories(${target} INTERFACE
	..
)

target_link_libraries(${target} INTERFACE
	rp2x_cat24m01_lib
)

target_compile_definitions(${target} INTERFACE
	W_RTC_I2C=i2c0
	W_RTC_SDA=4
	W_RTC_SCL=5
)

