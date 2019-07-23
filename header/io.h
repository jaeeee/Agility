#ifndef __io_h__
#define __io_h__

void LCD_init();
void LCD_ClearScreen(void);
void LCD_WriteCommand (unsigned char Command);
// void LCD_CreateCustom(unsigned char index, const unsigned char Data[8]);
// void LCD_CreateCustom
void LCD_createChar(unsigned char location, unsigned char *p);
void LCD_Cursor (unsigned char column);
void LCD_DisplayString(unsigned char column ,const unsigned char *string);
void delay_ms(int miliSec);
#endif
