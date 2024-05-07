// External Modules 
#include "windows.h"
#include "int.h"
#include "motor.h"
#include "DIO.h"
// FreeRTOS Modules
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"


/*************************************************************************************************************
  			                                GLOBAL VARIABLES
 *************************************************************************************************************/
static volatile uint8 g_isLocked = 0;
Window_type windows[NUM_OF_WINDOWS];
QueueHandle_t xCommandQueue;


/*************************************************************************************************************
  			                                FUNCTION DEFINITIONS
 *************************************************************************************************************/
/*
    Interrupt Service Routine -- Fucntion to be executed on ON/OFF Switch interrupt
*/
void WINDOW_LockToggle(void){
	// Toggle Lock function
	g_isLocked ^= 1;
}

/*
    Function to initialize necessary pins and modules for the power windows
*/
void WINDOW_init(void){	
    // Initializing Motors 
    Motor_init();

    /*============================ DRIVER INITS =============================*/
		// Window for driver 
		windows[0].id = DRIVER;
    windows[0].up_port = DRIVER_UP_PORT;
    windows[0].up_pin = DRIVER_UP_PIN;
    windows[0].driver_up_port = 0;
    windows[0].driver_up_pin = 0;
    windows[0].down_port = DRIVER_DOWN_PORT;
    windows[0].down_pin = DRIVER_DOWN_PIN;
    windows[0].driver_down_port = 0;
    windows[0].driver_down_pin = 0;
    windows[0].top_limit_port = DRIVER_TOP_LIMIT_PORT;
    windows[0].top_limit_pin = DRIVER_TOP_LIMIT_PIN;
    windows[0].bottom_limit_port = DRIVER_BOTTOM_LIMIT_PORT;
    windows[0].bottom_limit_pin = DRIVER_BOTTOM_LIMIT_PIN;
    windows[0].jam_port = DRIVER_JAM_PORT;
    windows[0].jam_pin = DRIVER_JAM_PIN;
    // Initializing driver control board
    DIO_Init(DRIVER_UP_PORT, DRIVER_UP_PIN, PIN_INPUT);
    DIO_Init(DRIVER_DOWN_PORT, DRIVER_DOWN_PIN, PIN_INPUT);
		DIO_Init(DRIVER_TOP_LIMIT_PORT, DRIVER_TOP_LIMIT_PIN, PIN_INPUT);
    DIO_Init(DRIVER_BOTTOM_LIMIT_PORT, DRIVER_BOTTOM_LIMIT_PIN, PIN_INPUT);
    DIO_Init(DRIVER_JAM_PORT, DRIVER_JAM_PIN, PIN_INPUT);
		DIO_Init(DRIVER_LOCK_PORT, DRIVER_LOCK_PIN, PIN_INPUT);
		INT_init(DRIVER_LOCK_PORT, DRIVER_LOCK_PIN);
		INT_setCallBack(DRIVER_LOCK_PORT,WINDOW_LockToggle);
		// Initializing Driver-Passengers controls
		DIO_Init(DRIVER_PASSENGER1_UP_PORT, DRIVER_PASSENGER1_UP_PIN, PIN_INPUT);
		DIO_Init(DRIVER_PASSENGER1_DOWN_PORT, DRIVER_PASSENGER1_DOWN_PIN, PIN_INPUT);
		#if (NUM_OF_WINDOWS == 4)
		DIO_Init(DRIVER_PASSENGER2_UP_PORT, DRIVER_PASSENGER2_UP_PIN, PIN_INPUT);
		DIO_Init(DRIVER_PASSENGER2_DOWN_PORT, DRIVER_PASSENGER2_DOWN_PIN, PIN_INPUT);		
		DIO_Init(DRIVER_PASSENGER3_UP_PORT, DRIVER_PASSENGER3_UP_PIN, PIN_INPUT);
		DIO_Init(DRIVER_PASSENGER3_DOWN_PORT, DRIVER_PASSENGER3_DOWN_PIN, PIN_INPUT);
		#endif
		
    #if (NUM_OF_WINDOWS == 4)
    DIO_Init(DRIVER_PASSENGER2_UP_PORT, DRIVER_PASSENGER2_UP_PIN, PIN_INPUT);
    DIO_Init(DRIVER_PASSENGER2_DOWN_PORT, DRIVER_PASSENGER2_DOWN_PIN, PIN_INPUT);
    DIO_Init(DRIVER_PASSENGER3_UP_PORT, DRIVER_PASSENGER3_UP_PIN, PIN_INPUT);
    DIO_Init(DRIVER_PASSENGER3_DOWN_PORT, DRIVER_PASSENGER3_DOWN_PIN, PIN_INPUT);
    #endif 
    /*============================ End of DRIVER INITS =============================*/
    
    /*============================ PASSENGER 1 INITS =============================*/
    // Window for the first passenger
    windows[1].id = PASSENGER1;
    windows[1].up_port = PASSENGER1_UP_PORT;
    windows[1].up_pin = PASSENGER1_UP_PIN;
    windows[1].driver_up_port = DRIVER_PASSENGER1_UP_PORT;
    windows[1].driver_up_pin = DRIVER_PASSENGER1_UP_PIN;
    windows[1].down_port = PASSENGER1_DOWN_PORT;
    windows[1].down_pin = PASSENGER1_DOWN_PIN;
    windows[1].driver_down_port = DRIVER_PASSENGER1_DOWN_PORT;
    windows[1].driver_down_pin = DRIVER_PASSENGER1_DOWN_PIN;
    windows[1].top_limit_port = PASSENGER1_TOP_LIMIT_PORT;
    windows[1].top_limit_pin = PASSENGER1_TOP_LIMIT_PIN;
    windows[1].bottom_limit_port = PASSENGER1_BOTTOM_LIMIT_PORT;
    windows[1].bottom_limit_pin = PASSENGER1_BOTTOM_LIMIT_PIN;
    windows[1].jam_port = PASSENGER1_JAM_PORT;
    windows[1].jam_pin = PASSENGER1_JAM_PIN;
    // Initializing passenger 1 control board
    DIO_Init(PASSENGER1_UP_PORT, PASSENGER1_UP_PIN, PIN_INPUT);
    DIO_Init(PASSENGER1_DOWN_PORT, PASSENGER1_DOWN_PIN, PIN_INPUT);
		DIO_Init(PASSENGER1_TOP_LIMIT_PORT, PASSENGER1_TOP_LIMIT_PIN, PIN_INPUT);
    DIO_Init(PASSENGER1_BOTTOM_LIMIT_PORT, PASSENGER1_BOTTOM_LIMIT_PIN, PIN_INPUT);
    DIO_Init(PASSENGER1_JAM_PORT, PASSENGER1_JAM_PIN, PIN_INPUT);
		
    /*============================ End of PASSENGER 1 INITS =============================*/


    #if (NUM_OF_WINDOWS == 4)
    /*============================ PASSENGER 2 INITS =============================*/
    // Window for the second passenger
    windows[2].id = PASSENGER2;
    windows[2].up_port = PASSENGER2_UP_PORT;
    windows[2].up_pin = PASSENGER2_UP_PIN;
    windows[2].driver_up_port = DRIVER_PASSENGER2_UP_PORT;
    windows[2].driver_up_pin = DRIVER_PASSENGER2_UP_PIN;
    windows[2].down_port = PASSENGER2_DOWN_PORT;
    windows[2].down_pin = PASSENGER2_DOWN_PIN;
    windows[2].driver_down_port = DRIVER_PASSENGER2_DOWN_PORT;
    windows[2].driver_down_pin = DRIVER_PASSENGER2_DOWN_PIN;
    windows[2].top_limit_port = PASSENGER2_TOP_LIMIT_PORT;
    windows[2].top_limit_pin = PASSENGER2_TOP_LIMIT_PIN;
    windows[2].bottom_limit_port = PASSENGER2_BOTTOM_LIMIT_PORT;
    windows[2].bottom_limit_pin = PASSENGER2_BOTTOM_LIMIT_PIN;
    windows[2].jam_port = PASSENGER2_JAM_PORT;
    windows[2].jam_pin = PASSENGER2_JAM_PIN;
    // Initializing passenger 1 control board
    DIO_Init(PASSENGER2_UP_PORT, PASSENGER2_UP_PIN, PIN_INPUT);
    DIO_Init(PASSENGER2_DOWN_PORT, PASSENGER2_DOWN_PIN, PIN_INPUT);
		DIO_Init(PASSENGER2_TOP_LIMIT_PORT, PASSENGER2_TOP_LIMIT_PIN, PIN_INPUT);
    DIO_Init(PASSENGER2_BOTTOM_LIMIT_PORT, PASSENGER2_BOTTOM_LIMIT_PIN, PIN_INPUT);
    DIO_Init(PASSENGER2_JAM_PORT, PASSENGER2_JAM_PIN, PIN_INPUT);
    /*============================ End of PASSENGER 2 INITS =============================*/

    /*============================ PASSENGER 3 INITS =============================*/
    // Initializing passenger 3 control board
    windows[3].id = PASSENGER3;
    windows[3].up_port = PASSENGER3_UP_PORT;
    windows[3].up_pin = PASSENGER3_UP_PIN;
    windows[3].driver_up_port = DRIVER_PASSENGER3_UP_PORT;
    windows[3].driver_up_pin = DRIVER_PASSENGER3_UP_PIN;
    windows[3].down_port = PASSENGER3_DOWN_PORT;
    windows[3].down_pin = PASSENGER3_DOWN_PIN;
    windows[3].driver_down_port = DRIVER_PASSENGER3_DOWN_PORT;
    windows[3].driver_down_pin = DRIVER_PASSENGER3_DOWN_PIN;
    windows[3].top_limit_port = PASSENGER3_TOP_LIMIT_PORT;
    windows[3].top_limit_pin = PASSENGER3_TOP_LIMIT_PIN;
    windows[3].bottom_limit_port = PASSENGER3_BOTTOM_LIMIT_PORT;
    windows[3].bottom_limit_pin = PASSENGER3_BOTTOM_LIMIT_PIN;
    windows[3].jam_port = PASSENGER3_JAM_PORT;
    windows[3].jam_pin = PASSENGER3_JAM_PIN;
    // Initializing passenger 3 control board
    DIO_Init(PASSENGER3_UP_PORT, PASSENGER3_UP_PIN, PIN_INPUT);
    DIO_Init(PASSENGER3_DOWN_PORT, PASSENGER3_DOWN_PIN, PIN_INPUT);
		DIO_Init(PASSENGER3_TOP_LIMIT_PORT, PASSENGER3_TOP_LIMIT_PIN, PIN_INPUT);
    DIO_Init(PASSENGER3_BOTTOM_LIMIT_PORT, PASSENGER3_BOTTOM_LIMIT_PIN, PIN_INPUT);
    DIO_Init(PASSENGER3_JAM_PORT, PASSENGER3_JAM_PIN, PIN_INPUT);
    /*============================ End of PASSENGER 3 INITS =============================*/

    #endif
}


