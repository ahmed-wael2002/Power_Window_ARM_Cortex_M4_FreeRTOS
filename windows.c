// External Modules 
#include "windows.h"
#include "int.h"
#include "motor.h"
#include "DIO.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"



////////// Global lock variable
// Variable should be inaccessible from other modules 
// static = private
static uint8 g_isLocked = 0;

void WINDOW_LockToggle(void){
	// Toggle Lock function
	g_isLocked ^= 1;
}

 /*
 Function to initialize necessary pins and modules for the power windows
 */
 void WINDOW_init(void){	 
    /**** Initializing Interrupt for Windows Lock switch *****/
    INT_init(DRIVER_LOCK_PORT, DRIVER_LOCK_PIN);
    INT_setCallBack(WINDOW_LockToggle);
	 
    /**** Initializing GPIO Input Pins for control buttons/switches ****/

    // Initializing driver control board 
    DIO_Init(DRIVER_UP_PORT, DRIVER_UP_PIN, PIN_INPUT);
    DIO_Init(DRIVER_DOWN_PORT, DRIVER_DOWN_PIN, PIN_INPUT);
    DIO_Init(DRIVER_ONE_TOUCH_PORT, DRIVER_ONE_TOUCH_PIN, PIN_INPUT);
    DIO_Init(DRIVER_LOCK_PORT, DRIVER_LOCK_PIN, PIN_INPUT);

    // Initializing passenger 1 control board
    DIO_Init(PASSENGER1_UP_PORT, PASSENGER1_UP_PIN, PIN_INPUT);
    DIO_Init(PASSENGER1_DOWN_PORT, PASSENGER1_DOWN_PIN, PIN_INPUT);
    DIO_Init(PASSENGER1_ONE_TOUCH_PORT, PASSENGER1_ONE_TOUCH_PIN, PIN_INPUT);

    #if (NUM_OF_WINDOWS == 4)
    
    // Initializing passenger 2 control board
    DIO_Init(PASSENGER2_UP_PORT, PASSENGER2_UP_PIN, PIN_INPUT);
    DIO_Init(PASSENGER2_DOWN_PORT, PASSENGER2_DOWN_PIN, PIN_INPUT);
    DIO_Init(PASSENGER2_ONE_TOUCH_PORT, PASSENGER2_ONE_TOUCH_PIN, PIN_INPUT);

    // Initializing passenger 3 control board
    DIO_Init(PASSENGER3_UP_PORT, PASSENGER3_UP_PIN, PIN_INPUT);
    DIO_Init(PASSENGER3_DOWN_PORT, PASSENGER3_DOWN_PIN, PIN_INPUT);
    DIO_Init(PASSENGER3_ONE_TOUCH_PORT, PASSENGER3_ONE_TOUCH_PIN, PIN_INPUT);

    #endif
 }



void WINDOW_Task(void* pvParameter){
    Window_type* window = (Window_type*)pvParameter;
    // Application
    while(1){
        // Up Button Code
        if ((window->id != DRIVER) && (g_isLocked == 1)) { /* Do Nothing*/ }
        else{
            /*============================ UP DIRECTION =============================*/
            if (IS_ON(window->up_port, window->up_pin) && (g_isLocked!=0)){
                // Debouncing Delay
                vTaskDelay(10 / portTICK_RATE_MS);
                while(IS_ON(window->up_port, window->up_pin) && (g_isLocked!=0) && IS_OFF(window->top_limit_port, window->top_limit_pin)){
                    DcMotor_Rotate(window->id % 2, ANTICLOCKWISE);
                }
            }

            /*============================ DOWN DIRECTION =============================*/
            else if (IS_ON(window->down_port, window->down_pin) && (g_isLocked!=0)){
                // Debouncing Delay
                vTaskDelay(10 / portTICK_RATE_MS);
                while(IS_ON(window->down_port, window->down_pin) && (g_isLocked!=0) && IS_OFF(window->bottom_limit_port, window->bottom_limit_pin)){
                    DcMotor_Rotate(window->id % 2, ANTICLOCKWISE);
                }
            }

            /*============================ ONE-TOUCH DIRECTION =============================*/
            else if (IS_ON(window->auto_port, window->auto_pin) && (g_isLocked!=0)){
                /******* Generic Task Creation for windows *******/
                // One-touch task has priority higher than normal tasks
                xTaskCreate(
                    ONE_TOUCH,					// Task Function
                    "One Touch",				// Debugging Task Name
                    120,									// Stack Depth
                    (void *)&window,			// We won't use Task Parameters
                    2, 										// Task Priority = configMAX_PRIORITIES - 1 = 55
                    NULL									// We don't need a task handle
                );							
            }
            /*============================ RESETTING MOTOR =============================*/
            DcMotor_Rotate(window->id % 2, OFF);
        }
    }
}
	


void ONE_TOUCH(void * pvParameters){
    Window_type * window = (Window_type*)pvParameters;

    // The window is currently closed -- triggering automatic open
    if(IS_ON(window->top_limit_port, window->top_limit_pin)){
        while(IS_OFF(window->bottom_limit_port, window->bottom_limit_pin)){
            DcMotor_Rotate(window->id%2, DOWN);
        }
    }
    // The window is fully or partially open -- triggering automatic close
    else{
        while(IS_OFF(window->top_limit_port, window->top_limit_pin)){
            DcMotor_Rotate(window->id%2, UP);
        }
    }
    // Stopping motor 
    DcMotor_Rotate(window->id%2, OFF);
    // Delete task for tasks queue and return to normal operations
    vTaskDelete(NULL);
}