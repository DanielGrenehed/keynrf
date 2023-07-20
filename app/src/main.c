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

LOG_MODULE_REGISTER(main);
static const struct device *display = DEVICE_DT_GET(DT_NODELABEL(ssd1306));
static const struct device *i2c = DEVICE_DT_GET(DT_NODELABEL(i2c0));

void key_callback(uint8_t key, uint8_t state) {
	LOG_INF("Key %i state: %i", key, state);
}

void scan_i2c() {
	for (int i = 0; i < 127; i++) LOG_INF("i2c: %x : %i", i, i2c_write(i2c, NULL, 0, i));
}

void config_gpio() {
	config_keys();
	set_keys_callback(key_callback);
	config_leds();
	//configure rotary encoder
	gpio_pin_configure_dt(&rota, GPIO_INPUT);
	gpio_pin_configure_dt(&rotb, GPIO_INPUT);
}

void loop(uint8_t r, uint8_t g, uint8_t b, uint8_t l1, uint32_t delay) {
	uint32_t lt = k_uptime_get();
	uint8_t roll = 0;
	while (1) {
		poll_keys();
		if (k_uptime_get() > lt + delay) {
			if (roll) set_led_2(r,g,b);
			else set_led_2(0,0,0);
			if (l1) toggle_led_1();
			roll = !roll;
			lt = k_uptime_get();
		}
	}
}

void main(void) {

	config_gpio();
	
	if (i2c == NULL || !device_is_ready(i2c)) {
		LOG_ERR("i2c device failed");
	}
	
	scan_i2c();
	
	if (display == NULL) {
		LOG_ERR("device pointer is NULL");
		loop(255, 0, 0, 0, 500);
	}
	LOG_INF("Device pointer not null");
	if (!device_is_ready(display)) {
		LOG_ERR("display device is not ready");
		loop(0, 127, 127, 0, 5000);
	}

	int ret;
	ret = cfb_framebuffer_init(display);
	if (ret != 0) {
		LOG_ERR("could not initialize display");
		loop(0, 0, 255, 0, 1000);	
	}

	ret = cfb_print(display, "Hello, world!", 0, 0);
	
	if (ret != 0) {
		LOG_ERR("could not print to display!");
		loop(0, 0, 0, 1, 250);
	}
	ret = cfb_framebuffer_finalize(display);
	if (ret != 0) {
		LOG_ERR("could not finalize to display");
		loop(0, 0, 255, 1, 400);
	}
	LOG_INF("Passed all tests!");
	loop(0,0,0,1,5000);
}
