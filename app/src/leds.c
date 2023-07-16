#include <stdint.h>
#include <leds.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/drivers/led.h>
#include <zephyr/drivers/gpio.h>

static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_NODELABEL(led1), gpios);
static const struct gpio_dt_spec led2r = GPIO_DT_SPEC_GET(DT_NODELABEL(led2r), gpios);
static const struct gpio_dt_spec led2g = GPIO_DT_SPEC_GET(DT_NODELABEL(led2g), gpios);
static const struct gpio_dt_spec led2b = GPIO_DT_SPEC_GET(DT_NODELABEL(led2b), gpios);

void config_leds() {
	gpio_pin_configure_dt(&led1, GPIO_OUTPUT);
	gpio_pin_configure_dt(&led2r, GPIO_OUTPUT);
	gpio_pin_configure_dt(&led2g, GPIO_OUTPUT);
	gpio_pin_configure_dt(&led2b, GPIO_OUTPUT);

	gpio_pin_toggle_dt(&led1);
	gpio_pin_toggle_dt(&led2r);
	gpio_pin_toggle_dt(&led2g);
	gpio_pin_toggle_dt(&led2b);
}

void set_led_2(uint8_t r, uint8_t g, uint8_t b) {
	led_set_brightness(&led2r, 0, r);
	led_set_brightness(&led2g, 0, g);
	led_set_brightness(&led2b, 0, b);
}

void toggle_led_2() {
	gpio_pin_toggle_dt(&led2r);
	gpio_pin_toggle_dt(&led2g);
	gpio_pin_toggle_dt(&led2b);
}

void set_led_1(uint8_t g) {
	led_set_brightness(&led1, 0, g);
}

void toggle_led_1() {
	gpio_pin_toggle_dt(&led1);
}
