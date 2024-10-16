/**
 * lab6_template_extra_help.c
 *
 * Description: This is file is meant for those that would like a little
 *              extra help with formatting their code.
 *
 */

#define _PART1 1
#define _PART2 0
#define _PART3 0

#include "timer.h"
#include "lcd.h"
#include "uart.h"


// Adding global volatile varibles for communcating between
// your Interupt Service Routine, and your non-interupt code.

volatile  char uart_data;  // Your UART interupt code can place read data here
volatile  char flag;       // Your UART interupt can update this flag
// to indicate that it has placed new data
// in uart_data


void main()
{

    timer_init();
    lcd_init();
    uart_init(115200);
    uart_interrupt_init();

    lcd_clear();

    //#if _PART1      // Receive and display text

    while (1){
        char input_char = 0;
        char data[21];
        int i = 0;

        while (i < 20)
        {
            input_char = uart_receive(); //read the received data from UART
            if (input_char == '\r') {
                break;
            }
            data[i] = input_char;
            i++;
        }
        data[i] = '\0';
        uart_sendStr(data); //send receive string back to UART
        lcd_clear();
        lcd_home();
        lcd_printf("%s", data);
    }


    //#endif
    //
    //#if _PART2      // Echo Received Character
    //    // TODO
    //
    //#endif
    //
    //#if _PART3 // Receive and send characters using interrupts.
    // // TODO
    //
    //#endif

}

