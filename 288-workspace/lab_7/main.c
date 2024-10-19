/**
 * lab6_template_extra_help.c
 *
 * Description: This is file is meant for those that would like a little
 *              extra help with formatting their code.
 *
 */


#define BUFFER_SIZE 20
#define NUM_SAMPLES 3
#define SCAN_DELAY_MS 1
#define NUM_OBJECTS 4

#include "timer.h"
#include "lcd.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "cyBot_Scan.h"
#include "open_interface.h"
#include "movement.h"

volatile  char uart_data;
volatile  char flag;

float distances[] = {9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48, 50};
int ir_values[] = {1499, 1277, 1147, 1087, 987, 955, 919, 877, 842, 833, 803, 795, 779, 751, 731};

typedef struct{
    float IR;
    float PING;
} Store_Data;

typedef struct {
    float start_angle;
    float end_angle;
    float mid_angle;
    float distance;
} Detected_Object;

float calculate_distance_linear(int raw_ir) {
    int i;
    if (raw_ir >= ir_values[0]) {
        return distances[0];
    }
    if (raw_ir <= ir_values[14]) {
        return distances[14];
    }
    for (i = 0; i < 14; i++) {
        if (raw_ir <= ir_values[i] && raw_ir > ir_values[i + 1]) {
            float slope = (distances[i + 1] - distances[i]) / (ir_values[i + 1] - ir_values[i]);
            return distances[i] + slope * (raw_ir - ir_values[i]);
        }
    }
    return -1;
}

void perform_scan_1(cyBOT_Scan_t *scanData) {
    int angle;
    int i;
    int raw_ir_sum, ping_sum;
    float avg_ir, avg_ping;

    Store_Data data[91]; // Store data for angles from 0 to 180
    Detected_Object objects[NUM_OBJECTS]; // Store detected objects
    int object_count = 0;

    uart_sendStr("\nAngle  \tPING Distance (cm)  \tIR Distance (cm)\n");
    uart_sendStr("---------------------------------------------------\n");

    // Scan from 0 to 180 degrees
    for (angle = 0; angle <= 180; angle += 2) {  // Scan every 2 degrees
        raw_ir_sum = 0;
        ping_sum = 0;

        // Collect NUM_SAMPLES values for averaging
        for (i = 0; i < NUM_SAMPLES; i++) {
            cyBOT_Scan(angle, scanData);
            raw_ir_sum += scanData->IR_raw_val;
            ping_sum += scanData->sound_dist;

            // Optional delay between readings
            timer_waitMillis(SCAN_DELAY_MS);
        }

        // Calculate the average for IR and ping values
        avg_ir = raw_ir_sum / (float)NUM_SAMPLES;
        avg_ping = ping_sum / (float)NUM_SAMPLES;

        data[angle / 2].IR = avg_ir;
        data[angle / 2].PING = avg_ping;

        // Display the averaged values
        char message[50];
        sprintf(message, "Angle: %d IR: %.2f PING: %.2f cm\r\n", angle, avg_ir, avg_ping);
        uart_sendStr(message);

        // Object detection logic
        if (avg_ir < 1087) { // Detecting an object
            if (object_count == 0 || objects[object_count - 1].end_angle != 0) {
                // New object detected
                objects[object_count].start_angle = angle;
                objects[object_count].end_angle = 0; // To be updated later
                object_count++;
            }
            objects[object_count - 1].end_angle = angle; // Update the end angle
        }
    }

    // Calculate mid-angles and measure distance using PING sensor
    for (i = 0; i < object_count; i++) {
        objects[i].mid_angle = (objects[i].start_angle + objects[i].end_angle) / 2;

        // Measure distance with PING sensor at mid-angle
        cyBOT_Scan(objects[i].mid_angle, scanData);
        objects[i].distance = scanData->sound_dist; // Store the PING distance
    }

    // Output detected objects
    uart_sendStr("\nDetected Objects:\n");
    for (i = 0; i < object_count; i++) {
        char obj_message[100];
        sprintf(obj_message, "Object %d: Start: %.1f°, End: %.1f°, Mid: %.1f°, Distance: %.2f cm\r\n",
                i + 1, objects[i].start_angle, objects[i].end_angle, objects[i].mid_angle, objects[i].distance);
        uart_sendStr(obj_message);
    }
}

