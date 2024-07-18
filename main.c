#include "G8RTOS/G8RTOS.h"
#include "MultimodDrivers/multimod.h"
#include "threads.h"

// Lab 6, uP2 Fall 2023
// Created: 2023-07-31
// Updated: 2023-08-01
// Lab 6 is intended to serve as an introduction to the BeagleBone Black and wireless
// communication concepts.

/************************************Includes***************************************/

#include "G8RTOS/G8RTOS.h"
#include "./MultimodDrivers/multimod.h"

#include "./threads.h"

/************************************MAIN*******************************************/
int main(void) {
    // Set TIVA processor clock speed to 80 MHz:
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    //Initialize relevant modules on multimod board:
    multimod_init();

    //Initialize G8RTOS by moving Interrupt-vector to SRAM so we can dynamically modify it during runtime-execution
    G8RTOS_Init();

    // Draw a line: (test for final project)
//#define X_MAX                       240
//#define Y_MAX                       280
//void ST7789_DrawRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    //ST7789_DrawRectangle(20, 20, 200, 240, ST7789_GREEN);
    // So valid 200x240, find 25x30 dimension sprites


// Initialize semaphore, add threads:
    
    //Initialize FIFOs:
        //G8RTOS_InitFIFO(SPAWNCOOR_FIFO);

    //Initialize Semaphores:
        G8RTOS_InitSemaphore(&sem_SPIA, 1);

    //Add in background threads:
        char* idle_thread_name = "Idle Thread";
        char* display_face_rectangle_thread_name = "Display Face Rectangle Thread";

        G8RTOS_AddThread(Idle_Thread, 200, idle_thread_name); //make the idle-thread have lowest priority
        G8RTOS_AddThread(DisplayFaceRectangle, 100, display_face_rectangle_thread_name);

    //Add in periodic threads:
        uint32_t print_latest_image_frame_data_period = 600; //in ms

        //G8RTOS_Add_PeriodicEvent(PrintLatestImageFrameData, print_latest_image_frame_data_period, 5);

    //Add in a-periodic threads:
        G8RTOS_Add_APeriodicEvent(BeagleboneUART_Handler, 80, INT_UART4);
        UARTIntEnable(UART4_BASE, UART_INT_RX);

        G8RTOS_Add_APeriodicEvent(BluetoothUART_Handler, 81, INT_UART3); //set to a slighly lower priority b/c it's not as imperative that the phone gets face_rectangle data
        UARTIntEnable(UART3_BASE, UART_INT_RX);


        //ST7789_DrawRectangle(100, 100, 50, 50, ST7789_GREEN);

    //Officially launch the G8RTOS!
    G8RTOS_Launch();

    //Enter infinite while loop:
    while(1);
}

/************************************MAIN*******************************************/
