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


int main( void )
{
	
	WINDOW_init();	
	
	for (uint8 i=0; i<NUM_OF_WINDOWS; i++){	
		xTaskCreate(
			WINDOW_PassengerTask,	
			"Window Task",				
			300,									
			(void *)&windows[i],	
			1, 									
			NULL								
		);	
	}
		
	vTaskStartScheduler();
	
	
}


/* Idle hook functions MUST be called vApplicationIdleHook(), take no parameters,
and return void. */
void vApplicationIdleHook( void )
{
	// The hook places the system in low power mode
	__ASM("wfi");
}