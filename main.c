#include "windows.h"
// FreeRTOS Libraries 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

int main( void )
{
	// Initializing Windows using Init function 
	WINDOW_init();
	// Array of windows defined in the windows module 
	extern Window_type windows[NUM_OF_WINDOWS];
	
	// Instantiating tasks for windows 
	for (uint8 i=0; i<NUM_OF_WINDOWS; i++){
			/******* Generic Task Creation for windows *******/
			xTaskCreate(
					WINDOW_Task,					// Task Function
					"Window Task",				// Debugging Task Name
					120,									// Stack Depth
					(void *)&windows[i],	// Parameters are stored as an array of structs holding parameters
					1, 										// Task Priority = 1
					NULL									// We don't need a task handle
			);		
	}

}



/* Idle hook functions MUST be called vApplicationIdleHook(), take no parameters,
and return void. */
void vApplicationIdleHook( void )
{
	// The hook places the system in low power mode
	__ASM("wfi");
}