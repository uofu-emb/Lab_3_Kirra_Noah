#include <stdio.h>
#include "FreeRTOS.h"
#include "semphr.h"

struct Deadlock_Args{
    xSemaphoreHandle one;
    xSemaphoreHandle two;
};

void side_counter( int* count, SemaphoreHandle_t sem );

void main_counter( int* count, SemaphoreHandle_t sem );

void deadlock_one(void* args);

void deadlock_two(void* args);