/*
  Generic Task for passenger windows 
*/
void WINDOW_PassengerTask(void* pvParameter){
  Window_type* window = (Window_type*)pvParameter;
  uint8 cmd;
  // Application
  while(1){
    // Reading Passenger Commands
    if (g_isLocked == 1){
      vTaskDelay(5/portTICK_RATE_MS);
    }
    else{
      // Passenger Controls
      /*============================= UP ================================*/
      if (((IS_ON(window->up_port, window->up_pin) && (g_isLocked == 0)) || ((window->id != DRIVER) && IS_ON(window->driver_up_port, window->driver_up_pin))) && (IS_OFF(window->top_limit_port, window->top_limit_pin))){ 
        vTaskDelay(100/portTICK_RATE_MS);
        // Manual Control
        if (((IS_ON(window->up_port, window->up_pin) && (g_isLocked == 0)) || ((window->id != DRIVER) && IS_ON(window->driver_up_port, window->driver_up_pin))) && (IS_OFF(window->top_limit_port, window->top_limit_pin))){
          cmd = (window->id * 10) + MOVE_UP_CMD;
          xQueueSend(xCommandQueue, &cmd, portMAX_DELAY);
          while(((IS_ON(window->up_port, window->up_pin) && (g_isLocked == 0)) || ((window->id != DRIVER) && IS_ON(window->driver_up_port, window->driver_up_pin))) && (IS_OFF(window->top_limit_port, window->top_limit_pin)));
          cmd = (window->id * 10) + STOP_CMD;
          xQueueSend(xCommandQueue, &cmd, portMAX_DELAY);
        }
        else{
          cmd = (window->id * 10) + MOVE_UP_CMD;
					xQueueSend(xCommandQueue, &cmd, portMAX_DELAY);
        }
      }
      /*============================= DOWN ================================*/
      if (((IS_ON(window->down_port, window->down_pin) && (g_isLocked == 0)) || ((window->id != DRIVER) && IS_ON(window->driver_down_port, window->driver_down_pin))) && (IS_OFF(window->bottom_limit_port, window->bottom_limit_pin))){
        vTaskDelay(100/portTICK_RATE_MS);
        // Manual Control
        if (((IS_ON(window->down_port, window->down_pin) && (g_isLocked == 0)) || ((window->id != DRIVER) && IS_ON(window->driver_down_port, window->driver_down_pin))) && (IS_OFF(window->bottom_limit_port, window->bottom_limit_pin))){
          cmd = (window->id * 10) + MOVE_DOWN_CMD;
          xQueueSend(xCommandQueue, &cmd, portMAX_DELAY);
          while(((IS_ON(window->down_port, window->down_pin) && (g_isLocked == 0)) || ((window->id != DRIVER) && IS_ON(window->driver_down_port, window->driver_down_pin))) && (IS_OFF(window->bottom_limit_port, window->bottom_limit_pin)));
          cmd = (window->id * 10) + STOP_CMD;
          xQueueSend(xCommandQueue, &cmd, portMAX_DELAY);
        }
        else{
          cmd = (window->id * 10) + MOVE_DOWN_CMD;
					xQueueSend(xCommandQueue, &cmd, portMAX_DELAY);
        }
      }
			/*============================= LIMIT SWITCHES ================================*/
			if (IS_ON(window->top_limit_port, window->top_limit_pin) || IS_ON(window->bottom_limit_port, window->bottom_limit_pin)){
          cmd = (window->id * 10) + STOP_CMD;
					xQueueSend(xCommandQueue, &cmd, portMAX_DELAY);
			}
    }
		
  }
}


// A motor task is created per window to handle the motor movement of each window
void WINDOW_MotorTask(void* pvParameters){
    uint8 cmd, window_id, iterator;
    while(1){
      if (xQueueReceive(xCommandQueue, &cmd, portMAX_DELAY)){
        // Extract command information
        window_id = cmd / 10;
        cmd = cmd - (window_id * 10);

        // Motor Processing 
        switch(cmd){
          // Move Up Direction
          case MOVE_UP_CMD:
            Motor_Rotate(window_id, UP);
            break;
          // Move Down Direction
          case MOVE_DOWN_CMD:
            Motor_Rotate(window_id, DOWN);
            break;
          // Stop Motor
          case STOP_CMD:
            Motor_Rotate(window_id, OFF);
            break;
        }
			}
		}
}