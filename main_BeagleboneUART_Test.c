// Lab 6, uP2 Fall 2023
// Created: 2023-07-31
// Updated: 2023-08-01
// Lab 6 is intended to serve as an introduction to the BeagleBone Black and wireless
// communication concepts.

/************************************Includes***************************************/

#include "G8RTOS/G8RTOS.h"
#include "./MultimodDrivers/multimod.h"
#include "./threads.h"

#include <driverlib/interrupt.h>

/*********************************FUNCTIONS*****************************************/
#define MAX_NUM_FACES     10
uint16_t face_array[3 + (MAX_NUM_FACES*4)];
bool lowByteReceived = false;
uint16_t currentFaceParameterNumber = 0;
uint16_t character = 0x00;


void UARTOutstring(char* string) {
    while(*string) {
        UARTCharPut(UART4_BASE, *(string++));
    }
}

void UART4ISR_TEST(void) {
    // First, clear interrupt flag (normally we do it last in the ISR, but the UARTIntClear() function recommends we actually do it first
    // in the ISR because clearing it takes several clock cycles
    UARTIntClear(UART4_BASE, UART_INT_RX);

    // Since this interrupt is only called when something comes to the 'RX' pin, get the char:
    int32_t beaglebone_char = UARTCharGet(UART4_BASE);

    // Output to console (we can use an "UARTstdio" function because as of this function call, we're working within the TIVA-Launchpad
    UARTCharPut(UART0_BASE, beaglebone_char);

}

void UART4ISR(void) { // According to the results of this, we receive 8 characters at a time?
    int32_t beaglebone_char = 0x00;
    // First, clear interrupt flag (normally we do it last in the ISR, but the UARTIntClear() function recommends we actually do it first
    // in the ISR because clearing it takes several clock cycles
    UARTIntClear(UART4_BASE, UART_INT_RX);

    // Since this interrupt is only called when something comes to the 'RX' pin, get the char:
    beaglebone_char = UARTCharGet(UART4_BASE);
    if (beaglebone_char == '\n') {
        //UARTprintf("\nSending new image frame!\n");
        currentFaceParameterNumber = 0;
    }
    else {
        if (!lowByteReceived) {
            character |= beaglebone_char << 0;
            lowByteReceived = true;
        }
        else if (lowByteReceived) { // This calls once we've finished receiving a character
            character |= beaglebone_char << 8;
            //UARTprintf("Character received: %d\n", character); //for some reason, commenting this in causes problems for uart transmission!
            face_array[currentFaceParameterNumber++] = character;
            character = 0x00;

            // Reset the lowByteReceived parameter back to false!
            lowByteReceived = false;
        }
    }
}


/************************************MAIN*******************************************/
int main(void) {
    // Initialize the TIVA processor's system clock:
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Initialize relevant modules from the multi-mod board:
        multimod_init();

    // Initialize Interrupt Handler for the UART4_BASE RX Buttons:
      IntMasterDisable();
        UARTIntRegister(UART4_BASE, UART4ISR); // Handles the "IntEnable()" and "IntRegister()" functions for any UART-based interrupts
          // IntRegister(INT_GPIOD, GPIODHandler);
        //IntPrioritySet(INT_UART4, 7);
        UARTIntEnable(UART4_BASE, UART_INT_RX);
        IntEnable(INT_UART4);
      IntMasterEnable();

   // Write messages to the Beaglebone via UART:
      while(1) {

      }
}


