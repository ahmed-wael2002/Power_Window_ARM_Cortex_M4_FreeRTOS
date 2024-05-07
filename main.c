#include "windows.h"
#include "led.h"
#include "delay.h"
#include "motor.h"
#include "DIO.h"
// FreeRTOS Libraries 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

extern Window_type windows[NUM_OF_WINDOWS];
extern QueueHandle_t xCommandQueue;

int main( void )
{
	// Initializing Windows GPIO 
	WINDOW_init();
	// Initializing commands queue	
	xCommandQueue = xQueueCreate(20, sizeof(uint8_t));

	for (uint8 i=0; i<NUM_OF_WINDOWS; i++){	
		// Create Passenger Task (PASSENGER1, PASSENGER2, PASSENGER3	
		xTaskCreate(
			WINDOW_PassengerTask,	
			"Window Task",				
			300,									
			(void *)&windows[i],	
			1, 									
			NULL								
		);
	}

	xTaskCreate(
		WINDOW_MotorTask,	
		"Window Task",				
		300,									
		NULL,	
		1, 									
		NULL								
	);	
		
	vTaskStartScheduler();	
}


/* Idle hook functions MUST be called vApplicationIdleHook(), take no parameters,
and return void. */
void vApplicationIdleHook( void )
{
	// The hook places the system in low power mode
	__ASM("wfi");
}