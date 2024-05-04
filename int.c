#include "tm4c123gh6pm.h"
#include "bitwise_operation.h"
#include "int.h"

/*===============================================================================*/
/*                         FUNCTION DEFINITIONS                                  */
/*===============================================================================*/
void GPIOD_Handler(void);

/* Function to initialize Magnet Pin */
void INT_init(uint8 portNumber, uint8 pinNumber)
{
    switch (portNumber)
    {
    case PORTA_ID:
        SET_BIT(GPIO_PORTA_IM_R, pinNumber);
        CLEAR_BIT(GPIO_PORTA_IS_R, pinNumber);
        SET_BIT(GPIO_PORTA_IEV_R, pinNumber);
        CLEAR_BIT(GPIO_PORTA_IBE_R, pinNumber);
        NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFFFF1F) | ((INTERRUPT_PRIORITY_LEVEL & 0x07) << 5);
        SET_BIT(NVIC_EN0_R, INT_GPIOA - 16);
        break;

    case PORTB_ID:
        SET_BIT(GPIO_PORTB_IM_R, pinNumber);
        CLEAR_BIT(GPIO_PORTB_IS_R, pinNumber);
        SET_BIT(GPIO_PORTB_IEV_R, pinNumber);
        CLEAR_BIT(GPIO_PORTB_IBE_R, pinNumber);
        NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFF1FFF) | ((INTERRUPT_PRIORITY_LEVEL & 0x07) << 13);
        SET_BIT(NVIC_EN0_R, INT_GPIOB - 16);
        break;

    case PORTC_ID:
        SET_BIT(GPIO_PORTC_IM_R, pinNumber);
        CLEAR_BIT(GPIO_PORTC_IS_R, pinNumber);
        SET_BIT(GPIO_PORTC_IEV_R, pinNumber);
        CLEAR_BIT(GPIO_PORTC_IBE_R, pinNumber);
        NVIC_PRI0_R = (NVIC_PRI0_R & 0xFF1FFFFF) | ((INTERRUPT_PRIORITY_LEVEL & 0x07) << 21);
        SET_BIT(NVIC_EN0_R, INT_GPIOC - 16);
        break;

    case PORTD_ID:
        CLEAR_BIT(GPIO_PORTD_ICR_R, pinNumber);                                               // clears the int flag (if any
        SET_BIT(GPIO_PORTD_IM_R, pinNumber);                                                  // enables int for the pin
        CLEAR_BIT(GPIO_PORTD_IS_R, pinNumber);                                                // edge or level ?
        SET_BIT(GPIO_PORTD_IEV_R, pinNumber);                                                 // +ve or -ve ?
        CLEAR_BIT(GPIO_PORTD_IBE_R, pinNumber);                                               // both ?
        NVIC_PRI0_R = (NVIC_PRI0_R & 0x1FFFFFFF) | ((INTERRUPT_PRIORITY_LEVEL & 0x07) << 29); // Priotiy ?
        SET_BIT(NVIC_EN0_R, INT_GPIOD - 16);
        break;

    case PORTE_ID:
        SET_BIT(GPIO_PORTE_IM_R, pinNumber);
        CLEAR_BIT(GPIO_PORTE_IS_R, pinNumber);
        SET_BIT(GPIO_PORTE_IEV_R, pinNumber);
        CLEAR_BIT(GPIO_PORTE_IBE_R, pinNumber);
        NVIC_PRI1_R = (NVIC_PRI1_R & 0xFFFFFF1F) | ((INTERRUPT_PRIORITY_LEVEL & 0x07) << 5);
        SET_BIT(NVIC_EN0_R, INT_GPIOE - 16);
        break;

    case PORTF_ID:
        SET_BIT(GPIO_PORTF_IM_R, pinNumber);
        CLEAR_BIT(GPIO_PORTF_IS_R, pinNumber);
        SET_BIT(GPIO_PORTF_IEV_R, pinNumber);
        CLEAR_BIT(GPIO_PORTF_IBE_R, pinNumber);
        NVIC_PRI1_R = (NVIC_PRI1_R & 0xFFFF1FFF) | ((INTERRUPT_PRIORITY_LEVEL & 0x07) << 13);
        SET_BIT(NVIC_EN0_R, INT_GPIOF - 16);
        break;
    default:
        break;
    }
}

volatile void (*callBackPtr[NUM_OF_PORTS])(void)  = {NULL};

/* Function to set call back */
void INT_setCallBack(uint8 portNumber, void(*a_ptr)(void))
{
    if((portNumber >= 0) && (portNumber < NUM_OF_PORTS)){
        /* Save the address of the Call back function in a global variable */
        callBackPtr[portNumber] = (volatile void (*) (void))a_ptr;
    }
}

/*===============================================================================*/
/*                             INTERRUPT HANDLERS                                */
/*===============================================================================*/

//********************* PORT A - Handler
void GPIOA_Handler(void)
{
    if (callBackPtr != NULL)
    {
        /* Call the Call Back function in the application after the edge is detected */
        (*callBackPtr[PORTA_ID])(); /* another method to call the function using pointer to function g_callBackPtr(); */
        SET_BIT(GPIO_PORTA_ICR_R, PIN0_ID);
    }
}

//********************* PORT B - Handler
void GPIOB_Handler(void)
{
    if (callBackPtr != NULL)
    {
        /* Call the Call Back function in the application after the edge is detected */
        (*callBackPtr[PORTB_ID])(); /* another method to call the function using pointer to function g_callBackPtr(); */
        SET_BIT(GPIO_PORTB_ICR_R, PIN0_ID);
    }
}

//********************* PORT C - Handler
void GPIOC_Handler(void)
{
    if (callBackPtr != NULL)
    {
        /* Call the Call Back function in the application after the edge is detected */
        (*callBackPtr[PORTC_ID])(); /* another method to call the function using pointer to function g_callBackPtr(); */
        SET_BIT(GPIO_PORTC_ICR_R, PIN0_ID);
    }
}

//********************* PORT D - Handler
void GPIOD_Handler(void)
{
    if (callBackPtr != NULL)
    {
        /* Call the Call Back function in the application after the edge is detected */
        (*callBackPtr[PORTD_ID])(); /* another method to call the function using pointer to function g_callBackPtr(); */
        SET_BIT(GPIO_PORTD_ICR_R, PIN0_ID);
    }
}

//********************* PORT E - Handler
void GPIOE_Handler(void)
{
    if (callBackPtr != NULL)
    {
        /* Call the Call Back function in the application after the edge is detected */
        (*callBackPtr[PORTE_ID])(); /* another method to call the function using pointer to function g_callBackPtr(); */
        SET_BIT(GPIO_PORTE_ICR_R, PIN0_ID);
    }
}

//********************* PORT F - Handler
void GPIOF_Handler(void)
{
    if (callBackPtr != NULL)
    {
        /* Call the Call Back function in the application after the edge is detected */
        (*callBackPtr[PORTF_ID])(); /* another method to call the function using pointer to function g_callBackPtr(); */
        SET_BIT(GPIO_PORTF_ICR_R, PIN0_ID);
    }
}


