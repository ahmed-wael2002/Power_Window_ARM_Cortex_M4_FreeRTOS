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
Window_type windows[NUM_OF_WINDOWS];

void WINDOW_LockToggle(void){
	// Toggle Lock function
	g_isLocked ^= 1;
}

 /*
 Function to initialize necessary pins and modules for the power windows
 */
 void WINDOW_init(void){	 
    // Windows Array Initialization
    windows[0].id = DRIVER;
    windows[0].up_port = DRIVER_UP_PORT;
    windows[0].up_pin = DRIVER_UP_PIN;
    windows[0].down_port = DRIVER_DOWN_PORT;
    windows[0].down_pin = DRIVER_DOWN_PIN;
    windows[0].top_limit_port = DRIVER_TOP_LIMIT_PORT;
    windows[0].top_limit_pin = DRIVER_TOP_LIMIT_PIN;
    windows[0].bottom_limit_port = DRIVER_BOTTOM_LIMIT_PORT;
    windows[0].bottom_limit_pin = DRIVER_BOTTOM_LIMIT_PIN;
    windows[0].jam_port = DRIVER_JAM_PORT;
    windows[0].jam_pin = DRIVER_JAM_PIN;

    // Window for the first passenger
    windows[1].id = PASSENGER1;
    windows[1].up_port = PASSENGER1_UP_PORT;
    windows[1].up_pin = PASSENGER1_UP_PIN;
    windows[1].down_port = PASSENGER1_DOWN_PORT;
    windows[1].down_pin = PASSENGER1_DOWN_PIN;
    windows[1].top_limit_port = PASSENGER1_TOP_LIMIT_PORT;
    windows[1].top_limit_pin = PASSENGER1_TOP_LIMIT_PIN;
    windows[1].bottom_limit_port = PASSENGER1_BOTTOM_LIMIT_PORT;
    windows[1].bottom_limit_pin = PASSENGER1_BOTTOM_LIMIT_PIN;
    windows[1].jam_port = PASSENGER1_JAM_PORT;
    windows[1].jam_pin = PASSENGER1_JAM_PIN;


    /**** Initializing Interrupt for Windows Lock switch *****/
    INT_init(DRIVER_LOCK_PORT, DRIVER_LOCK_PIN);
    INT_setCallBack(DRIVER_LOCK_PORT,WINDOW_LockToggle);
	 
    /**** Initializing GPIO Input Pins for control buttons/switches ****/

    // Initializing driver control board 
    DIO_Init(DRIVER_UP_PORT, DRIVER_UP_PIN, PIN_INPUT);
    DIO_Init(DRIVER_DOWN_PORT, DRIVER_DOWN_PIN, PIN_INPUT);
    DIO_Init(DRIVER_LOCK_PORT, DRIVER_LOCK_PIN, PIN_INPUT);
    DIO_Init(DRIVER_JAM_PORT, DRIVER_JAM_PIN, PIN_INPUT); 

    // Initializing passenger 1 control board
    DIO_Init(PASSENGER1_UP_PORT, PASSENGER1_UP_PIN, PIN_INPUT);
    DIO_Init(PASSENGER1_DOWN_PORT, PASSENGER1_DOWN_PIN, PIN_INPUT);
    DIO_Init(PASSENGER1_JAM_PORT, PASSENGER1_JAM_PIN, PIN_INPUT);

    #if (NUM_OF_WINDOWS == 4)
    
    // Initializing passenger 2 control board
    windows[2].id = PASSENGER2;
    windows[2].up_port = PASSENGER2_UP_PORT;
    windows[2].up_pin = PASSENGER2_UP_PIN;
    windows[2].down_port = PASSENGER2_DOWN_PORT;
    windows[2].down_pin = PASSENGER2_DOWN_PIN;
    windows[2].top_limit_port = PASSENGER2_TOP_LIMIT_PORT;
    windows[2].top_limit_pin = PASSENGER2_TOP_LIMIT_PIN;
    windows[2].bottom_limit_port = PASSENGER2_BOTTOM_LIMIT_PORT;
    windows[2].bottom_limit_pin = PASSENGER2_BOTTOM_LIMIT_PIN;
    windows[2].jam_port = PASSENGER2_JAM_PORT;
    windows[2].jam_pin = PASSENGER2_JAM_PIN;

    // Initializing passenger 3 control board
    windows[3].id = PASSENGER3;
    windows[3].up_port = PASSENGER3_UP_PORT;
    windows[3].up_pin = PASSENGER3_UP_PIN;
    windows[3].down_port = PASSENGER3_DOWN_PORT;
    windows[3].down_pin = PASSENGER3_DOWN_PIN;
    windows[3].top_limit_port = PASSENGER3_TOP_LIMIT_PORT;
    windows[3].top_limit_pin = PASSENGER3_TOP_LIMIT_PIN;
    windows[3].bottom_limit_port = PASSENGER3_BOTTOM_LIMIT_PORT;
    windows[3].bottom_limit_pin = PASSENGER3_BOTTOM_LIMIT_PIN;
    windows[3].jam_port = PASSENGER3_JAM_PORT;
    windows[3].jam_pin = PASSENGER3_JAM_PIN;

    DIO_Init(PASSENGER2_UP_PORT, PASSENGER2_UP_PIN, PIN_INPUT);
    DIO_Init(PASSENGER2_DOWN_PORT, PASSENGER2_DOWN_PIN, PIN_INPUT);
    DIO_Init(PASSENGER2_JAM_PORT, PASSENGER2_JAM_PIN, PIN_INPUT);

    // Initializing passenger 3 control board
    DIO_Init(PASSENGER3_UP_PORT, PASSENGER3_UP_PIN, PIN_INPUT);
    DIO_Init(PASSENGER3_DOWN_PORT, PASSENGER3_DOWN_PIN, PIN_INPUT);
    DIO_Init(PASSENGER3_JAM_PORT, PASSENGER3_JAM_PIN, PIN_INPUT);

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
                vTaskDelay(850 / portTICK_RATE_MS);
                // Manual Control -- Up Button
                if (IS_ON(window->up_port, window->up_pin) && (g_isLocked!=0)){
                    DcMotor_Rotate(window->id % 2, UP);
                    while(IS_ON(window->up_port, window->up_pin) && (g_isLocked!=0));
                }
                // One-touch Control -- Up Button
                else{
                    // Keep rotating motor until window reaches top limit switch
                    while(IS_OFF(window->top_limit_port, window->top_limit_pin) && (g_isLocked!=0)){
                        DcMotor_Rotate(window->id % 2, UP);
                        // Jam Detection 
                        if (IS_ON(window->jam_port, window->jam_pin)){
                            DcMotor_Rotate(window->id % 2, DOWN);
                            vTaskDelay(500 / portTICK_RATE_MS);
                            break;
                        }
                    }
                }

            }

            /*============================ DOWN DIRECTION =============================*/
            else if (IS_ON(window->down_port, window->down_pin) && (g_isLocked!=0)){
                // Debouncing Delay
                vTaskDelay(850 / portTICK_RATE_MS);
                // Manual Control -- Down Button
                if (IS_ON(window->down_port, window->down_pin) && (g_isLocked!=0)){
                    DcMotor_Rotate(window->id % 2, DOWN);
                    while(IS_ON(window->down_port, window->down_pin) && (g_isLocked!=0));
                }
                // One-touch Control -- Down Button
                else{
                    // Keep rotating motor until window reaches top limit switch
                    while(IS_OFF(window->bottom_limit_port, window->bottom_limit_pin) && (g_isLocked!=0)){
                        DcMotor_Rotate(window->id % 2, DOWN);
                    }
                }
            }

            /*============================ RESETTING MOTOR =============================*/
            DcMotor_Rotate(window->id % 2, OFF);
        }
    }
}
