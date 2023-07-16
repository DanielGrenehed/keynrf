#include <keys.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

static const struct gpio_dt_spec row1 = GPIO_DT_SPEC_GET(DT_NODELABEL(row1), gpios);
static const struct gpio_dt_spec row2 = GPIO_DT_SPEC_GET(DT_NODELABEL(row2), gpios);
static const struct gpio_dt_spec row3 = GPIO_DT_SPEC_GET(DT_NODELABEL(row3), gpios);
static const struct gpio_dt_spec row4 = GPIO_DT_SPEC_GET(DT_NODELABEL(row4), gpios);

static const struct gpio_dt_spec col1 = GPIO_DT_SPEC_GET(DT_NODELABEL(col1), gpios);
static const struct gpio_dt_spec col2 = GPIO_DT_SPEC_GET(DT_NODELABEL(col2), gpios);
static const struct gpio_dt_spec col3 = GPIO_DT_SPEC_GET(DT_NODELABEL(col3), gpios);
static const struct gpio_dt_spec col4 = GPIO_DT_SPEC_GET(DT_NODELABEL(col4), gpios);

void config_keys() {
	// config matrix col output
	gpio_pin_configure_dt(&col1, GPIO_OUTPUT);
	gpio_pin_configure_dt(&col2, GPIO_OUTPUT);
	gpio_pin_configure_dt(&col3, GPIO_OUTPUT);
	gpio_pin_configure_dt(&col4, GPIO_OUTPUT);
	// config matrix row input
	gpio_pin_configure_dt(&row1, GPIO_INPUT);
	gpio_pin_configure_dt(&row2, GPIO_INPUT);
	gpio_pin_configure_dt(&row3, GPIO_INPUT);
	gpio_pin_configure_dt(&row4, GPIO_INPUT);
}
