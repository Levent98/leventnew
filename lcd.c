#include "lcd.h"
#include <stdint.h>
#include "i2c.h"

void Delay_ms(uint16_t);
//extern void master_to_device_write(uint8_t ,const uint8_t* , size_t );
#define Adress_write 0x41
#define Address_read 0x40

void lcd_send_cmd (char cmd)
{
	
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0

	master_to_device_write(0x4E ,(uint8_t*)data_t ,4);
	
	
}
void lcd_send_data (char data)
{
	
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0

	master_to_device_write(0x4E ,(uint8_t*)data_t ,4);
	
	
}


void lcd_init()
	{
	Delay_ms(50);//wait for >40ms
	lcd_send_cmd(0x30);
	Delay_ms(5);
	lcd_send_cmd(0x30);
	Delay_us(200);
	lcd_send_cmd(0x30);
	Delay_ms(10);
	lcd_send_cmd(0x20);
	Delay_ms(10);
	
 //display initialization 
	lcd_send_cmd(0x28);
	Delay_ms(1);
	lcd_send_cmd(0x08);
	Delay_ms(1);
	lcd_send_cmd(0x1);//Clear display
	Delay_ms(2);
	lcd_send_cmd(0x06);
	Delay_ms(1);
	lcd_send_cmd(0x0C);
	Delay_ms(1);
}

	
void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    lcd_send_cmd (col);
}	
void lcd_clear (void)
{
	lcd_send_cmd (0x01);
	Delay_ms(5);
}
void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}

void app_main(void){

	lcd_init();
  lcd_clear();

  lcd_put_cur(0, 0);
  lcd_send_string("Merhaba!");

  lcd_put_cur(1, 0);
  lcd_send_string("bugun guzel");	
	
	
}	

	
