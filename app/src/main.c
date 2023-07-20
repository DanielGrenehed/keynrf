#include <stdint.h>
#include <keys.h>
#include <leds.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/display/cfb.h>
#include <zephyr/logging/log.h>


static const struct gpio_dt_spec rota = GPIO_DT_SPEC_GET(DT_NODELABEL(rota), gpios);
static const struct gpio_dt_spec rotb = GPIO_DT_SPEC_GET(DT_NODELABEL(rotb), gpios);

LOG_MODULE_REGISTER(display);
static const struct device *display = DEVICE_DT_GET(DT_NODELABEL(ssd1306));
static const struct device *i2c = DEVICE_DT_GET(DT_NODELABEL(i2c0));

void scan_i2c() {
	for (int i = 0; i < 127; i++) LOG_INF("i2c: %x : %i", i, i2c_write(i2c, NULL, 0, i));
}

void config_gpio() {
	config_keys();
	config_leds();
	//configure rotary encoder
	gpio_pin_configure_dt(&rota, GPIO_INPUT);
	gpio_pin_configure_dt(&rotb, GPIO_INPUT);
}

void main(void) {

	config_gpio();
	
	if (i2c == NULL || !device_is_ready(i2c)) {
		LOG_ERR("i2c device failed");
		return;
	}
	
	//scan_i2c();
	//
	if (display == NULL) {
		LOG_ERR("device pointer is NULL");
		while(1) {
			set_led_2(255, 0, 0);
			k_msleep(500);
			set_led_2(0, 0, 0);
			k_msleep(500);
		}
		return;
	}
	LOG_INF("Device pointer not null");
	if (!device_is_ready(display)) {
		LOG_ERR("display device is not ready");
		while (!device_is_ready(display)) {
			set_led_2(0, 127, 127);
			k_msleep(700);
			set_led_2(0, 0, 0);
			k_msleep(500);
		}
		//return;
	}

	int ret;
	ret = cfb_framebuffer_init(display);
	if (ret != 0) {
		LOG_ERR("could not initialize display");
		while (1) {
			set_led_2(0, 0, 255);
			k_msleep(1000);
			set_led_2(0, 0, 0);
			k_msleep(1000);
		}
		return;
	}
	ret = cfb_print(display, "Hello, world!", 0, 0);
	if (ret != 0) {
		LOG_ERR("could not print to display!");
		while (1) {
			toggle_led_1();
			k_msleep(250);
		};
		return;
	}
	ret = cfb_framebuffer_finalize(display);
	if (ret != 0) {
		LOG_ERR("could not finalize to display");
		while (1) {
			set_led_2(0, 0, 255);
			k_msleep(250);
			set_led_2(0, 0, 0);
			toggle_led_1();
			k_msleep(500);
		}
		return;
	}
	while (1) {
		toggle_led_1();
		k_msleep(5000);
	}
}
