/*
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
    right_calibration_value = 311500;
    left_calibration_value = 1319500;
    int loop;
    int angle;
    char data[200];
    char command;

    while (1) {
        command = cyBot_getByte();
        if (command == 'm') {
            sprintf(data, "Degrees\t\tDistance (m)\r");
            for (loop = 0; loop < strlen(data); loop++) {
                cyBot_sendByte(data[loop]);
            }

            for (angle = 0; angle <= 180; angle += 2) {
                cyBOT_Scan(angle, &scanData);
                sprintf(data, "%d\t\t%.2f\r", angle, scanData.sound_dist / 100);
                for (loop = 0; loop < strlen(data); loop++) {
                    cyBot_sendByte(data[loop]);
                }

                if (command == 'q') {
                    break;
                }
            }
        }

        if (command == 'q') {
            break;
        }
    }
}
 */



#include "cyBot_Scan.h"
#include "cyBot_uart.h"
#include "Timer.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "sensor-data.h"

// tolerance function (new)
int is_within_tolerance(float a, float b, float tolerance) {
    return fabs(a - b) <= tolerance;
}

// function to automatically clean up distance (new)
float cleanup_distance (float previous_distance, float current_distance){

    tolerance_val = 0.1;

    if (current_distance > 2.5) {
        current_distance = 2.5;
    }
    if (is_within_tolerance(previous_distance, current_distance, tolerance_val)){
        current_distance = previous_distance;
        return current_distance;
    } else {
        return current_distance;
    }
}

// function to automatically clean distance in header file (new)
void cleanup_distance_in_header_file () {
    float previous_distance = -1; // Initialize previous distance
    for (int i = 0; i < 91; i++) {
        sensor_data_array[i] = cleanup_distance(previous_distance, sensor_data_array[i]);
        previous_distance = sensor_data_array[i]; // Update previous distance
    }
}

void main() {
    timer_init();
    lcd_init();
    cyBot_uart_init();
    cyBOT_init_Scan(0b0011);

    cyBOT_Scan_t scanData;

    right_calibration_value = 311500;
    left_calibration_value = 1319500;

    float distance;
    float prev_distance = -1;
    int object_start = -1;
    int object_count = 0;
    int smallest_object_angle = -1;
    int smallest_object_width = 360;
    int current_object_width = 0;
    float object_distance = 0.0;
    int degree;
    int loop;
    char data[200];

    int start_angle, end_angle;

    cleanup_distance_in_header_file(); // added this (not sure if it works)

    for (degree = 0; degree <= 180; degree += 2) {
        distance = sensor_data_array[degree / 2];

        if (distance < 1.5 && prev_distance >= 1.5) {
            object_start = degree;
            object_distance = distance;
        }
        else if (distance >= 1.5 && prev_distance < 1.5 && object_start != -1) {
            end_angle = degree;
            current_object_width = end_angle - object_start;

            object_count++;

            sprintf(data, "%d\t%d\t%.2f\t%d\r", object_count, object_start + (current_object_width / 2), object_distance, current_object_width);
            for (loop = 0; loop < strlen(data); loop++) {
                cyBot_sendByte(data[loop]);
            }


            if (current_object_width < smallest_object_width) {
                smallest_object_width = current_object_width;
                smallest_object_angle = object_start + (current_object_width / 2);
            }
            object_start = -1;
        }

        prev_distance = distance;
    }

    if (smallest_object_angle != -1) {
        lcd_printf("Smallest object at %d degrees", smallest_object_angle);
        sprintf(data, "Smallest object at %d degrees\r", smallest_object_angle);
        for (loop = 0; loop < strlen(data); loop++) {
            cyBot_sendByte(data[loop]);
        }


        cyBOT_Scan(smallest_object_angle, &scanData);
    } else {
        lcd_printf("No object detected.");
        sprintf(data, "No object detected.\r");
        for (loop = 0; loop < strlen(data); loop++) {
            cyBot_sendByte(data[loop]);
        }
    }

    while (1) {}
}





