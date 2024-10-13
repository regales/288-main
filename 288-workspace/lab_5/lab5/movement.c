#include "movement.h"
#include "open_interface.h"

void move_forward(oi_t *sensor, int centimeters){

        double sum = 0;
        oi_setWheels(200, 200); // move forward; full speed


        while (sum < centimeters * 10) {
            oi_update(sensor);  // Update sensor data
            sum += sensor->distance;
            sensor -> distance = 0;


        }

        oi_setWheels(0, 0); // stop

}
void move_backwards(oi_t *sensor, int centimeters){

        double sum = 0;
        oi_setWheels(-200, -200);

        // Simulate the loop until 1000mm distance is covered
        while (sum < centimeters * 10) {
            oi_update(sensor);  // Update sensor data
            sum -= sensor->distance;
            sensor -> distance = 0;
        }

        oi_setWheels(0, 0); // stop

}
void turn_clockwise(oi_t *sensor, int degrees){

    oi_setWheels(-100, 100);
    double sum = 0;

    while (sum > -degrees){
        oi_update(sensor);  // Update sensor data
        sum += sensor->angle;
        sensor -> angle = 0;
    }
    oi_setWheels(0, 0);
}

void turn_counter_clockwise(oi_t *sensor, int degrees){

    oi_setWheels(100, -100);
    double sum = 0;

    while (sum < degrees){
        oi_update(sensor);  // Update sensor data
        sum += sensor->angle;
        sensor -> angle = 0;
    }
    oi_setWheels(0, 0);
}

void handle_bump(oi_t *sensor, char bump_direction){
    move_backwards(sensor, 15);

    if(bump_direction == 'L'){
        turn_clockwise(sensor, 90);
    }
    else if (bump_direction == 'R'){
        turn_counter_clockwise(sensor, 90);
    }

    move_forward(sensor, 25);

    if(bump_direction == 'L'){
        turn_counter_clockwise(sensor, 90);
    }
    else if (bump_direction == 'R'){
        turn_clockwise(sensor, 90);
    }


}
void move_forward_with_bump(oi_t *sensor, int centimeters){
    double total_distance = 0;
    int total_goal = centimeters * 10;
            oi_setWheels(200, 200); // move forward; full speed

            // Simulate the loop until 1000mm distance is covered
            while (total_distance < total_goal) {
                oi_update(sensor);  // Update sensor data
                if(sensor -> bumpLeft || sensor -> bumpRight){
                    oi_setWheels(0, 0);

                    if (sensor -> bumpLeft){
                    handle_bump(sensor, 'L');

                }
                else if (sensor -> bumpRight){
                    handle_bump(sensor, 'R');
                }
               // total_distance += sensor ->distance;
                //sensor -> distance = 0;
                    total_goal += 150;                           //!!!!!!!!!!!!Look here
                    oi_setWheels(200, 200);
            }


                if (sensor->distance > 0) {
                            total_distance += sensor->distance;
                        }

                        sensor->distance = 0;  // Reset the sensor distance after each update
                    }

            oi_setWheels(0, 0);

}







