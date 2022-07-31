#include <unity.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "Regulator.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_assert_equal(int expected, int actual)
{
    TEST_ASSERT_EQUAL(expected, actual); 
}

void test_assert_equal(float expected, float actual)
{
    // TEST_ASSERT_EQUAL_FLOAT computes delta (epsilon) on the fly: expected * 0.00001
    TEST_ASSERT_EQUAL_FLOAT(expected, actual);
}

template<typename T>
void test_step(int N, const T input[], const T expected[], unsigned len)
{
    ExpFilter<T> filter{N};
    for(unsigned i = 0; i < len; i++)
    {
        auto out = filter.next(input[i]);

        test_assert_equal(expected[i], out);
    }
}

void test_step_N_4_int(void)
{
    const int FILTER_N = 4;
    const int input[] = {0, 0, 0, 0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    const int expected[] = {0, 0, 0, 0, 25, 44, 58, 69, 77, 83, 87, 90, 93, 95, 96, 97, 98, 99, 99};

    test_step(FILTER_N, input, expected, sizeof(input)/sizeof(input[0])); 
}

void test_step_N_10_int(void)
{
    const int FILTER_N = 10;
    const int input[] = {0, 0, 0, 0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    const int expected[] = {0, 0, 0, 0, 10, 19, 27, 34, 40, 46, 51, 55, 59, 63, 66, 69, 72, 74, 76};

    test_step(FILTER_N, input, expected, sizeof(input)/sizeof(input[0])); 
}

int main( int argc, char **argv) {
    UNITY_BEGIN();
   
    RUN_TEST(test_step_N_4_int);
    RUN_TEST(test_step_N_10_int);

    UNITY_END();

    // Needed to stop simavr simulator after the test cases
    cli();
    sleep_cpu();

    return 0; 
}



