#include <stdint.h>
#include <leds.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_NODELABEL(led1), gpios);

static const struct pwm_dt_spec led2_r = PWM_DT_SPEC_GET(DT_NODELABEL(red_pwm_led));
static const struct pwm_dt_spec led2_g = PWM_DT_SPEC_GET(DT_NODELABEL(green_pwm_led));
static const struct pwm_dt_spec led2_b = PWM_DT_SPEC_GET(DT_NODELABEL(blue_pwm_led));

LOG_MODULE_REGISTER(leds);


static uint32_t max_period;

static void set_pwm(const struct pwm_dt_spec * dev, uint8_t v) {
	uint32_t period = v * max_period / 255;
	if (pwm_set_pulse_dt(dev, period)) {
		LOG_ERR("Failed to set pulse %i from value %i, Max %i", period, v, max_period);
		return;
	}
}

void config_leds() {
	gpio_pin_configure_dt(&led1, GPIO_OUTPUT);
	
	gpio_pin_toggle_dt(&led1);

	if (!device_is_ready(led2_r.dev) || !device_is_ready(led2_g.dev) || !device_is_ready(led2_b.dev)) {
		LOG_ERR("PWM leds not ready!");
		return;
	}

	max_period = led2_r.period;
	LOG_INF("Period: %u", max_period);
	set_led_2(0, 0, 0);
}

void set_led_2(uint8_t r, uint8_t g, uint8_t b) {
	set_pwm(&led2_r, r);
	set_pwm(&led2_g, g);
	set_pwm(&led2_b, b);
}

void toggle_led_1() {
	gpio_pin_toggle_dt(&led1);
}
