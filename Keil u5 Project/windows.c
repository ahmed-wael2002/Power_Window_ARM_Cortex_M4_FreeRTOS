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
#include "tm4c123gh6pm.h"


/*************************************************************************************************************
  			                                GLOBAL VARIABLES
 *************************************************************************************************************/
static volatile uint8 g_isLocked = 0;   // lock flag for windows (PRIVATE -- should not be accessible outside the module)
Window_type windows[NUM_OF_WINDOWS];    // Array of windows objects (PUBLIC -- used to access windows information in the system & inits)
QueueHandle_t xCommandQueue;            // Commands queues for windows (PUBLIC -- used to send commands to motors)
xSemaphoreHandle xBinarySemaphore;      // Binary semaphore for jamming detection (PUBLIC -- used to detect jamming of windows)


/*************************************************************************************************************
  			                            INTERRUPT SERVICE ROUTINES
 *************************************************************************************************************/
/*
    Interrupt Service Routine -- Function to be executed on ON/OFF Switch interrupt
*/
void WINDOW_LockToggle(void){
	// Toggle Lock function
	g_isLocked ^= 1;
    INT_clearInterrupt(DRIVER_LOCK_PORT, DRIVER_LOCK_PIN);
}

/*
    Interrupt Service Routine -- Function to be executed on Jamming Sensor interrupt
*/
void WINDOW_JamISR(void){
  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR(xBinarySemaphore, &xHigherPriorityTaskWoken);
  // Detect which interrupt is fired then clear the flag
  for (uint8 i=0; i<NUM_OF_WINDOWS; i++){
      if(INT_isInterruptSet(windows[i].jam_port, windows[i].jam_pin)){
          windows[i].jam_flag = 1;
          INT_clearInterrupt(windows[i].jam_port, windows[i].jam_pin);
      }
  }
  portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}


/*************************************************************************************************************
  			                            FUNCTION DEFINITIONS & TASKS
 *************************************************************************************************************/

