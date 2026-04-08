#ifndef LCD_H
#define LCD_H

#include "gps.h"

void lcd_init(void);
void lcd_show_startup(void);
void lcd_show_no_fix(void);
void lcd_update(const GPSData *data);

#endif