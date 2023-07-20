#include <stdint.h>
#include <keys.h>
#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define DEBOUNCE_TIME 10

#define ROWS 4
#define COLS 4

static const struct gpio_dt_spec rows[ROWS] = {
	GPIO_DT_SPEC_GET(DT_NODELABEL(row1), gpios),
	GPIO_DT_SPEC_GET(DT_NODELABEL(row2), gpios),
	GPIO_DT_SPEC_GET(DT_NODELABEL(row3), gpios),
	GPIO_DT_SPEC_GET(DT_NODELABEL(row4), gpios)
};
static const struct gpio_dt_spec cols[COLS] = {
	GPIO_DT_SPEC_GET(DT_NODELABEL(col1), gpios),
	GPIO_DT_SPEC_GET(DT_NODELABEL(col2), gpios),
	GPIO_DT_SPEC_GET(DT_NODELABEL(col3), gpios),
	GPIO_DT_SPEC_GET(DT_NODELABEL(col4), gpios)
};

LOG_MODULE_REGISTER(keys);

static long btn_debounces[ROWS * COLS];
static uint8_t btn_states[ROWS * COLS];

void config_keys() {
	for (int r = 0; r < ROWS; r++) gpio_pin_configure_dt(&rows[r], GPIO_INPUT);
	for (int c = 0; c < COLS; c++) gpio_pin_configure_dt(&cols[c], GPIO_OUTPUT);
	for (int i = 0; i < ROWS * COLS; i++) btn_debounces[i] = 0;
}

static void (*key_event_callback)(uint8_t key, uint8_t state) = NULL;

void set_keys_callback(void (*cb)(uint8_t, uint8_t)) {
	key_event_callback = cb;
}

enum key_poll_states {
	col_change,
	poll_rows
};

static enum key_poll_states key_poll_state = 0;
static int col = 0;


void poll_keys() {
	
	switch (key_poll_state) {
		case col_change:
			{
				gpio_pin_set_dt(&cols[col], 0);
				col = (COLS + 1) % COLS;
				gpio_pin_set_dt(&cols[col], 1);
			}
			break;
		case poll_rows:
			{
				int c_o = col * ROWS;
				long current_time = k_uptime_get();
				for (int r = 0; r < ROWS; r++) {
					int current_state = gpio_pin_get_dt(&rows[r]);
					int btn = c_o+r; 
					if (btn_debounces[btn] && (btn_debounces[btn] + DEBOUNCE_TIME) < current_time ) {
						btn_debounces[btn] = 0;
						btn_states[btn] = current_state;
						if (btn_states[btn] != current_state && key_event_callback != NULL) key_event_callback(btn, current_state);
					} else if (btn_states[btn] != current_state) btn_debounces[btn] = current_time;
				}
			}
			break;
		default:
	}

	key_poll_state = (key_poll_state == poll_rows) ? 0 : key_poll_state + 1;
}
