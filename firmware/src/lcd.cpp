#include "lcd.h"
#include <wire.h>
#include <LiquidCrystal_I2C.h>

// Common I2C address is 0x27 or 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

void lcd_init() {
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("GPS Initializing");
}

void lcd_update(GPSData* data) {
  lcd.clear();

  if(data->valid) {
    lcd.setCursor(0, 0);
    lcd.print("Lat:");
    lcd.print(data->longitude, 2);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("No GPS Fix");
  }
}
