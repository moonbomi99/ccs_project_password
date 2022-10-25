#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

void KeypadSetup(void)
{
    //
    // Enable the GPIO port that is used
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
    }

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {
    }

    //
    // Enable the GPIO pin for the keypad row (PB 3210).  Set the direction as Outputt, and
    // enable the GPIO pin for digital function.
    //
//    GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4, GPIO_DIR_MODE_OUT);
//    GPIODirModeSet(GPIO_PORTB_BASE, 0xF0, GPIO_DIR_MODE_OUT);
//    GPIOPadConfigSet(GPIO_PORTB_BASE, 0xF0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, 0xF0);  //

    //
    // Enable the GPIO pin for the keypad row (PB 3210).  Set the direction as Input, and
    // enable pur
    // enable the GPIO pin for digital function.
    //
//    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0);  // wo pur setup
//    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, 0x0F);      // wo pur setup
//    GPIO_PORTB_PUR_R |= 0x0F;
    GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

}
