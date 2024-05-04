/*
 *  windows.h
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

 #define NUM_OF_WINDOWS     2       // Define the number of windows in the system (4: Sedan - 2: Coupe)
 #define DRIVER_LOCATION    0       // Define the driver seat with the location (UK standard - UN standard) [0-NUM_OF_WINDOWS - 1]
 
 #define DRIVER_UP_PORT                 PORTB_ID
 #define DRIVER_UP_PIN                  PIN0_ID
 #define DRIVER_DOWN_PORT               PORTB_ID
 #define DRIVER_DOWN_PIN                PIN0_ID
 #define DRIVER_ONE_TOUCH_PORT          PORTB_ID
 #define DRIVER_ONE_TOUCH_PIN           PIN0_ID
 #define DRIVER_LOCK_PORT               PORTD_ID
 #define DRIVER_LOCK_PIN                PIN0_ID
 #define DRIVER_TOP_LIMIT_PORT          PORTB_ID
 #define DRIVER_TOP_LIMIT_PIN           PIN0_ID
 #define DRIVER_BOTTOM_LIMIT_PORT       PORTB_ID
 #define DRIVER_BOTTOM_LIMIT_PIN        PIN0_ID
 #define DRIVER_JAM_PORT                PORTA_ID
 #define DRIVER_JAM_PIN                 PIN0_ID
 

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
 #define PASSENGER1_JAM_PORT            PORTA_ID
 #define PASSENGER1_JAM_PIN             PIN0_ID
 
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
 #define PASSENGER2_JAM_PORT            PORTA_ID
 #define PASSENGER2_JAM_PIN             PIN0_ID
 
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
 #define PASSENGER3_JAM_PORT            PORTA_ID
 #define PASSENGER3_JAM_PIN             PIN0_ID

 #endif

 /*=============== Operation Abstraction ================*/
 #define SWITCH_ON            LOGIC_LOW
 #define SWITCH_OFF           LOGIC_HIGH
 #define UP                   ANTICLOCKWISE
 #define DOWN                 CLOCKWISE
 #define IS_ON(port, pin)    (DIO_readPin(port, pin) == SWITCH_ON)
 #define IS_OFF(port, pin)   (DIO_readPin(port, pin) == SWITCH_OFF)

/******************************************
 * 			 TYPES DEFINITIONS
 ******************************************/
typedef enum{
   DRIVER,
   PASSENGER1,
   PASSENGER2,
   PASSENGER3
}Window_ID;

/*
   Window_type task parameters -- window object attributes 
   -----------------------------------------------------------------------------------------------------------------------------
   id:                                     Identify which window to control {DRIVER=0, PASSENGER1=1, PASSENGER2=2, PASSENGER3=3}
   up_port, up_pin:                        Input GPIO coming from UP direction buttons/switches
   down_port, down_pin:                    Input GPIO coming from DOWN direction buttons/switches
   auto_port, auto_pin:                    Input GPIO coming from ONE TOUCH AUTO OPEN/CLOSE buttons/switches 
   top_limit_port, top_limit_pin:          Input GPIO coming from limit switches placed on the top of the window frame
   bottom_limit_port, bottom_limit_pin:    Input GPIO coming from limit switches placed on the bottom of the window frame
*/
 typedef struct{
    Window_ID id;
    uint8 up_port;
    uint8 up_pin;
    uint8 down_port;
    uint8 down_pin;
    uint8 auto_port;
    uint8 auto_pin;
    uint8 top_limit_port;
    uint8 top_limit_pin;
    uint8 bottom_limit_port;
    uint8 bottom_limit_pin;
    uint8 jam_port;
    uint8 jam_pin;
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

#endif