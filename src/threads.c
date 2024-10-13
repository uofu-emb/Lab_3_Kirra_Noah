#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>
#include "helpers.h"

// Setting the priority of the main task to be 1
#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

// Setting the priority of the side task to be the same as main. ==1.
#define SIDE_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define SIDE_TASK_STACK_SIZE configMINIMAL_STACK_SIZE


SemaphoreHandle_t semaphore;

// counter is a shared variable
int counter;

// on controls the state of the LED
int on;

// Entry point for the side_thread execution context.
// This thread will delay for .1s then update the
// counter.
void side_thread(void *params)
{
	while (1) {
        vTaskDelay(100);

        // BaseType_t retStatus =  xSemaphoreTake( semaphore, 500 );
        // if( retStatus == pdPASS )
        // {
        //     counter += counter + 1;
        //     printf("hello world from %s! Count %u\n", "thread", counter);
        //     xSemaphoreGive( semaphore );
        // }
        side_counter( &counter, semaphore );

	}
}

// Entry point for the main_thread exectuion context.
// This thread will delay for .1s then update the
// counter.
void main_thread(void *params)
{
	while (1) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
        vTaskDelay(100);
        // BaseType_t retStatus =  xSemaphoreTake( semaphore, 500 );
        // if( retStatus == pdPASS )
        // {
        //     printf("hello world from %s! Count %u\n", "main", counter++);
        //     xSemaphoreGive( semaphore );
        // }
        main_counter(&counter, semaphore);
        on = !on;
	}
}

// Main entry point to our program. This function sets up
// the semaphore with a max count of 1 and and initial count
// of 1.
int main(void)
{
    stdio_init_all();
    hard_assert(cyw43_arch_init() == PICO_OK);
    on = false;
    counter = 0;
    TaskHandle_t main, side;
    // Creating the semaphore
    semaphore = xSemaphoreCreateCounting(1, 1);

    // Put main thread on the stack of tasks
    xTaskCreate(main_thread, "MainThread",
                MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &main);
    
    // Put side thread on the stack of tasks
    xTaskCreate(side_thread, "SideThread",
                SIDE_TASK_STACK_SIZE, NULL, SIDE_TASK_PRIORITY, &side);
    vTaskStartScheduler();
	return 0;
}
