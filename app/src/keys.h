#ifndef KEYNRF_KEYS_H
#define KEYNRF_KEYS_H

void config_keys();

void set_keys_callback(void (*cb)(uint8_t key, uint8_t state));

void poll_keys();

#endif /* KEYNRF_KEYS_H */
