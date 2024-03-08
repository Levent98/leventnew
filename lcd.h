#include "sysclock.h"
#include "delay.h"
#include "i2c.h"



void lcd_init(void);
void lcd_send_cmd (char );
void lcd_put_cur(int, int);
void lcd_clear (void);
void lcd_send_data (char );
void lcd_send_string (char *);
void app_main(void);

