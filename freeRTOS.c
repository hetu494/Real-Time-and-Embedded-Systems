 /*Name: HETANSH MADHANI
 */

/* Standard includes */
#include <stdio.h>

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "Queue.h"
#include "semphr.h"
/* global variables */
/* (Declare your global variables here) */
xQueueHandle numberQueue=0;
xSemaphoreHandle lock=0; //semaphore to give access to function to increase globalCount
/* function prototypes */
unsigned int globalCount = 0;
void increase_globalCount()   //function to increase globalCount
{

	/*printf("count value:%i\n", globalCount);*/
	globalCount +=1;
}
/* (Add your function prototypes here) */
void TxTask1(void *p)
{
	(void) p;

	while(1){

		if(xSemaphoreTake(lock,1000)) //wait for semaphore
				{
				printf("task 1 has access to globalCount \n");
				printf("Sending %i to queue\n", globalCount);
					if( ! xQueueSend(numberQueue, &globalCount, 10))// check if sending to queue failed
					{
						printf("failed to send to queue\n");
					}
					increase_globalCount();//increase global count
					xSemaphoreGive(lock);//release the resource

				}

				vTaskDelay(20);
			}
}

void TxTask2(void *p)
{
	(void) p;

	while(1){
		if(xSemaphoreTake(lock,2000))// check for semaphore and give access to this task for globalCount
		{
			printf("Task 2 has access to globalCount\n");
			printf("Sending %i to queue\n", globalCount);
			if( ! xQueueSend(numberQueue, &globalCount, 10))//check if sending to queue failed
			{
				printf("failed to send to queue\n");
			}
			increase_globalCount(); //increment the globalCount againt
			xSemaphoreGive(lock);//release those resources for another task
		}
		vTaskDelay(20);
	}
}

void RxTask(void *p)
{
	(void) p;
	int rx_globalCount = 0;
	while(1){
		if(xQueueReceive(numberQueue, &rx_globalCount, 10))//if data in queue print that data
		{
		printf("recieved %i\n", rx_globalCount);
	}
		vTaskDelay(20);
}
}
/* main application code */
int main( void )
{	//int i;
	lock = xSemaphoreCreateMutex(); //creating semaphore
	numberQueue = xQueueCreate(10, sizeof(int)); //greating queue
	xTaskCreate(TxTask1, ( const char * const ) "tx1", 1024, NULL, 1, NULL);//task creationg

	xTaskCreate(TxTask2, ( const char * const ) "tx2", 1024, NULL, 1, NULL);
		xTaskCreate(RxTask, ( const char * const ) "rx", 1024, NULL, 1, NULL);
		xTaskCreate(RxTask, ( const char * const ) "rx", 1024, NULL, 1, NULL);

	/* (Add your application code here) */
    vTaskStartScheduler();
	return 0;
}

/* (Add any additional function definitions here) */
