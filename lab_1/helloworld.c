/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Describtion: Added timer_init call, and including Timer.h
 */

#include "Timer.h"
#include "lcd.h"
#include <string.h>

int main(void) {

    timer_init();
    lcd_init();

  int i, j, k = 0;
    char* message = "                  Microcontrollers are lots of fun!";
    int length = strlen(message);
    char buffer[21] = {0};

    while (1) {
        for (i = 0; i < length; i++) {
            for (j = 0; j < 20; j++) {
                if (i + j < length) {
                    buffer[j] = message[i + j];
                } else {
                    buffer[j] = ' ';
                }
            }
            buffer[20] = '\0'; // Ensure null termination
            lcd_printf("%s", buffer); // Correct format specifier
            timer_waitMillis(300);
        }

        lcd_clear(); // Assume this is a function to clear the LCD
        timer_waitMillis(300);
    }

    return 0;
}


