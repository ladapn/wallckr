#include <unity.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "TimeManager.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void testStatusCheckTrue(void) {
    TimeManager time_manager;

    // first pass should always return true
    TEST_ASSERT_TRUE(time_manager.isTimeForStatusCheck(0));

    const unsigned long TIME_FOR_STATUS = 1001;
    TEST_ASSERT_TRUE(time_manager.isTimeForStatusCheck(TIME_FOR_STATUS));
}

void testStatusCheckFalse(void) {
    TimeManager time_manager;

    // first pass should always return true
    time_manager.isTimeForStatusCheck(0);

    const unsigned long NOT_TIME_FOR_STATUS = 100;
    TEST_ASSERT_FALSE(time_manager.isTimeForStatusCheck(NOT_TIME_FOR_STATUS));
}

int main( int argc, char **argv) {
    UNITY_BEGIN();
   
    RUN_TEST(testStatusCheckTrue);
    RUN_TEST(testStatusCheckFalse);

    UNITY_END();

    // Needed to stop simavr simulator after the test cases
    cli();
    sleep_cpu();

    return 0; 
}



