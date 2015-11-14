/*

lcd_moving.h

Author: Aaron crump

*/

#ifndef lcd_moving_H_
#define lcd_moving_H_

//function prototypes

void LCD_light_init(void);
void Lcd_Light_bright(uint8_t brigtness);
void Scrolling_Text(char message[]);
void Scrolling_Text_single(char input[], uint8_t position);
void display_temp(uint8_t temp);
void array_shift(char message[]);

#endif /* lcd_moving_H_ */