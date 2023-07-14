#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/display/cfb.h>
#include <zephyr/logging/log.h>

static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_NODELABEL(led1), gpios);

static const struct gpio_dt_spec led2r = GPIO_DT_SPEC_GET(DT_NODELABEL(led2r), gpios);
static const struct gpio_dt_spec led2g = GPIO_DT_SPEC_GET(DT_NODELABEL(led2g), gpios);
static const struct gpio_dt_spec led2b = GPIO_DT_SPEC_GET(DT_NODELABEL(led2b), gpios);

LOG_MODULE_REGISTER(display);
static const struct device *display = DEVICE_DT_GET(DT_NODELABEL(ssd1306));
static const struct device *i2c = DEVICE_DT_GET(DT_NODELABEL(i2c0));


void main(void) {
	// configure leds
	gpio_pin_configure_dt(&led1, GPIO_OUTPUT);
	gpio_pin_configure_dt(&led2r, GPIO_OUTPUT);
	gpio_pin_configure_dt(&led2g, GPIO_OUTPUT);
	gpio_pin_configure_dt(&led2b, GPIO_OUTPUT);
	
	// turn off all leds
	gpio_pin_toggle_dt(&led1);
	gpio_pin_toggle_dt(&led2r);
	gpio_pin_toggle_dt(&led2g);
	gpio_pin_toggle_dt(&led2b);
	
	if (i2c == NULL || !device_is_ready(i2c)) {
		LOG_ERR("i2c device failed");
		return;
	}
	
	for (int i = 0; i < 127; i++) {
		LOG_INF("i2c: %x : %i", i, i2c_write(i2c, NULL, 0, i));
	}

	LOG_INF("Hello world!");
	if (display == NULL) {
		LOG_ERR("device pointer is NULL");
		while(1) {
			gpio_pin_toggle_dt(&led2r);
			k_msleep(500);
		}
		return;
	}
	LOG_INF("Device pointer not null");
	if (!device_is_ready(display)) {
		LOG_ERR("display device is not ready");
		while (!device_is_ready(display)) {
			gpio_pin_toggle_dt(&led2g);
			k_msleep(700);
			gpio_pin_toggle_dt(&led2b);
		}
		//return;
	}

	int ret;
	ret = cfb_framebuffer_init(display);
	if (ret != 0) {
		LOG_ERR("could not initialize display");
		while (1) {
			gpio_pin_toggle_dt(&led2b);
			k_msleep(1000);
		}
		return;
	}
	ret = cfb_print(display, "Hello, world!", 0, 0);
	if (ret != 0) {
		LOG_ERR("could not print to display!");
		while (1) {
			gpio_pin_toggle_dt(&led1);
			k_msleep(250);
		};
		return;
	}
	ret = cfb_framebuffer_finalize(display);
	if (ret != 0) {
		LOG_ERR("could not finalize to display");
		while (1) {
			gpio_pin_toggle_dt(&led2b);
			k_msleep(250);
			gpio_pin_toggle_dt(&led1);
			k_msleep(500);
		}
		return;
	}
	while (1) {
		gpio_pin_toggle_dt(&led1);
		k_msleep(5000);
	}


}