/*
    Jamming Semaphore Task -- Task to handle jamming of windows
*/
void WINDOW_JammingSemaphoreTask(void* pvParameters){
  xSemaphoreTake(xBinarySemaphore,0);
  uint8 cmd;
  while(1){
      xSemaphoreTake(xBinarySemaphore,portMAX_DELAY);
      for(uint8 i=0; i<NUM_OF_WINDOWS;i++){
          // Look for windows that are running on automatic mode and are jammed
          if((windows[i].auto_flag == 1) && (windows[i].jam_flag == 1)){
              Motor_Rotate(i, DOWN);
              for(uint32 delay=0; ((delay<JAM_DELAY_MS)&&(IS_OFF(windows[i].bottom_limit_port, windows[i].bottom_limit_pin))); delay++);
              windows[i].auto_flag = 0;
              windows[i].jam_flag = 0;
          }
      }
  }
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
    windows[0].driver_up_port = DRIVER_UP_PORT;
    windows[0].driver_up_pin = DRIVER_UP_PIN;
    windows[0].down_port = DRIVER_DOWN_PORT;
    windows[0].down_pin = DRIVER_DOWN_PIN;
    windows[0].driver_down_port = DRIVER_DOWN_PORT;
    windows[0].driver_down_pin = DRIVER_DOWN_PIN;
    windows[0].top_limit_port = DRIVER_TOP_LIMIT_PORT;
    windows[0].top_limit_pin = DRIVER_TOP_LIMIT_PIN;
    windows[0].bottom_limit_port = DRIVER_BOTTOM_LIMIT_PORT;
    windows[0].bottom_limit_pin = DRIVER_BOTTOM_LIMIT_PIN;
    windows[0].jam_port = DRIVER_JAM_PORT;
    windows[0].jam_pin = DRIVER_JAM_PIN;
    windows[0].auto_flag = 0;
    windows[0].jam_flag = 0;
    // Initializing driver control board
    DIO_Init(DRIVER_UP_PORT, DRIVER_UP_PIN, PIN_INPUT);
    DIO_Init(DRIVER_DOWN_PORT, DRIVER_DOWN_PIN, PIN_INPUT);
    DIO_Init(DRIVER_TOP_LIMIT_PORT, DRIVER_TOP_LIMIT_PIN, PIN_INPUT);
    DIO_Init(DRIVER_BOTTOM_LIMIT_PORT, DRIVER_BOTTOM_LIMIT_PIN, PIN_INPUT);
    // Initializing Driver Jam Sensor
    DIO_Init(DRIVER_JAM_PORT, DRIVER_JAM_PIN, PIN_INPUT);
    INT_init(DRIVER_JAM_PORT, DRIVER_JAM_PIN);
    INT_setCallBack(DRIVER_JAM_PORT,WINDOW_JamISR);
    // Initializing Driver Lock
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
    windows[1].auto_flag = 0;
    windows[1].jam_flag = 0;
    // Initializing passenger 1 control board
    DIO_Init(PASSENGER1_UP_PORT, PASSENGER1_UP_PIN, PIN_INPUT);
    DIO_Init(PASSENGER1_DOWN_PORT, PASSENGER1_DOWN_PIN, PIN_INPUT);
    DIO_Init(PASSENGER1_TOP_LIMIT_PORT, PASSENGER1_TOP_LIMIT_PIN, PIN_INPUT);
    DIO_Init(PASSENGER1_BOTTOM_LIMIT_PORT, PASSENGER1_BOTTOM_LIMIT_PIN, PIN_INPUT);
    DIO_Init(PASSENGER1_JAM_PORT, PASSENGER1_JAM_PIN, PIN_INPUT);
    // Initializing Passenger Jam Sensor
    DIO_Init(PASSENGER1_JAM_PORT, PASSENGER1_JAM_PIN, PIN_INPUT);
    INT_init(PASSENGER1_JAM_PORT, PASSENGER1_JAM_PIN);
    INT_setCallBack(PASSENGER1_JAM_PORT,WINDOW_JamISR);
		
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
    windows[2].auto_flag = 0;
    windows[2].jam_flag = 0;
    // Initializing passenger 1 control board
    DIO_Init(PASSENGER2_UP_PORT, PASSENGER2_UP_PIN, PIN_INPUT);
    DIO_Init(PASSENGER2_DOWN_PORT, PASSENGER2_DOWN_PIN, PIN_INPUT);
    DIO_Init(PASSENGER2_TOP_LIMIT_PORT, PASSENGER2_TOP_LIMIT_PIN, PIN_INPUT);
    DIO_Init(PASSENGER2_BOTTOM_LIMIT_PORT, PASSENGER2_BOTTOM_LIMIT_PIN, PIN_INPUT);
    DIO_Init(PASSENGER2_JAM_PORT, PASSENGER2_JAM_PIN, PIN_INPUT);
    DIO_Init(PASSENGER2_JAM_PORT, PASSENGER2_JAM_PIN, PIN_INPUT);
    INT_init(PASSENGER2_JAM_PORT, PASSENGER2_JAM_PIN);
    INT_setCallBack(PASSENGER2_JAM_PORT,WINDOW_JamISR);
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
    windows[3].auto_flag = 0;
    windows[3].jam_flag = 0;
    // Initializing passenger 3 control board
    DIO_Init(PASSENGER3_UP_PORT, PASSENGER3_UP_PIN, PIN_INPUT);
    DIO_Init(PASSENGER3_DOWN_PORT, PASSENGER3_DOWN_PIN, PIN_INPUT);
    DIO_Init(PASSENGER3_TOP_LIMIT_PORT, PASSENGER3_TOP_LIMIT_PIN, PIN_INPUT);
    DIO_Init(PASSENGER3_BOTTOM_LIMIT_PORT, PASSENGER3_BOTTOM_LIMIT_PIN, PIN_INPUT);
    DIO_Init(PASSENGER3_JAM_PORT, PASSENGER3_JAM_PIN, PIN_INPUT);
    DIO_Init(PASSENGER3_JAM_PORT, PASSENGER3_JAM_PIN, PIN_INPUT);
    INT_init(PASSENGER3_JAM_PORT, PASSENGER3_JAM_PIN);
    INT_setCallBack(PASSENGER3_JAM_PORT,WINDOW_JamISR);
    /*============================ End of PASSENGER 3 INITS =============================*/

    #endif
}


