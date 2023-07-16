cmake_minimum_required(VERSION 3.13.1)
set(BOARD nrf52840dongle_nrf52840)
set(DTC_OVERLAY_FILE keynrf.overlay)
find_package(Zephyr)
project(keynrf)

include_directories(src)
target_sources(app PRIVATE src/main.c src/keys.c src/leds.c)

set_property(GLOBAL APPEND PROPERTY HEX_FILES_TO_MERGE
	${app_bootloader_hex}
	${PROJECT_BINARY_DIR}/${KERNEL_HEX_NAME}
	${app_provision_hex})
