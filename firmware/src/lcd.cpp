#include "lcd.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>
#include <stdio.h>

// LCD at I2C address 0x27, 16 columns x 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Cache of what is currently shown on each row
static char lastLine0[17] = "";
static char lastLine1[17] = "";
static bool lcd_ready = false;

// Helper: write exactly 16 chars to one LCD row only if content changed
static void lcd_write_line(uint8_t row, const char *text) {
    if (text == nullptr) {
        return;
    }

    char padded[17];
    snprintf(padded, sizeof(padded), "%-16.16s", text);

    char *last = (row == 0) ? lastLine0 : lastLine1;

    if (strncmp(last, padded, 16) == 0) {
        return; // no change, avoid extra I2C traffic
    }

    lcd.setCursor(0, row);
    lcd.print(padded);
    strncpy(last, padded, 16);
    last[16] = '\0';
}

void lcd_init(void) {
    Wire.begin();
    lcd.begin(16, 2);
    lcd.backlight();

    lcd_ready = true;

    // Force first update to write
    lastLine0[0] = '\0';
    lastLine1[0] = '\0';

    lcd_show_startup();
}

void lcd_show_startup(void) {
    if (!lcd_ready) {
        return;
    }

    lcd_write_line(0, "GPS Initializing");
    lcd_write_line(1, "Please wait...");
}

void lcd_show_no_fix(void) {
    if (!lcd_ready) {
        return;
    }

    lcd_write_line(0, "No GPS Fix");
    lcd_write_line(1, "Searching...");
}

void lcd_update(const GPSData *data) {
    if (!lcd_ready) {
        return;
    }

    if (data == nullptr || !data->fix) {
        lcd_show_no_fix();
        return;
    }

    char line0[17];
    char line1[17];

    // Fit within 16 chars
    // Example: "Lat:35.9830"
    //          "Lon:-95.8216"
    snprintf(line0, sizeof(line0), "Lat:%8.4f", data->latitude);
    snprintf(line1, sizeof(line1), "Lon:%8.4f", data->longitude);

    lcd_write_line(0, line0);
    lcd_write_line(1, line1);
}
