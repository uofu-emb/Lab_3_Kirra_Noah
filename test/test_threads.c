#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"
#include "helpers.h"

// Setting the priority of the main task to be 1
#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

// Setting the priority of the side task to be the same as main. ==1.
#define SIDE_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define SIDE_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

TaskHandle_t threadlock_one, threadlock_two, deadlock_check;

void setUp(void) {

}

void tearDown(void) {}

void test_side_counter_success()
{
    SemaphoreHandle_t sem = xSemaphoreCreateCounting(1, 1);
    int count = 0;
    side_counter(&count, sem);
    TEST_ASSERT_TRUE_MESSAGE(count == 1, "FAIL: Did not take Semaphore.");
    TEST_ASSERT_TRUE_MESSAGE(uxSemaphoreGetCount(sem) == 1, "FAIL: Did not release semaphore");
}


void test_side_counter_fail(){
    SemaphoreHandle_t sem = xSemaphoreCreateCounting(1, 1);
    int count = 0;
    xSemaphoreTake(sem, 0);
    side_counter(&count, sem);
    TEST_ASSERT_TRUE_MESSAGE(count == 0, "FAIL: Did take Semaphore.");
}

void test_main_counter_success(void)
{
    SemaphoreHandle_t sem = xSemaphoreCreateCounting(1, 1);
    int count = 0;
    main_counter(&count, sem);
    TEST_ASSERT_TRUE_MESSAGE(count == 1, "FAIL: Did not take Semaphore.");
    TEST_ASSERT_TRUE_MESSAGE(uxSemaphoreGetCount(sem) == 1, "FAIL: Did not release semaphore");
}

void test_main_counter_fail(){
    SemaphoreHandle_t sem = xSemaphoreCreateCounting(1, 1);
    int count = 0;
    xSemaphoreTake(sem, 0);
    main_counter(&count, sem);
    TEST_ASSERT_TRUE_MESSAGE(count == 0, "FAIL: Did take Semaphore.");
}

void test_deadlock_checker(void* args){
    printf("Entered the deadlock checker!\n");
    vTaskDelay(1000);
    printf("Delayed for 1000 ticks.\n");
    struct Deadlock_Args *dargs = (struct Deadlock_Args*)args;

    TEST_ASSERT_TRUE_MESSAGE(uxSemaphoreGetCount(dargs->one) == 0, "Fail: Did not deadlock.");
    TEST_ASSERT_TRUE_MESSAGE(uxSemaphoreGetCount(dargs->two) == 0, "FAIL: Did not deadlock 2.0");

    vTaskDelete(threadlock_one);
    vTaskDelete(threadlock_two);
}

void test_deadlock(){
    
    // Creating the semaphore
   SemaphoreHandle_t one = xSemaphoreCreateCounting(1, 1);
   SemaphoreHandle_t two = xSemaphoreCreateCounting(1, 1);

   struct Deadlock_Args dargs = {one, two};

    // Put main thread on the stack of tasks
    xTaskCreate(deadlock_one, "Deadlock_One",
                MAIN_TASK_STACK_SIZE, &dargs, MAIN_TASK_PRIORITY, &threadlock_one);
    // Put side thread on the stack of tasks
    xTaskCreate(deadlock_two, "Deadlock_two",
                SIDE_TASK_STACK_SIZE, &dargs, SIDE_TASK_PRIORITY, &threadlock_two);
    xTaskCreate(test_deadlock_checker, "Deadlock_checker",
                SIDE_TASK_STACK_SIZE, &dargs, SIDE_TASK_PRIORITY, &deadlock_check);
    vTaskStartScheduler();
    
}

int main (void)
{
    stdio_init_all();
    sleep_ms(8000); // Give time for TTY to attach.
    printf("Start tests\n");
    UNITY_BEGIN();
    RUN_TEST(test_side_counter_success);
    RUN_TEST(test_side_counter_fail);
    RUN_TEST(test_main_counter_success);
    RUN_TEST(test_main_counter_fail);
    RUN_TEST(test_deadlock);
    sleep_ms(8000);
    return UNITY_END();
}