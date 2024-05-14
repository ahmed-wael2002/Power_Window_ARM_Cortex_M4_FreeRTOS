// Including Windows Application APIs
#include "windows.h"
// FreeRTOS Libraries 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/*************************************************************************************************************
  			                                GLOBAL VARIABLES
 *************************************************************************************************************/
// Variables are externed from module -- Check for documentation 
extern Window_type windows[NUM_OF_WINDOWS];
extern QueueHandle_t xCommandQueue;
extern xSemaphoreHandle xBinarySemaphore;


int main( void )
{
	// Initializing Windows GPIO 
	WINDOW_init();
	// Initializing commands queue	
	xCommandQueue = xQueueCreate(20, sizeof(uint8_t));
	// Initializing binary semaphore
	vSemaphoreCreateBinary(xBinarySemaphore);

	if (xBinarySemaphore != NULL){
		// Semaphore is created successfully 
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

		// Create Motor Task
		xTaskCreate(
			WINDOW_MotorTask,	
			"Window Task",				
			300,									
			NULL,	
			1, 									
			NULL								
		);	

		// Create jamming semaphore task of higher priority
		xTaskCreate(
			WINDOW_JammingSemaphoreTask,	
			"Window Task",				
			300,									
			NULL,	
			3, 									
			NULL								
		);

		// Start the scheduler and run tasks 
		vTaskStartScheduler();	
	}
	/* If all is well we will never reach here as the scheduler will noe
	be running the tasks. If we reach here then it is likely that there was
	insufficient heap memory available for a resource to be created */
	for(;;);
}


/* Idle hook functions MUST be called vApplicationIdleHook(), take no parameters,
and return void. */
void vApplicationIdleHook( void )
{
	// The hook places the system in low power mode
	__ASM("wfi");
}