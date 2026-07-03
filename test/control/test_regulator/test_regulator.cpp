#include <unity.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "Regulator.h"
#include "../../common/stop_simavr.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_p_regulator_within_insensitivity_band_is_zero(void) {
    Regulator_P<int> regulator(5, 25, 2);

    TEST_ASSERT_EQUAL(0, regulator.action(24)); // e = 1, within insensitivity
}

void test_p_regulator_applies_gain(void) {
    Regulator_P<int> regulator(5, 25, 2);

    TEST_ASSERT_EQUAL(25, regulator.action(20)); // e = 5, out = 5 * 5
}

void test_p_regulator_clamps_to_upper_limit(void) {
    Regulator_P<int> regulator(10, 25, 2);

    TEST_ASSERT_EQUAL(45, regulator.action(0)); // e = 25, raw out = 250
}

void test_p_regulator_clamps_to_lower_limit(void) {
    Regulator_P<int> regulator(10, 25, 2);

    TEST_ASSERT_EQUAL(-30, regulator.action(50)); // e = -25, raw out = -250
}

void test_pd_regulator_first_action_ignores_derivative(void) {
    Regulator_PD<int> regulator(2, 3, 25, 2);

    TEST_ASSERT_EQUAL(10, regulator.action(20)); // e = 5, out = gain * e
}

void test_pd_regulator_adds_derivative_term_on_subsequent_action(void) {
    Regulator_PD<int> regulator(2, 3, 25, 2);

    regulator.action(20); // e = 5, primes derivative state (last_e = 5)
    TEST_ASSERT_EQUAL(35, regulator.action(15)); // e = 10, out = 2*10 + (10-5)*3
}

void test_pd_regulator_keeps_last_error_across_insensitivity_band(void) {
    Regulator_PD<int> regulator(1, 1, 25, 2);

    regulator.action(20); // e = 5, primes derivative state (last_e = 5)
    regulator.action(25); // e = 0, inside insensitivity band, derivative state left untouched
    // e = 6; derivative term uses the stale last_e (5) rather than the skipped e (0)
    TEST_ASSERT_EQUAL(7, regulator.action(19)); // out = 1*6 + (6-5)*1
}

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_p_regulator_within_insensitivity_band_is_zero);
    RUN_TEST(test_p_regulator_applies_gain);
    RUN_TEST(test_p_regulator_clamps_to_upper_limit);
    RUN_TEST(test_p_regulator_clamps_to_lower_limit);

    RUN_TEST(test_pd_regulator_first_action_ignores_derivative);
    RUN_TEST(test_pd_regulator_adds_derivative_term_on_subsequent_action);
    RUN_TEST(test_pd_regulator_keeps_last_error_across_insensitivity_band);

    UNITY_END();

    // Needed to stop simavr simulator after the test cases
    cli();
    sleep_cpu();

    return 0;
}
