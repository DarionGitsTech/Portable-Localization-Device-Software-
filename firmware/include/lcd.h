#ifndef LCD_H
#define LCD_H

#include "gps.h"

void lcd_init();
void lcd_update(GPSData* data);

#endif
