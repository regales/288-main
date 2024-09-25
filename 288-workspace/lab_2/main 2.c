

#include <stdio.h>
#include <stdlib.h>
#include "open_interface.h"
#include "lcd.h"
#include "Timer.h"
#include "cyBot_uart.h"
#include "movement.h"


int main() {
    cyBot_uart_init();
    lcd_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    char command;

        while (1) {

            command = cyBot_getByte();

           //LCD
            lcd_printf("----> %c", command);

            // 'm' is pressed
            if (command == 'm') {
                cyBot_sendByte('G');
                            cyBot_sendByte('o');
                            cyBot_sendByte('t');
                            cyBot_sendByte(' ');
                            cyBot_sendByte('a');
                            cyBot_sendByte('n');
                            cyBot_sendByte(' ');
                            cyBot_sendByte('m');
                            cyBot_sendByte('\n');
            }


            if (command == 'w') {
                move_forward(sensor_data, 50);  // Move forward
            } else if (command == 's') {
                move_backwards(sensor_data, 25);  // Move backward
            } else if (command == 'a') {
                turn_counter_clockwise(sensor_data, 15);  // Turn left
            } else if (command == 'd') {
                turn_clockwise(sensor_data, 15);  // Turn right s
            }
            if (command == 'q'){
                oi_free(sensor_data);
                break;
            }
        }

            return 0;


   /* oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

                                                    //part3
    move_forward_with_bump(sensor_data, 200);

    oi_free(sensor_data);

    return 0;*/
}
