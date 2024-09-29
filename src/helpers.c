#include "helpers.h"
#include "FreeRTOS.h"
#include "semphr.h"

void side_counter( int* count, SemaphoreHandle_t sem )
{
    BaseType_t retStatus =  xSemaphoreTake( sem, 500 );
    if( retStatus == pdPASS )
    {
        *count += *count + 1;
        printf("hello world from %s! Count %u\n", "thread", *count);
        xSemaphoreGive( sem );
    }
}

void main_counter( int* count, SemaphoreHandle_t sem )
{
    BaseType_t retStatus =  xSemaphoreTake( sem, 500 );
    if( retStatus == pdPASS )
    {
        printf("hello world from %s! Count %u\n", "main", (*count)++);
        xSemaphoreGive( sem );
    }
}

void deadlock_one(void* args){

    struct Deadlock_Args *dargs = (struct Deadlock_Args *)args;
    BaseType_t retStatus = xSemaphoreTake(dargs->one, portMAX_DELAY);
    if (retStatus == pdPASS){
        printf("Deadlock_one: Got the first one!\n");
        vTaskDelay(100);
        retStatus = xSemaphoreTake(dargs->two, portMAX_DELAY);
        if (retStatus == pdPASS){
            printf("Deadlock_one: Got the second!\n");
        }
        xSemaphoreGive(dargs->two);
    }
    xSemaphoreGive(dargs->one);
    vTaskSuspend(NULL);
}

void deadlock_two(void* args){

    struct Deadlock_Args *dargs = (struct Deadlock_Args *)args;
    BaseType_t retStatus = xSemaphoreTake(dargs->two, portMAX_DELAY);
    if (retStatus == pdPASS){
        printf("Deadlock_two: Got the first one!\n");
        vTaskDelay(100);
        retStatus = xSemaphoreTake(dargs->one, portMAX_DELAY);
        if (retStatus == pdPASS){
            printf("Deadlock_two: Got the second!\n");
        }
        xSemaphoreGive(dargs->one);
    }
    xSemaphoreGive(dargs->two);
    vTaskSuspend(NULL);
}