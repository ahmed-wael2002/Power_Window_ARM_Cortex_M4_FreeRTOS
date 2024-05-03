#include "windows.h"
// FreeRTOS Libraries 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

int main( void )
{
		// Creating an array of windows 
		Window_type wins[NUM_OF_WINDOWS] = {0};
		wins[0].id = DRIVER;
		wins[1].id = PASSENGER1;
		#if (NUM_OF_WINDOWS == 4)
		wins[2].id = PASSENGER2;
		wins[3].id = PASSENGER3;
		#endif
		
		// Instantiating tasks for windows 
		for (uint8 i=0; i<NUM_OF_WINDOWS; i++){
				/******* Generic Task Creation for windows *******/
				xTaskCreate(
					WINDOW_Task,					// Task Function
					"Window Task",				// Debugging Task Name
					120,									// Stack Depth
					(void *)&wins[i],			// We won't use Task Parameters
					1, 										// Task Priority = configMAX_PRIORITIES - 1 = 55
					NULL									// We don't need a task handle
				);		
		}

}