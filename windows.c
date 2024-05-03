#include "windows.h"


////////// Global lock variable
// Variable should be inaccessible from other modules 
// static = private
static uint8 g_isLocked = 0;
static Window_type windows[NUM_OF_WINDOWS];

 /*
 Function to initialize necessary pins and modules for the power windows
 */
 void WINDOW_init(void){
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
    Window_type window = (Window_type *) pvParameter;

    while(1){
        // Up Button Code
        if ((window.id != DRIVER) && (g_isLocked == 1)){
            /* Do Nothing*/
        }
        else{
            if (IS_ON(PASSENGER_UP_ACTION(window.id)) && (g_isLocked==0)){
                while(IS_OFF(PASSENGER_UP_ACTION(window.id)) && (g_isLocked==0));
                if (IS_ON(PASSENGER_UP_ACTION(window.id)) && (g_isLocked==0)){
                    DcMotor_Rotate(window.id, ANTICLOCKWISE);
                }
            }
            else if (IS_ON(PASSENGER_DOWN_ACTION(window.id)) && (g_isLocked==0)){
                while(IS_OFF(PASSENGER_DOWN_ACTION(window.id)) && (g_isLocked==0));
                if (IS_ON(PASSENGER_DOWN_ACTION(window.id)) && (g_isLocked==0)){
                    DcMotor_Rotate(window.id, CLOCKWISE);
                }
            }
            DcMotor_Rotate(window.id, OFF);
        }

    }
  }