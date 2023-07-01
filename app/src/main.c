#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <display/cfb.h>

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

#define DISPLAY_DEVICE DT_NODELABEL(lcd)
#define SELECTED_FONT_INDEX 4
static struct device *dev = DEVICE_DT_GET(DISPLAY_DEVICE);

void display_text() {
	cfb_print(dev, "Hello world", 16, 16);
	cfb_framebuffer_finalize(dev);
}
void display_clear() {
	cfb_framebuffer_clear(dev, false);
}
void display_init() {
	while (!device_is_ready(dev));

	if (dev == NULL) return;
	if (display_set_pixel_format(dev, PIXEL_FORMAT_MONO10) != 0) return;
	if (cfb_framebuffer_init(dev)) return;
	cfb_framebuffer_clear(dev, true);
	display_blanking_off(dev);

	cfb_framebuffer_set_font(dev, SELECTED_FONT_INDEX);
	cfb_framebuffer_invert(dev);
}

#define STACKSIZE 1024
#define PRIORITY 7

int main_thread(void * id) {
	display_init();
	display_text();
	return 0;
}

K_THREAD_DEFINE(main_id, STACKSIZE, main_thread, NULL PRIORITY, 0, K_NO_WAIT);
