/*
 * motor.c
 *
 *  Created on: Apr 24, 2024
 *      Author: Wael
 */

 // This is a simple implementation for the motor driver
 // The Power Window project requirements only need constant rotation speed so PWM control will not be implemented
 // Such reduction is necessary to avoid unnecessary usage of timers and PWM controls
 // Ensure jumper is placed on the Enable pins to allow max speed operation

/****** PWM control can be added in future implementations calling for variable speed ******/

#include "motor.h"


/*
 * This function initializes the DC motors by setting up the direction and state of the motor pins.
 * It also ensures that all the motor pins are initially set to the off state.
 */
void Motor_init(void){
	INITIALIZE_MOTOR(1);
	INITIALIZE_MOTOR(2);
	
	#if (NUM_OF_MOTORS == 4)
	INITIALIZE_MOTOR(3);
	INITIALIZE_MOTOR(4);
	#endif
}

/*
 * This function rotates the DC motor in the specified direction.
 * It takes two parameters: the motor ID and the direction.
 * The motor ID can be either MOTOR1 or MOTOR2.
 * The direction can be OFF, ANTICLOCKWISE, or CLOCKWISE.
 */
void Motor_Rotate(Motor_ID motor, Motor_Direction direction){
	switch(motor){
		case MOTOR1:
			if (direction == OFF){
				DIO_writePin(MOTOR1_IN1_PORT, MOTOR1_IN1_PIN, LOGIC_LOW);
				DIO_writePin(MOTOR1_IN2_PORT, MOTOR1_IN2_PIN, LOGIC_LOW);
			}
			else if (direction == ANTICLOCKWISE){
				DIO_writePin(MOTOR1_IN1_PORT, MOTOR1_IN1_PIN, LOGIC_LOW);
				DIO_writePin(MOTOR1_IN2_PORT, MOTOR1_IN2_PIN, LOGIC_HIGH);
			}
			else if (direction == CLOCKWISE){
				DIO_writePin(MOTOR1_IN1_PORT, MOTOR1_IN1_PIN, LOGIC_HIGH);
				DIO_writePin(MOTOR1_IN2_PORT, MOTOR1_IN2_PIN, LOGIC_LOW);
			}
		break;

		case MOTOR2:
			if (direction == OFF){
				DIO_writePin(MOTOR2_IN1_PORT, MOTOR2_IN1_PIN, LOGIC_LOW);
				DIO_writePin(MOTOR2_IN2_PORT, MOTOR2_IN2_PIN, LOGIC_LOW);
			}
			else if (direction == ANTICLOCKWISE){
				DIO_writePin(MOTOR2_IN1_PORT, MOTOR2_IN1_PIN, LOGIC_LOW);
				DIO_writePin(MOTOR2_IN2_PORT, MOTOR2_IN2_PIN, LOGIC_HIGH);
			}
			else if (direction == CLOCKWISE){
				DIO_writePin(MOTOR2_IN1_PORT, MOTOR2_IN1_PIN, LOGIC_HIGH);
				DIO_writePin(MOTOR2_IN2_PORT, MOTOR2_IN2_PIN, LOGIC_LOW);
			}
		break;

		#if (NUM_OF_MOTORS == 4)
		case MOTOR3:
			if (direction == OFF){
				DIO_writePin(MOTOR3_IN1_PORT, MOTOR3_IN1_PIN, LOGIC_LOW);
				DIO_writePin(MOTOR3_IN2_PORT, MOTOR3_IN2_PIN, LOGIC_LOW);
			}
			else if (direction == ANTICLOCKWISE){
				DIO_writePin(MOTOR3_IN1_PORT, MOTOR3_IN1_PIN, LOGIC_LOW);
				DIO_writePin(MOTOR3_IN2_PORT, MOTOR3_IN2_PIN, LOGIC_HIGH);
			}
			else if (direction == CLOCKWISE){
				DIO_writePin(MOTOR3_IN1_PORT, MOTOR3_IN1_PIN, LOGIC_HIGH);
				DIO_writePin(MOTOR3_IN2_PORT, MOTOR3_IN2_PIN, LOGIC_LOW);
			}
		break;

		case MOTOR4:
			if (direction == OFF){
				DIO_writePin(MOTOR4_IN1_PORT, MOTOR4_IN1_PIN, LOGIC_LOW);
				DIO_writePin(MOTOR4_IN2_PORT, MOTOR4_IN2_PIN, LOGIC_LOW);
			}
			else if (direction == ANTICLOCKWISE){
				DIO_writePin(MOTOR4_IN1_PORT, MOTOR4_IN1_PIN, LOGIC_LOW);
				DIO_writePin(MOTOR4_IN2_PORT, MOTOR4_IN2_PIN, LOGIC_HIGH);
			}
			else if (direction == CLOCKWISE){
				DIO_writePin(MOTOR4_IN1_PORT, MOTOR4_IN1_PIN, LOGIC_HIGH);
				DIO_writePin(MOTOR4_IN2_PORT, MOTOR4_IN2_PIN, LOGIC_LOW);
			}
		break;

		#endif 

	}
}
