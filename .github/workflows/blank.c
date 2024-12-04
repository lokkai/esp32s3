#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Define number of elements GPIO pin arrays
#define NUM_LEDS 3 // number of LEDs in each direction
#define NUM_DIRECTIONS 2  // 2 directions the one intersection

// Input the GPIO pins that represent the LED "Traffic Lights". (JI)
int traffic_lights[NUM_DIRECTIONS][NUM_LEDS] = {
    {21, 38, 37}, // Direction 1: RED1, YELLOW1, GREEN1
    {16, 17, 18}   // Direction 2: RED2, YELLOW2, GREEN2
};

// Pedestrian GPIO pins
#define PIR_SENSOR_GPIO 3 //Assigned GPIO value to PIR SENSOR. (JI)
#define PUSH_BUTTON_GPIO 4 //Assigned GPIO value to PUSH button. (JI)
#define START_BUTTON_GPIO 5 //Assigned GPIO value to START button. (JI)

int white_leds[NUM_DIRECTIONS] = {36, 8}; // WHITE1 (Dir1), WHITE2 (Dir2)

// Function Declarations
void initialize_gpio();
void normal_traffic_sequence();
void pedestrian_signal(int pedestrian_input, int same_traffic[], int opposite_traffic[], int white_led);

// Main function
void app_main() {
    
initialize_gpio(); //Calling function to initialize LEDs as outputs. (JI)

    while (1) {
        
        vTaskDelay(500 / portTICK_PERIOD_MS); // 0.5 second delay (JI)

        // Read button states to check for pedestrian crossing. (JI)
        int push_button_state = gpio_get_level(PUSH_BUTTON_GPIO);
        int start_button_state = gpio_get_level(START_BUTTON_GPIO);
        int pir_motion_state = gpio_get_level(PIR_SENSOR_GPIO);

      if (start_button_state == 1) {
         printf("Start button pressed! Beginning traffic sequence...\n");
        normal_traffic_sequence();
      }    

      else if (pir_motion_state == 1) {
        pedestrian_signal(1, traffic_lights[0], traffic_lights[1], white_leds[0]);
       }

      else if (push_button_state == 1) {
        pedestrian_signal(1, traffic_lights[1], traffic_lights[0], white_leds[1]);
       }

    }


} // End of app_main (JI)

// Function definition for initializing LEDs as outputs. (JI)
// Two for-loops with one being nested is required because it initializes a 2D array. (JI)
void initialize_gpio() {
 
  gpio_set_direction(START_BUTTON_GPIO, GPIO_MODE_INPUT); // Setting "START" button as output. (JI)
   
    for (int i = 0; i < NUM_DIRECTIONS; i++) { // Loop through each direction of traffic. (JI)
        for (int j = 0; j < NUM_LEDS; j++) { // Loop through each LED in that direction (JI)
            gpio_set_direction(traffic_lights[i][j], GPIO_MODE_OUTPUT); // Each traffic light is set as an output (JI).
        }
    }
  
  gpio_set_direction(PIR_SENSOR_GPIO, GPIO_MODE_INPUT); // Setting the motion sensor as an input (JI)
  gpio_set_direction(PUSH_BUTTON_GPIO, GPIO_MODE_INPUT); // Setting direction of push buttons as inputs. (JI)
  
  for (int i = 0; i < NUM_DIRECTIONS; i++) {
    gpio_set_direction(white_leds[i], GPIO_MODE_OUTPUT); // Setting the white LEDs as an output. (JI). 
  }
  
}
      
