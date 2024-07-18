// G8RTOS_Threads.c
// Date Created: 2023-07-25
// Date Updated: 2023-07-27
// Defines for thread functions.

/************************************Includes***************************************/

#include "./threads.h"

#include "./MultimodDrivers/multimod.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/************************************Includes***************************************/

/*************************************Defines***************************************/
/*************************************Defines***************************************/

/*********************************Global Variables**********************************/
uint16_t face_array[3 + (MAX_NUM_FACES*4)];
bool lowByteReceived = false;
uint16_t currentFaceParameterNumber = 0;
uint16_t character = 0x00;
/*********************************Global Variables**********************************/

/*************************************Threads***************************************/
void Idle_Thread(void) {
    while(1);
}


// ST7789 Boundaries
//#define X_MAX                       240
//#define Y_MAX                       280
void DisplayFaceRectangle(void) {
    uint16_t image_height, image_width, numFaces;
    uint16_t x, y, w, h, normalized_x, normalized_y, normalized_w, normalized_h;
    uint16_t local_face_array[3 + (MAX_NUM_FACES*4)];
    while(1) {
        // Copy the array that the a-periodic interrupt sees to the local array:
        IntMasterDisable();
        for (uint8_t i = 0; i < (3 + (MAX_NUM_FACES*4)); i++) {
            local_face_array[i] = face_array[i];
        }
        IntMasterEnable();

        image_height = local_face_array[0];
        image_width = local_face_array[1];
        numFaces = local_face_array[2];

        for (uint8_t i = 0; i < numFaces; i++) {
            x = local_face_array[3 + (4*i) + 0];
            y = local_face_array[3 + (4*i) + 1];
            w = local_face_array[3 + (4*i) + 2];
            h = local_face_array[3 + (4*i) + 3];

            normalized_x = X_MAX * (((float)x)/((float)image_width));
            normalized_y = Y_MAX * (((float)y)/((float)image_height));
            normalized_w = X_MAX * (((float)w)/((float)image_width));
            normalized_h = Y_MAX * (((float)w)/((float)image_height));

            //Draw in bounds:
            ST7789_DrawRectangle(normalized_x, normalized_y, normalized_w, normalized_h, ST7789_GREEN);

            //Draw a slightly smaller rectangle over it that's black:
            ST7789_DrawRectangle(normalized_x + BOUNDING_BOX_THICKNESS, normalized_y + BOUNDING_BOX_THICKNESS, normalized_w - (2*BOUNDING_BOX_THICKNESS), normalized_h - (2*BOUNDING_BOX_THICKNESS), ST7789_BLACK);
        }

        //sleep (this is important): WE HAVE TO SLEEP RIGHT AFTER WE DRAW THE PIXEL (just the way it works out)
            sleep(100);

        for (uint8_t i = 0; i < numFaces; i++) {
            x = local_face_array[3 + (4*i) + 0];
            y = local_face_array[3 + (4*i) + 1];
            w = local_face_array[3 + (4*i) + 2];
            h = local_face_array[3 + (4*i) + 3];

            normalized_x = X_MAX * (((float)x)/((float)image_width));
            normalized_y = Y_MAX * (((float)y)/((float)image_height));
            normalized_w = X_MAX * (((float)w)/((float)image_width));
            normalized_h = Y_MAX * (((float)w)/((float)image_height));
            ST7789_DrawRectangle(normalized_x, normalized_y, normalized_w, normalized_h, ST7789_BLACK);
        }

    } // end while loop
} // end function call

/*************************************Threads***************************************/

/********************************Periodic Threads***********************************/
void PrintLatestImageFrameData(void) {
    uint8_t numFaces = face_array[2];
    UARTprintf("\nLatest Image Frame Stats:\n");
    if (numFaces == 0) {
        UARTprintf("No faces detected in video frame!\n");
    }
    else {
        UARTprintf("Faces detected in image frame!\n");
        UARTprintf("Image Frame Dimensions: (%d, %d)\n", face_array[0], face_array[1]);
        UARTprintf("Number of Faces Detected: %d\n", numFaces);

        for (uint8_t i = 0; i < numFaces; i++) {
            UARTprintf("Face %d:\n", i+1);
            UARTprintf("x = %d\n", face_array[3 + (4*i) + 0]);
            UARTprintf("y = %d\n", face_array[3 + (4*i) + 1]);
            UARTprintf("w = %d\n", face_array[3 + (4*i) + 2]);
            UARTprintf("h = %d\n", face_array[3 + (4*i) + 3]);
        }
    }
}
/********************************Periodic Threads***********************************/


