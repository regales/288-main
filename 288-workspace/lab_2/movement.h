#ifndef MOVEMENT_H
#define MOVEMENT_H
#include "open_interface.h"

// Moves the robot forward
void move_forward(oi_t *sensor, int centimeters);

// Turns the robot by "degree" degrees, - is clockwise, + is counter
void turn_clockwise(oi_t *sensor, int angle);

void turn_counter_clockwise(oi_t *sensor, int angle);

void move_forward_with_bump(oi_t *sensor, int centimeters);

void handle_bump(oi_t *sensor, char bump_direction);

void move_backwards(oi_t *sensor, int centimeters);

#endif
