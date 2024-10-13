/**
 * lab4_template.c
 * 
 * Template file for CprE 288 lab 4
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
// PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1

#define REPLACEME 0



int main(void) {
    button_init();
    lcd_init();
    cyBot_uart_init();            // Don't forget to initialize the cyBot UART before trying to use it

    // YOUR CODE HERE
    int loop;
    char data[200];
    uint8_t prev_button = 0;


    while(1)
    {

        // YOUR CODE HERE
        uint8_t button = button_getButton();

        lcd_printf("Button %d", button);

        if (prev_button != button){

            switch (button) {
            case 1:
                sprintf(data, "Button 1 pressed\r");
                break;
            case 2:
                sprintf(data, "Button 2 pressed\r");
                break;
            case 3:
                sprintf(data, "Button 3 pressed\r");
                break;
            case 4:
                sprintf(data, "Button 4 pressed\r");
                break;
            default:
                prev_button = 0;
                continue;
            }

            for (loop = 0; loop < strlen(data); loop++) {
                cyBot_sendByte(data[loop]);
            }

        }
        prev_button = button;

    }

}