/*******************************Aperiodic Threads***********************************/
void BeagleboneUART_Handler(void) {
    int32_t beaglebone_char = 0x00;
    // First, clear interrupt flag (normally we do it last in the ISR, but the UARTIntClear() function recommends we actually do it first
    // in the ISR because clearing it takes several clock cycles
    UARTIntClear(UART4_BASE, UART_INT_RX);

    // Since this interrupt is only called when something comes to the 'RX' pin, get the char:
    beaglebone_char = UARTCharGet(UART4_BASE);
    if (beaglebone_char == '\n') {
        currentFaceParameterNumber = 0;
        UARTprintf("Image Frame received!\n");
    }
    else {
        if (!lowByteReceived) {
            character |= beaglebone_char << 0;
            lowByteReceived = true;
        }
        else if (lowByteReceived) { // This calls once we've finished receiving a character
            character |= beaglebone_char << 8;
            face_array[currentFaceParameterNumber++] = character;
            character = 0x00;

            // Reset the lowByteReceived parameter back to false!
            lowByteReceived = false;
        }
    }
}

void NumToString(char str[], int num)
{
    int i, rem, len = 0, n;

    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

void UART3Outstring(char* string) {
    while(*string || ( (*string) != '\0')) {
        UARTCharPut(UART3_BASE, *(string++));
    }
}

void BluetoothUART_Handler(void) {
    int32_t bluetooth_char = 0x00;
    // First, clear interrupt flag (normally we do it last in the ISR, but the UARTIntClear() function recommends we actually do it first
    // in the ISR because clearing it takes several clock cycles
    UARTIntClear(UART3_BASE, UART_INT_RX);

    // Since this interrupt is only called when something comes to the 'RX' pin, get the char:
    bluetooth_char = UARTCharGet(UART3_BASE);


    char string1[10];
    char* string;
    if (bluetooth_char == 's') {
        UARTprintf("Sent data over Bluetooth!\n");

        uint16_t local_face_array[3 + (MAX_NUM_FACES*4)];
        // Copy the array that the a-periodic interrupt sees to the local array:
        IntMasterDisable();
        for (uint8_t i = 0; i < (3 + (MAX_NUM_FACES*4)); i++) {
            local_face_array[i] = face_array[i];
        }
        IntMasterEnable();

      // Send data over UART:

        // Format the string to send:
        string = "Latest face data: \n";
        UART3Outstring(string);

        string = " Image Dimensions: (";
        UART3Outstring(string);

        uint16_t imageWidth = local_face_array[0];
        NumToString(string1, imageWidth);
        UART3Outstring(&string1[0]);

        string = ", ";
        UART3Outstring(string);

        uint16_t imageHeight = local_face_array[1];
        NumToString(string1, imageHeight);
        UART3Outstring(&string1[0]);

        string = ")\n";
        UART3Outstring(string);

        string = "Number of Faces Detected: ";
        UART3Outstring(string);

        uint16_t numFaces = local_face_array[2];
        NumToString(string1, numFaces);
        UART3Outstring(&string1[0]);

        string = "\n";
        UART3Outstring(string);

        // Finally, send face information over UART:
        uint8_t x, y, w, h;
        for (uint8_t i = 0; i < numFaces; i++) {
            //U("Face %d:\n", i+1);
            string = "Face ";
            UART3Outstring(string);

            NumToString(string1, i+1);
            UART3Outstring(&string1[0]);

            string = ": \n";
            UART3Outstring(string);


            x = local_face_array[3 + (4*i) + 0];
            y = local_face_array[3 + (4*i) + 1];
            w = local_face_array[3 + (4*i) + 2];
            h = local_face_array[3 + (4*i) + 3];


          //------- Sending x ----------
            string = "x: ";
            UART3Outstring(string);

            NumToString(string1, x);
            UART3Outstring(&string1[0]);

            string = "\n";
            UART3Outstring(string);


          //------- Sending y ----------
            string = "y: ";
            UART3Outstring(string);

            NumToString(string1, y);
            UART3Outstring(&string1[0]);

            string = "\n";
            UART3Outstring(string);


          //------- Sending w ----------
            string = "w: ";
            UART3Outstring(string);

            NumToString(string1, w);
            UART3Outstring(&string1[0]);

            string = "\n";
            UART3Outstring(string);


          //------- Sending h ----------
            string = "h: ";
            UART3Outstring(string);

            NumToString(string1, h);
            UART3Outstring(&string1[0]);

            string = "\n";
            UART3Outstring(string);


        }

    }
}
/*******************************Aperiodic Threads***********************************/
