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
#include "DIO.h"


/*
 * This function initializes the DC motors by setting up the direction and state of the motor pins.
 * It also ensures that all the motor pins are initially set to the off state.
 */
void DcMotor_init(void){
	/* setup direction of the first motor pins to be OUTPUT */
	DIO_Init(MOTOR1_EN_PORT, MOTOR1_EN_PIN, PIN_OUTPUT);
	DIO_Init(MOTOR1_IN1_PORT, MOTOR1_IN1_PIN, PIN_OUTPUT);
	DIO_Init(MOTOR1_IN2_PORT, MOTOR1_IN2_PIN, PIN_OUTPUT);

	/* setup direction of the second motor pins to be OUTPUT */
	DIO_Init(MOTOR2_EN_PORT, MOTOR2_EN_PIN, PIN_OUTPUT);
	DIO_Init(MOTOR2_IN1_PORT, MOTOR2_IN1_PIN, PIN_OUTPUT);
	DIO_Init(MOTOR2_IN2_PORT, MOTOR2_IN2_PIN, PIN_OUTPUT);

	/* Initializing all pins to be off */
	DIO_writePin(MOTOR1_EN_PORT, MOTOR1_EN_PIN, LOGIC_LOW);
	DIO_writePin(MOTOR1_IN1_PORT, MOTOR1_IN1_PIN, LOGIC_LOW);
	DIO_writePin(MOTOR1_IN2_PORT, MOTOR1_IN2_PIN, LOGIC_LOW);
	DIO_writePin(MOTOR2_EN_PORT, MOTOR2_EN_PIN, LOGIC_LOW);
	DIO_writePin(MOTOR2_IN1_PORT, MOTOR2_IN1_PIN, LOGIC_LOW);
	DIO_writePin(MOTOR2_IN2_PORT, MOTOR2_IN2_PIN, LOGIC_LOW);
}

/*
 * This function rotates the DC motor in the specified direction.
 * It takes two parameters: the motor ID and the direction.
 * The motor ID can be either MOTOR1 or MOTOR2.
 * The direction can be OFF, ANTICLOCKWISE, or CLOCKWISE.
 */
void DcMotor_Rotate(DcMotor_ID motor, DcMotor_Direction direction){
	if (motor == MOTOR1){
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
	}
	else if (motor == MOTOR2){
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
	}
}
