// threads.h
// Date Created: 2023-07-26
// Date Updated: 2023-07-26
// Threads

#ifndef THREADS_H_
#define THREADS_H_

/************************************Includes***************************************/

#include "./G8RTOS/G8RTOS.h"

/************************************Includes***************************************/

/*************************************Defines***************************************/
#define MAX_NUM_FACES     10
#define BOUNDING_BOX_THICKNESS     5
/*************************************Defines***************************************/

/***********************************Semaphores**************************************/
semaphore_t sem_SPIA;
/***********************************Semaphores**************************************/

/***********************************Structures**************************************/
/***********************************Structures**************************************/


/*******************************Background Threads**********************************/
void Idle_Thread(void);
void DisplayFaceRectangle(void);
/*******************************Background Threads**********************************/

/********************************Periodic Threads***********************************/
void PrintLatestImageFrameData(void);
/********************************Periodic Threads***********************************/

/*******************************Aperiodic Threads***********************************/
void BeagleboneUART_Handler(void);
void BluetoothUART_Handler(void);
/*******************************Aperiodic Threads***********************************/

#endif /* THREADS_H_ */

