#include <stdio.h>
#include "FreeRTOS.h"
#include "semphr.h"

void side_counter( int* count, SemaphoreHandle_t sem );

void main_counter( int* count, SemaphoreHandle_t sem );