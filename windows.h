/*
 * windows.h
 *
 *  Created on: Apr 25, 2024
 *      Author: Ahmed Wael
 */

#ifndef WINDOWS_H_
#define WINDOWS_H_

#include "types.h"
#include "motor.h"
#include "DIO.h"

/* 
This is the header file for the application layer of Power Windows Application
*/

/******************************************
 * 			MACROS DEFINITIONS
 ******************************************/
 /*=============== GPIO ================*/

 #define NUM_OF_WINDOWS     4       // Define the number of windows in the system (4: Sedan - 2: Coupe)
 #define DRIVER_LOCATION    0       // Define the driver seat with the location (UK standard - UN standard) [0-NUM_OF_WINDOWS - 1]
 
 #define DRIVER_UP_PORT                 PORTB_ID
 #define DRIVER_UP_PIN                  PIN0_ID
 #define DRIVER_DOWN_PORT               PORTB_ID
 #define DRIVER_DOWN_PIN                PIN0_ID
 #define DRIVER_ONE_TOUCH_PORT          PORTB_ID
 #define DRIVER_ONE_TOUCH_PIN           PIN0_ID
 #define DRIVER_LOCK_PORT               PORTB_ID
 #define DRIVER_LOCK_PIN                PIN0_ID
 #define DRIVER_TOP_LIMIT_PORT          PORTB_ID
 #define DRIVER_TOP_LIMIT_PIN           PIN0_ID
 #define DRIVER_BOTTOM_LIMIT_PORT       PORTB_ID
 #define DRIVER_BOTTOM_LIMIT_PIN        PIN0_ID
 

 #define PASSENGER1_UP_PORT             PORTB_ID
 #define PASSENGER1_UP_PIN              PIN0_ID
 #define PASSENGER1_DOWN_PORT           PORTB_ID
 #define PASSENGER1_DOWN_PIN            PIN0_ID
 #define PASSENGER1_ONE_TOUCH_PORT      PORTB_ID
 #define PASSENGER1_ONE_TOUCH_PIN       PIN0_ID
 #define PASSENGER1_TOP_LIMIT_PORT      PORTB_ID
 #define PASSENGER1_TOP_LIMIT_PIN       PIN0_ID
 #define PASSENGER1_BOTTOM_LIMIT_PORT   PORTB_ID
 #define PASSENGER1_BOTTOM_LIMIT_PIN    PIN0_ID
 
 #if (NUM_OF_WINDOWS == 4)

 #define PASSENGER2_UP_PORT             PORTB_ID
 #define PASSENGER2_UP_PIN              PIN0_ID
 #define PASSENGER2_DOWN_PORT           PORTB_ID
 #define PASSENGER2_DOWN_PIN            PIN0_ID
 #define PASSENGER2_ONE_TOUCH_PORT      PORTB_ID
 #define PASSENGER2_ONE_TOUCH_PIN       PIN0_ID
 #define PASSENGER2_TOP_LIMIT_PORT      PORTB_ID
 #define PASSENGER2_TOP_LIMIT_PIN       PIN0_ID
 #define PASSENGER2_BOTTOM_LIMIT_PORT   PORTB_ID
 #define PASSENGER2_BOTTOM_LIMIT_PIN    PIN0_ID
 
 #define PASSENGER3_UP_PORT             PORTB_ID
 #define PASSENGER3_UP_PIN              PIN0_ID
 #define PASSENGER3_DOWN_PORT           PORTB_ID
 #define PASSENGER3_DOWN_PIN            PIN0_ID
 #define PASSENGER3_ONE_TOUCH_PORT      PORTB_ID
 #define PASSENGER3_ONE_TOUCH_PIN       PIN0_ID
 #define PASSENGER3_TOP_LIMIT_PORT      PORTB_ID
 #define PASSENGER3_TOP_LIMIT_PIN       PIN0_ID
 #define PASSENGER3_BOTTOM_LIMIT_PORT   PORTB_ID
 #define PASSENGER3_BOTTOM_LIMIT_PIN    PIN0_ID

 #endif

 /*=============== Operation Abstraction ================*/
 #define SWITCH_ON  LOGIC_LOW
 #define SWITCH_OFF LOGIC_HIGH
 #define IS_ON(parameters)   (DIO_readPin(parameters) == SWITCH_ON)
 #define IS_OFF(parameters)  (DIO_readPin(parameters) == SWITCH_OFF)

 #define PASSENGER_UP_ACTION(passenger_id)        (PASSENGER##passenger_id##_UP_PORT,PASSENGER##passenger_id##_UP_PIN)
 #define PASSENGER_DOWN_ACTION(passenger_id)      (PASSENGER##passenger_id##_DOWN_PORT,PASSENGER##passenger_id##_DOWN_PIN)
 #define PASSENGER_ONE_TOUCH_ACTION(passenger_id) (PASSENGER##passenger_id##_ONE_TOUCH_PORT,PASSENGER##passenger_id##_ONE_TOUCH_PIN)
 #define REACHED_TOP(passenger_id)                (IS_ON(PASSENGER##passenger_id##_TOP_LIMIT_PORT, PASSENGER##passenger_id##_TOP_LIMIT_PIN))
 #define REACHED_BOTTOM(passenger_id)             (IS_ON(PASSENGER##passenger_id##_BOTTOM_LIMIT_PORT, PASSENGER##passenger_id##_BOTTOM_LIMIT_PIN))

/******************************************
 * 			 TYPES DEFINITIONS
 ******************************************/
 typedef enum{
    DRIVER,
    PASSENGER1,
    PASSENGER2,
    PASSENGER3
 }Window_ID;
 
 typedef struct{
    Window_ID id
 }Window_type;

/******************************************
 * 			FUNCTION PROTOTYPES
 ******************************************/
 /*
 Function to initialize necessary pins and modules for the power windows
 */
 void WINDOW_init(void);

 /*
 Generic Task definition for power window 
 --- FreeRTOS specific definition
 */
 void WINDOW_Task(void* pvParameter);

 /*
 Auxilary function for window movement
 */
 void WINDOW_move(Window_type * window);

#endif