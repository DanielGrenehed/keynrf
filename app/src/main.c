#include <stdio.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/display/cfb.h>
#include <zephyr/logging/log.h>

/**
 *	nRF52840-Dongle:
 *		SW1 Button	- GIPO 1.06
 *		SW2 Reset	- GPIO 0.18
 *		LD1 Green	- GPIO 0.06
 *		LD2 Red		- GPIO 0.08
 *		LD2 Green	- GPIO 1.09
 *		LD2 Blue	- GPIO 0.12
 *
 * 	Perfboard:
 *
 *	Button matrix:
 *		Row A 		- GPIO 0.02
 *		Row B 		- GPIO 1.15
 *		Row C 		- GPIO 1.13
 *		Row D 		- GPIO 1.10
 *
 *		Col 1 		- GPIO 0.24
 *		Col 2 		- GPIO 1.00
 *		Col 3 		- GPIO 0.09
 *		Col 4 		- GPIO 0.10
 *
 *	Rotary encoder:
 *		A 		- GPIO 0.13
 *		B 		- GPIO 0.15
 *
 *	Display I2C:
 *		SCK 		- GPIO 0.31
 *		SDA 		- GPIO 0.29
 *
 *	UART (midi):
 *		TX 		- GPIO 0.17
 *		RX 		- GPIO 0.20
 *	
  * */
LOG_MODULE_REGISTER(display);
static const struct device *display = DEVICE_DT_GET(DT_NODELABEL(ssd1306));



void main(void) {
	if (display == NULL) {
		LOG_ERR("device pointer is NULL");
		return;
	}
	if (!device_is_ready(display)) {
		LOG_ERR("display device is not ready");
		return;
	}

	int ret;
	ret = cfb_framebuffer_init(display);
	if (ret != 0) {
		LOG_ERR("could not initialize display");
		return;
	}
	ret = cfb_print(display, "Hello, world!", 0, 0);
	if (ret != 0) {
		LOG_ERR("could not print to display!");
		return;
	}
	ret = cfb_framebuffer_finalize(display);
	if (ret != 0) {
		LOG_ERR("could not finalize to display");
		return;
	}
}
