#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"
#include "helpers.h"


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

int main (void)
{
    stdio_init_all();
    sleep_ms(10000); // Give time for TTY to attach.
    printf("Start tests\n");
    UNITY_BEGIN();
    RUN_TEST(test_side_counter_success);
    RUN_TEST(test_side_counter_fail);
    RUN_TEST(test_main_counter_success);
    RUN_TEST(test_main_counter_fail);
    sleep_ms(10000);
    return UNITY_END();
}