#ifndef BLE_H
#define BLE_H

#include <stdbool.h>

bool ble_init(unsigned long baud_rate);
void ble_send_string(const char *message);
void ble_send_line(const char *message);
bool ble_is_connected(void);

// HM-10 configuration helpers (Optional)
void ble_send_command(const char *command);
bool ble_available(void);
int ble_read_char(void);

#endif
