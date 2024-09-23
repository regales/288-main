#include "cyBot_Scan.h"
#include "cyBot_uart.h"
#include "Timer.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>

void main() {

    timer_init();
    lcd_init();
    cyBot_uart_init();
    cyBOT_init_Scan(0b0011);

    cyBOT_Scan_t scanData;
    //cyBOT_SERVO_cal();
    right_calibration_value = -34000;
    left_calibration_value = 1324750;
    int loop;
    int angle;
    char data[200];


    sprintf(data, "Degrees\t\tDistance (m)\r");
       for (loop = 0; loop < strlen(data); loop++) {
           cyBot_sendByte(data[loop]);
       }

    for(angle = 0; angle <= 180; angle += 2) {
       cyBOT_Scan(angle, &scanData);
       sprintf(data, "%d\t\t%.2f\r", angle, scanData.sound_dist/100);
       for (loop = 0; loop < strlen(data); loop++) {
           cyBot_sendByte(data[loop]);
       }
    }
}