// This is the function definition for the normal traffic sequence. (JI)
// The sequences calls upon a 2D array. The specific elements are selected through it's numerical value representation in the array. (JI)
void normal_traffic_sequence() {
    
    //***Sequence 1 ***

    gpio_set_level(traffic_lights[0][0], 1); // RED1 ON
    gpio_set_level(traffic_lights[0][1], 0); // YELLOW1 OFF
    gpio_set_level(traffic_lights[0][2], 0); // GREEN1 OFF
    

    gpio_set_level(traffic_lights[1][0], 0); // RED2 OFF
    gpio_set_level(traffic_lights[1][1], 0); // YELLOW2 OFF
    gpio_set_level(traffic_lights[1][2], 1); // GREEN2 ON
   
    vTaskDelay(5000 / portTICK_PERIOD_MS);

    //***Sequence 2***
    
    gpio_set_level(traffic_lights[0][0], 0); // RED1 OFF
    gpio_set_level(traffic_lights[0][1], 0); // YELLOW1 OFF
    gpio_set_level(traffic_lights[0][2], 1); // GREEN1 ON
    

    gpio_set_level(traffic_lights[1][0], 0); // RED2 OFF
    gpio_set_level(traffic_lights[1][1], 1); // YELLOW2 ON
    gpio_set_level(traffic_lights[1][2], 0); // GREEN2 OFF
    
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    //***Sequence 3***
    
    gpio_set_level(traffic_lights[0][0], 0); // RED1 OFF
    gpio_set_level(traffic_lights[0][1], 0); // YELLOW1 OFF
    gpio_set_level(traffic_lights[0][2], 1); // GREEN1 ON
    

    gpio_set_level(traffic_lights[1][0], 1); // RED2 ON
    gpio_set_level(traffic_lights[1][1], 0); // YELLOW2 OFF
    gpio_set_level(traffic_lights[1][2], 0); // GREEN2 OFF
  
    vTaskDelay(5000 / portTICK_PERIOD_MS);

    //***Sequence 4***
    
    gpio_set_level(traffic_lights[0][0], 0); // RED1 OFF
    gpio_set_level(traffic_lights[0][1], 1); // YELLOW1 ON
    gpio_set_level(traffic_lights[0][2], 0); // GREEN1 OFF


    gpio_set_level(traffic_lights[1][0], 1); // RED2 ON
    gpio_set_level(traffic_lights[1][1], 0); // YELLOW2 OFF
    gpio_set_level(traffic_lights[1][2], 0); // GREEN2 OFF
    
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    //***Sequence 5***
    
    gpio_set_level(traffic_lights[0][0], 0); // RED1 OFF
    gpio_set_level(traffic_lights[0][1], 0); // YELLOW1 OFF
    gpio_set_level(traffic_lights[0][2], 0); // GREEN1 OFF
    

    gpio_set_level(traffic_lights[1][0], 0); // RED2 OFF
    gpio_set_level(traffic_lights[1][1], 0); // YELLOW2 OFF
    gpio_set_level(traffic_lights[1][2], 0); // GREEN2 OFF
    
}


// Function definition for "pedestrian_signal". (JI)
void pedestrian_signal(int pedestrian_input, int same_traffic[], int opposite_traffic[], int white_led) {
    
    printf("Pedestrians Crossing!\n");
    
    gpio_set_level(same_traffic[2], 1); // Turns the Red LED of the same_traffic ON
    gpio_set_level(opposite_traffic[0], 1); // Turns the RED LED of the opposite_traffic OFF
    
    // For-loop to create the white LED blinking sequence. Less than 5 because 0 counts as the first iteration. (JI)
    for (int i = 0; i < 5; i++) {
      gpio_set_level(white_led, 1); // White LEDs on (JI)
      vTaskDelay(500 / portTICK_PERIOD_MS); // 5 seconds delay (JI)
      gpio_set_level(white_led, 0); // White LEDs off (JI)
      printf("%d\n",(5 - i)); // Countdown sequence after each blink. (JI)
      vTaskDelay(500 / portTICK_PERIOD_MS); // 5 seconds delay (JI)
    } 
    
    gpio_set_level(same_traffic[2], 0); // Turns the Green LED of the same_traffic OFF (JI)
    gpio_set_level(opposite_traffic[0], 0); // Turns the Red LED of the opposite_traffic OFF
    gpio_set_level(same_traffic[0], 1); // Turns the Red LED of the same_traffic ON.

    vTaskDelay(2000 / portTICK_PERIOD_MS); // 2 seconds delay (JI)

    gpio_set_level(same_traffic[0], 0); // Turns the Red LED of the same_traffic OFF.
   

}
    




