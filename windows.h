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

/*************************************************************************************************************
 *                                            MACROS DEFINITION
 *************************************************************************************************************/

 /*=============== SYSTEM STATIC CONFIGURATIONS ================*/

#define NUM_OF_WINDOWS     2       // Define the number of windows in the system (4: Sedan - 2: Coupe)
#define USED_MOTOR_TYPE    DC      // Define the type of motor used for windows movement      

/*=============== OPERATIONAL MACROS ================*/
#define SWITCH_ON            LOGIC_LOW
#define SWITCH_OFF           LOGIC_HIGH
#define UP                   ANTICLOCKWISE
#define DOWN                 CLOCKWISE
#define IS_ON(port, pin)    (DIO_readPin(port, pin) == SWITCH_ON)
#define IS_OFF(port, pin)   (DIO_readPin(port, pin) == SWITCH_OFF)

/*=============== DRIVER ================*/
// Driver UP Button
#define DRIVER_UP_PORT                 	PORTE_ID
#define DRIVER_UP_PIN                  	PIN5_ID
// Driver DOWN Button
#define DRIVER_DOWN_PORT               	PORTE_ID
#define DRIVER_DOWN_PIN                 PIN4_ID
// Driver Control for passenger1
#define DRIVER_PASSENGER1_UP_PORT       PORTA_ID
#define DRIVER_PASSENGER1_UP_PIN        PIN5_ID
#define DRIVER_PASSENGER1_DOWN_PORT     PORTB_ID
#define DRIVER_PASSENGER1_DOWN_PIN      PIN4_ID

#if (NUM_OF_WINDOWS == 4)
// Driver Control for passenger2
#define DRIVER_PASSENGER2_UP_PORT       PORTB_ID
#define DRIVER_PASSENGER2_UP_PIN        PIN0_ID
#define DRIVER_PASSENGER2_DOWN_PORT     PORTB_ID
#define DRIVER_PASSENGER2_DOWN_PIN      PIN0_ID
// Driver Control for passenger3
#define DRIVER_PASSENGER3_UP_PORT       PORTB_ID
#define DRIVER_PASSENGER3_UP_PIN        PIN0_ID
#define DRIVER_PASSENGER3_DOWN_PORT     PORTB_ID
#define DRIVER_PASSENGER3_DOWN_PIN      PIN0_ID
#endif
// Driver Lock Button
#define DRIVER_LOCK_PORT                PORTF_ID
#define DRIVER_LOCK_PIN                 PIN0_ID
// Driver Top Limit Switch
#define DRIVER_TOP_LIMIT_PORT           PORTA_ID
#define DRIVER_TOP_LIMIT_PIN            PIN6_ID
// Driver Bottom Limit Switch
#define DRIVER_BOTTOM_LIMIT_PORT        PORTA_ID
#define DRIVER_BOTTOM_LIMIT_PIN         PIN5_ID
// Driver Jam Sensor 
#define DRIVER_JAM_PORT                 PORTB_ID
#define DRIVER_JAM_PIN                  PIN4_ID

/*=============== PASSENGER 1 ================*/
// Passenger UP Button
#define PASSENGER1_UP_PORT             PORTD_ID
#define PASSENGER1_UP_PIN              PIN7_ID
// Passenger DOWN Button
#define PASSENGER1_DOWN_PORT           PORTD_ID
#define PASSENGER1_DOWN_PIN            PIN6_ID
// Passenger Top Limit Switch
#define PASSENGER1_TOP_LIMIT_PORT      PORTC_ID
#define PASSENGER1_TOP_LIMIT_PIN       PIN7_ID
// Passenger Bottom Limit Switch
#define PASSENGER1_BOTTOM_LIMIT_PORT   PORTC_ID
#define PASSENGER1_BOTTOM_LIMIT_PIN    PIN6_ID
// Passenger Jam Sensor
#define PASSENGER1_JAM_PORT            PORTC_ID
#define PASSENGER1_JAM_PIN             PIN5_ID

#if (NUM_OF_WINDOWS == 4)
/*=============== PASSENGER 2 ================*/

// Passenger UP Button
#define PASSENGER2_UP_PORT             PORTB_ID
#define PASSENGER2_UP_PIN              PIN0_ID
// Passenger DOWN Button
#define PASSENGER2_DOWN_PORT           PORTB_ID
#define PASSENGER2_DOWN_PIN            PIN0_ID
// Passenger Top Limit Switch
#define PASSENGER2_TOP_LIMIT_PORT      PORTB_ID
#define PASSENGER2_TOP_LIMIT_PIN       PIN0_ID
// Passenger Bottom Limit Switch
#define PASSENGER2_BOTTOM_LIMIT_PORT   PORTB_ID
#define PASSENGER2_BOTTOM_LIMIT_PIN    PIN0_ID
// Passenger Jam Sensor
#define PASSENGER2_JAM_PORT            PORTA_ID
#define PASSENGER2_JAM_PIN             PIN0_ID

/*=============== PASSENGER 3 ================*/

// Passenger UP Button
#define PASSENGER3_UP_PORT             PORTB_ID
#define PASSENGER3_UP_PIN              PIN0_ID
// Passenger DOWN Button
#define PASSENGER3_DOWN_PORT           PORTB_ID
#define PASSENGER3_DOWN_PIN            PIN0_ID
// Passenger Top Limit Switch
#define PASSENGER3_TOP_LIMIT_PORT      PORTB_ID
#define PASSENGER3_TOP_LIMIT_PIN       PIN0_ID
// Passenger Bottom Limit Switch
#define PASSENGER3_BOTTOM_LIMIT_PORT   PORTB_ID
#define PASSENGER3_BOTTOM_LIMIT_PIN    PIN0_ID
// Passenger Jam Sensor
#define PASSENGER3_JAM_PORT            PORTA_ID
#define PASSENGER3_JAM_PIN             PIN0_ID

#endif


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
   // Up button
   uint8 up_port;
   uint8 up_pin;
   // Down Button
   uint8 down_port;
   uint8 down_pin;
   // Top Limit Switch
   uint8 top_limit_port;
   uint8 top_limit_pin;
   // Bottom Limit Switch
   uint8 bottom_limit_port;
   uint8 bottom_limit_pin;
   // Jam Sensor
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
Specific Task for handling Driver Commands
--- FreeRTOS specific definition
*/
void WINDOW_DriverTask(void* pvParameter);

/*
Generic Task definition for power window 
--- FreeRTOS specific definition
*/
void WINDOW_PassengerTask(void* pvParameter);

#endif