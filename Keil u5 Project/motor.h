/*
 * motor.h
 *
 *  Created on: Apr 24, 2024
 *      Author: Ahmed Wael
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "types.h"
#include "DIO.h"

/* 
This is header file declares a set of functions to control 2 DC motors 
by interfacing with L298N Motor Driver Module which allows simultaneous control of 2 motors.
The driver module is controlled by 3 pins:
1- Enable 1 pin: to enable the first motor
2- Input 1,2 pins: Control the rotation direction of the first motor
3- Enable 2 pin: to enable the second motor
4- Input 3,4 pins: Control the rotation direction of the second motor
*/

/*************************************************************************************************************
  			                                MACROS DEFINITIONS
 *************************************************************************************************************/

/*=============== SYSTEM STATIC CONFIGURATIONS ================*/
#define NUM_OF_MOTORS		2
#define TYPE_OF_MOTORS		DC


/*=============== OPERATIONAL MACROS ================*/





/*=============== MOTORS GPIO ================*/

/* Defining Enable pin for Motor
 * In case of a variable speed driver:
 * 	This pin should be connected to timer PWM output pin
 */

// Controlling First Motor
#define MOTOR1_EN_PORT 		PORTA_ID
#define MOTOR1_EN_PIN 		PIN4_ID
#define MOTOR1_IN1_PORT 	PORTA_ID
#define MOTOR1_IN1_PIN 		PIN2_ID
#define MOTOR1_IN2_PORT 	PORTA_ID
#define MOTOR1_IN2_PIN 		PIN3_ID

// Controlling Second Motor
#define MOTOR2_EN_PORT 		PORTD_ID
#define MOTOR2_EN_PIN 		PIN6_ID
#define MOTOR2_IN1_PORT		PORTA_ID
#define MOTOR2_IN1_PIN		PIN4_ID
#define MOTOR2_IN2_PORT		PORTB_ID
#define MOTOR2_IN2_PIN		PIN6_ID

#if (NUM_OF_MOTORS == 4)
// Controlling Third Motor
#define MOTOR3_EN_PORT 		PORTB_ID
#define MOTOR3_EN_PIN 		PIN3_ID
#define MOTOR3_IN1_PORT 	PORTB_ID
#define MOTOR3_IN1_PIN 		PIN0_ID
#define MOTOR3_IN2_PORT 	PORTB_ID
#define MOTOR3_IN2_PIN 		PIN1_ID

// Controlling Fourth Motor
#define MOTOR4_EN_PORT 		PORTB_ID
#define MOTOR4_EN_PIN 		PIN4_ID
#define MOTOR4_IN1_PORT		PORTD_ID
#define MOTOR4_IN1_PIN		PIN0_ID
#define MOTOR4_IN2_PORT		PORTD_ID
#define MOTOR4_IN2_PIN		PIN1_ID
#endif

/******************************************
 * 			 TYPES DEFINITIONS
 ******************************************/
/*
 * This enum is used to define the direction of rotation of the motor
 * 1- OFF			: IN1=0, IN2=0
 * 2- ANTICLOCKWISE	: IN1=0, IN2=1
 * 3- CLOCKWISE		: IN1=1, IN2=0
 */
typedef enum{
	OFF,			/* 00 */
	ANTICLOCKWISE, 	/* 01 */
	CLOCKWISE		/* 10 */
}Motor_Direction;

typedef enum{
	MOTOR1,
	MOTOR2,
	MOTOR3,
	MOTOR4
}Motor_ID;


/******************************************
 * 			FUNCTION PROTOTYPES
 ******************************************/
void Motor_init(void);
void Motor_Rotate(Motor_ID motor, Motor_Direction direction);


#endif /* MOTOR_H_ */