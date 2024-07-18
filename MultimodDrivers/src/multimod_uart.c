// multimod_uart.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for UART functions

/************************************Includes***************************************/

#include "../multimod_uart.h"


#include <stdbool.h>

#include <inc/tm4c123gh6pm.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>

#include <driverlib/uartstdio.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>

/************************************Includes***************************************/

/********************************Public Functions***********************************/

// UART_Init
// Initializes UART serial communication with PC
// Return: void
void UART_Init() {
    // Enable port A:
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    // Enable UART0 module:
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // Configure UART0 pins on port A:
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTFIFODisable(UART0_BASE);

    // Set UART clock source:
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_SYSTEM);

    // Configure UART baud rate:
    UARTStdioConfig(0, 115200, SysCtlClockGet());

}

// UART_BeagleBone_Init
// Initializes UART serial communication with the Beaglebone
// Return: void
void UART_BeagleBone_Init(void) {
    // Finish this function:

    /*
     According to the MultiMod schematic, the TIVA Launchpad uses
     UART4 BASE to connect with the Beaglebone.

     From the perspective of the TIVA Launchpad:
     RX pin: PC4
     TX pin: PC5
     */

    // Enable port C:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    // Enable UART4 module:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);

    // Configure UART4 pins on port C:
        GPIOPinConfigure(GPIO_PC4_U4RX);
        GPIOPinConfigure(GPIO_PC5_U4TX);
        GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);

        //UARTEnable(UART4_BASE); //Shouldn't use this because this function enables the UART4_BASE's FIFOS, which we don't want

        UARTFIFODisable(UART4_BASE);

    // Set UART clock source:
        UARTClockSourceSet(UART4_BASE, UART_CLOCK_SYSTEM);

    // Configure UART baud rate:
        //UARTStdioConfig(0, 115200, SysCtlClockGet()); //from the "uartstdio.h" library
        UARTConfigSetExpClk(UART4_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8)); //from "uart.c" driverlib.c library
        //UARTEnable(UART4_BASE);

}


// UART_Bluetooth_Init
// Initializes UART serial communication with the Bluetooth
// Return: void
void UART_Bluetooth_Init(void) {
    /*
     According to the MultiMod schematic, the TIVA Launchpad uses
     UART3 BASE for communication with Bluetooth module.

     From the perspective of the TIVA Launchpad:
     RX pin: PC6
     TX pin: PC7
     */

    // Enable port C:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    // Enable UART3 module:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);

    // Configure UART3 pins on port C:
        GPIOPinConfigure(GPIO_PC6_U3RX);
        GPIOPinConfigure(GPIO_PC7_U3TX);
        GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);

        //UARTEnable(UART4_BASE); //Shouldn't use this because this function enables the UART4_BASE's FIFOS, which we don't want

        UARTFIFODisable(UART3_BASE);

    // Set UART clock source:
        UARTClockSourceSet(UART3_BASE, UART_CLOCK_SYSTEM);

    // Configure UART baud rate:
        //UARTStdioConfig(0, 115200, SysCtlClockGet()); //from the "uartstdio.h" library
        UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), 1000000, (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8)); //from "uart.c" driverlib.c library
        //UARTEnable(UART4_BASE);

}


/********************************Public Functions***********************************/

