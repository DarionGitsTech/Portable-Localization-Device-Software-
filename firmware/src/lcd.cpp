#include "lcd.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void lcd_init(void) {
    Wire.begin();
    lcd.begin(16, 2);
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("GPS Initializing");
    lcd.setCursor(0, 1);
    lcd.print("Please wait...");
}

void lcd_show_startup(void) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("GPS Initializing");
    lcd.setCursor(0, 1);
    lcd.print("Please wait...");
}

void lcd_show_no_fix(void) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No GPS Fix");
    lcd.setCursor(0, 1);
    lcd.print("Searching...");
}

void lcd_update(const GPSData *data) {
    if (data == nullptr) {
        lcd_show_no_fix();
        return;
    }

    lcd.clear();

    if (data->fix) {
        lcd.setCursor(0, 0);
        lcd.print("Lat:");
        lcd.print(data->latitude, 2);

        lcd.setCursor(0, 1);
        lcd.print("Lon:");
        lcd.print(data->longitude, 2);
    } else {
        lcd_show_no_fix();
    }
}