/*
  Generic Task for handling windows inputs 
*/
void WINDOW_PassengerTask(void* pvParameter){
  Window_type* window = (Window_type*)pvParameter;
  uint8 cmd;
  // Application
  while(1){
    // Passenger Controls
    /*============================= UP ================================*/
    if (((IS_ON(window->up_port, window->up_pin) && (g_isLocked == 0)) || (IS_ON(window->driver_up_port, window->driver_up_pin))) && (IS_OFF(window->top_limit_port, window->top_limit_pin))){ 
        vTaskDelay(200/portTICK_RATE_MS);
        // Manual Control
        if (((IS_ON(window->up_port, window->up_pin) && (g_isLocked == 0)) || ((IS_ON(window->driver_up_port, window->driver_up_pin)))) && (IS_OFF(window->top_limit_port, window->top_limit_pin))){
            cmd = (window->id * 10) + MOVE_UP_CMD;
            xQueueSend(xCommandQueue, &cmd, portMAX_DELAY);
            while(((IS_ON(window->up_port, window->up_pin) && (g_isLocked == 0)) || (IS_ON(window->driver_up_port, window->driver_up_pin))) && (IS_OFF(window->top_limit_port, window->top_limit_pin)));
            cmd = (window->id * 10) + STOP_CMD;
            xQueueSend(xCommandQueue, &cmd, portMAX_DELAY);
        }
        // Automatic Control
        else{
            window->auto_flag = 1;
            cmd = (window->id * 10) + MOVE_UP_CMD;
            xQueueSend(xCommandQueue, &cmd, portMAX_DELAY);
            while((IS_OFF(window->top_limit_port, window->top_limit_pin)) && (window->auto_flag == 1));
            cmd = (window->id * 10) + STOP_CMD;
            xQueueSend(xCommandQueue, &cmd, portMAX_DELAY);
            window->auto_flag = 0;
        }
    }

    /*============================= DOWN ================================*/
    if (((IS_ON(window->down_port, window->down_pin) && (g_isLocked == 0)) || (IS_ON(window->driver_down_port, window->driver_down_pin))) && (IS_OFF(window->bottom_limit_port, window->bottom_limit_pin))){ 
        vTaskDelay(200/portTICK_RATE_MS);
        // Manual Control
        if (((IS_ON(window->down_port, window->down_pin) && (g_isLocked == 0)) || (IS_ON(window->driver_down_port, window->driver_down_pin))) && (IS_OFF(window->bottom_limit_port, window->bottom_limit_pin))){
            cmd = (window->id * 10) + MOVE_DOWN_CMD;
            xQueueSend(xCommandQueue, &cmd, portMAX_DELAY);
            while(((IS_ON(window->down_port, window->down_pin) && (g_isLocked == 0)) || (IS_ON(window->driver_down_port, window->driver_down_pin))) && (IS_OFF(window->bottom_limit_port, window->bottom_limit_pin)));
            cmd = (window->id * 10) + STOP_CMD;
            xQueueSend(xCommandQueue, &cmd, portMAX_DELAY);
        }
        // Automatic Control
        else{
            cmd = (window->id * 10) + MOVE_DOWN_CMD;
            xQueueSend(xCommandQueue, &cmd, portMAX_DELAY);
            while((IS_OFF(window->bottom_limit_port, window->bottom_limit_pin)));
            cmd = (window->id * 10) + STOP_CMD;
            xQueueSend(xCommandQueue, &cmd, portMAX_DELAY);
        }
    }
  }
}


/*
  Generic Task for handling motor outputs 
*/
void WINDOW_MotorTask(void* pvParameters){
    uint8 cmd, window_id;
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