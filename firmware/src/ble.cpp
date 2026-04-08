#include "ble.h"
#include <Arduino.h>

static bool ble_initialized = false;

bool ble_init(unsigned long baud_rate) {
    Serial1.begin(baud_rate);
    delay(100);

    ble_initialized = true;
    return true;
}

void ble_send_string(const char *message) {
    if (!ble_initialized || message == nullptr) {
        return;
    }

    Serial1.print(message);
}

void ble_send_line(const char *message) {
    if (!ble_initialized || message == nullptr) {
        return;
    }

    Serial1.println(message);
}

void ble_send_command(const char *command) {
    if (!ble_initialized || command == nullptr) {
        return;
    }

    Serial1.print(command);
}

bool ble_available(void) {
    if (!ble_initialized) {
        return false;
    }

    return Serial1.available() > 0;
}

int ble_read_char(void) {
    if (!ble_initialized) {
        return -1;
    }

    return Serial1.read();
}

bool ble_is_connected(void) {
    return ble_initialized;
}
