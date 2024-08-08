#include <zephyr/ztest.h>
#include "ExpFilter.h"

ZTEST_SUITE(exp_fileter_tests, NULL, NULL, NULL, NULL, NULL);


// Encapsulate ASSERT_EQUAL macros to function with different argument types so
// that overloading can be used to select the macros  
void test_assert_equal(int expected, int actual)
{
    zassert_equal(expected, actual); 
}

void test_assert_equal(float expected, float actual)
{
    // Apparently ZTEST does not have anything to compare floats/doubles
    // But it can test if a difference of two values is within certain range 
    // Inspiration taken from unity's TEST_ASSERT_EQUAL_FLOAT that computes delta (epsilon) 
    // on the fly like so: expected * 0.00001
    float epsilon = expected * 0.00001;
    zassert_within(expected, actual, epsilon);
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


ZTEST(exp_fileter_tests, test_step_N_4_int)
{
    const int FILTER_N = 4;
    const int input[] = {0, 0, 0, 0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    const int expected[] = {0, 0, 0, 0, 25, 44, 58, 69, 77, 83, 87, 90, 93, 95, 96, 97, 98, 99, 99};

    test_step(FILTER_N, input, expected, sizeof(input)/sizeof(input[0])); 
}

ZTEST(exp_fileter_tests, test_step_N_4_float)
{
    const int FILTER_N = 4;
    const float input[] = {0, 0, 0, 0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    // FIXME update expected results to float ones
    const float expected[] = {0, 0, 0, 0, 25, 43.75, 57.8125, 68.359375, 76.26953125, 82.2021484375, 86.651611328125, 
                              89.9887084960938, 92.4915313720703, 94.3686485290527, 95.7764863967896, 96.8323647975922,
                              97.6242735981941, 98.2182051986456, 98.6636538989842
};

    test_step(FILTER_N, input, expected, sizeof(input)/sizeof(input[0])); 
}

ZTEST(exp_fileter_tests, test_step_N_10_int)
{
    const int FILTER_N = 10;
    const int input[] = {0, 0, 0, 0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    const int expected[] = {0, 0, 0, 0, 10, 19, 27, 34, 40, 46, 51, 55, 59, 63, 66, 69, 72, 74, 76};

    test_step(FILTER_N, input, expected, sizeof(input)/sizeof(input[0])); 
}

ZTEST(exp_fileter_tests, test_step_N_10_float)
{
    const int FILTER_N = 10;
    const float input[] = {0, 0, 0, 0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
    // FIXME update expected results to float ones
    const float expected[] = {0, 0, 0, 0, 10, 19, 27.1, 34.39, 40.951, 46.8559, 52.17031, 56.953279, 61.2579511, 
                              65.13215599, 68.618940391, 71.7570463519, 74.58134171671, 77.123207545039,
                              79.4108867905351};

    test_step(FILTER_N, input, expected, sizeof(input)/sizeof(input[0])); 
}



