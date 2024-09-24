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