void perform_scan(cyBOT_Scan_t *scanData) {
    int angle;
    int i;
    int raw_ir_sum, ping_sum;
    float avg_ir, avg_ping;

    Store_Data data[91];

    uart_sendStr("\nAngle  \tPING Distance (cm)  \tIR Distance (cm)\n");
    uart_sendStr("---------------------------------------------------\n");

    // Scan from 0 to 180 degrees
    for (angle = 0; angle <= 180; angle += 2) {  // Scan every 2 degrees
        raw_ir_sum = 0;
        ping_sum = 0;

        // Collect NUM_SAMPLES values for averaging
        for (i = 0; i < NUM_SAMPLES; i++) {
            cyBOT_Scan(angle, scanData);
            raw_ir_sum += scanData->IR_raw_val;
            ping_sum += scanData->sound_dist;

            // Delay between readings to slow down the scan
            //timer_waitMillis(SCAN_DELAY_MS);
        }

        // Calculate the average for IR and ping values
        avg_ir = raw_ir_sum / (float)NUM_SAMPLES;
        avg_ping = ping_sum / (float)NUM_SAMPLES;

//        data[angle/2].IR = avg_ir;
//        data[angle/2].PING = avg_ping;

        // Display the averaged values on the LCD and send to PuTTY
        lcd_printf("Angle: %d\nIR: %.2f\nPING: %.2f cm", angle, avg_ir, avg_ping);

        char message[50];
        sprintf(message, "Angle: %d IR: %.2f PING: %.2f cm\r\n", angle, avg_ir, avg_ping);
        for (i = 0; i < strlen(message); i++) {
            uart_sendChar(message[i]);
        }
    }
//    i = 0;
//    while(data[i].PING != 0){
//        if(i == 0){
//            cleanup_distance(0.0 , data[i].PING, data[i+1].PING);
//        } else {
//            cleanup_distance(data[i - 1].PING, data[i].PING, data[i+1].PING);
//        }
//        i++;
//    }

}

void control_movement(oi_t *sensor_data, char command) {
    switch (command) {
    case 'a':
        lcd_printf("Turning Left");
        turn_counter_clockwise(sensor_data, 5);
        timer_waitMillis(500);  // Small delay after turning
        break;
    case 'd':
        lcd_printf("Turning Right");
        turn_clockwise(sensor_data, 5);
        timer_waitMillis(500);  // Small delay after turning
        break;
    default:
        break;
    }
}

void move_robot(oi_t *sensor_data, char *move_command) {
    char direction = move_command[strlen(move_command) - 1];  // Get the direction ('w' or 's')
    move_command[strlen(move_command) - 1] = '\0';            // Remove the direction from the string
    int distance = atoi(move_command);                        // Convert remaining part to integer distance

    if (direction == 'w') {
        lcd_printf("Moving Forward %d cm", distance);
        move_forward(sensor_data, distance);  // Move forward
    } else if (direction == 's') {
        lcd_printf("Moving Backward %d cm", distance);
        move_backwards(sensor_data, distance);  // Move backward
    }
}

//int is_within_tolerance(float a, float b, float tolerance) {
//    return fabs(a - b) <= tolerance;
//}

//float cleanup_distance (float previous_distance, float current_distance, float next_distance){
//
//    float tolerance_val = 15.00;
//
//    if (current_distance > 50.0) {
//        current_distance = 50.0;
//    }
//    if (is_within_tolerance(previous_distance, current_distance, tolerance_val)){
//        current_distance = previous_distance;
//        return current_distance;
//    } else if (!is_within_tolerance(previous_distance, current_distance, tolerance_val)){
//        if (is_within_tolerance(current_distance, next_distance, tolerance_val)){
//            current_distance = next_distance;
//            return current_distance;
//        } else {
//            return 0.0;
//        }
//    }
//}



int main(void)
{
    uart_init(115200);
    lcd_init();
    timer_init();
    uart_interrupt_init();
    cyBOT_init_Scan(0b0111);
    right_calibration_value = 343000;
    left_calibration_value = 1340500;



    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    cyBOT_Scan_t scanData;

    lcd_printf("UART Manual Control and 180-degree scan");

    char command_buffer[10];  // Buffer to collect distance commands like '20w'
    int buffer_index = 0;

    while (1) {
        if (flag == 1) {  // Check for UART input
            flag = 0;

            // Append the received character to the buffer
            if (buffer_index < sizeof(command_buffer) - 1 && uart_data != '\r') {
                command_buffer[buffer_index++] = uart_data;
                command_buffer[buffer_index] = '\0';  // Null-terminate the string
            }

            // If a single character command is received ('a', 'd', 'm', 'q')
            if (uart_data == 'a' || uart_data == 'd') {
                control_movement(sensor_data, uart_data);
                buffer_index = 0;  // Reset the buffer after executing a command
            } else if (uart_data == 'm') {
                perform_scan_1(&scanData);
                buffer_index = 0;  // Reset the buffer after executing a command
            } else if (uart_data == 'q') {
                oi_free(sensor_data);
                uart_sendStr("Quitting...\r\n");
                break;
            } else if (uart_data == '\r') {  // Enter key pressed, indicating end of a command
                // Check if the command is a distance command like '20w' or '15s'
                if (buffer_index > 1) {
                    char direction = command_buffer[buffer_index - 1];
                    if ((direction == 'w' || direction == 's') && buffer_index > 1) {
                        move_robot(sensor_data, command_buffer);
                    } else {
                        uart_sendStr("Invalid command. Please use a valid format like '20w' or '15s'.\r\n");
                    }
                } else {
                    uart_sendStr("Invalid command. Please try again.\r\n");
                }
                buffer_index = 0;  // Reset the buffer after executing a command
                memset(command_buffer, 0, sizeof(command_buffer));  // Clear the buffer
            }

            // Echo the received character (for debugging purposes)
            if (uart_data != '\r') {
                uart_sendChar(uart_data);
            } else {
                uart_sendChar('\n');  // Send newline for Enter key
            }
        }
    }

    return 0;
}
