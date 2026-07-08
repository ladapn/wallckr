#include "../../common/stop_simavr.h"
#include "Regulator.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <unity.h>

namespace {
const int SETPOINT_CM = 25;
const int INSENSITIVITY_CM = 2;
} // namespace

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_p_regulator_within_insensitivity_band_is_zero(void) {
  const int GAIN = 5;
  Regulator_P<int> regulator(GAIN, SETPOINT_CM, INSENSITIVITY_CM);

  TEST_ASSERT_EQUAL(
      0, regulator.action(SETPOINT_CM - 1)); // e = 1, within insensitivity
}

void test_p_regulator_applies_gain(void) {
  const int GAIN = 5;
  const int ERROR_CM = 5;
  Regulator_P<int> regulator(GAIN, SETPOINT_CM, INSENSITIVITY_CM);

  TEST_ASSERT_EQUAL(GAIN * ERROR_CM, regulator.action(SETPOINT_CM - ERROR_CM));
}

void test_p_regulator_clamps_to_upper_limit(void) {
  const int GAIN = 10;
  const int ACTION_LIMIT_UP =
      45; // mirrors Regulator's internal m_action_limit_up
  Regulator_P<int> regulator(GAIN, SETPOINT_CM, INSENSITIVITY_CM);

  TEST_ASSERT_EQUAL(ACTION_LIMIT_UP,
                    regulator.action(0)); // e = 25, raw out = 250
}

void test_p_regulator_clamps_to_lower_limit(void) {
  const int GAIN = 10;
  const int ACTION_LIMIT_BOTTOM =
      -30; // mirrors Regulator's internal m_action_limit_bottom
  Regulator_P<int> regulator(GAIN, SETPOINT_CM, INSENSITIVITY_CM);

  TEST_ASSERT_EQUAL(ACTION_LIMIT_BOTTOM,
                    regulator.action(50)); // e = -25, raw out = -250
}

void test_pd_regulator_first_action_ignores_derivative(void) {
  const int GAIN = 2;
  const int D_GAIN = 3;
  const int ERROR_CM = 5;
  Regulator_PD<int> regulator(GAIN, D_GAIN, SETPOINT_CM, INSENSITIVITY_CM);

  TEST_ASSERT_EQUAL(GAIN * ERROR_CM, regulator.action(SETPOINT_CM - ERROR_CM));
}

void test_pd_regulator_adds_derivative_term_on_subsequent_action(void) {
  const int GAIN = 2;
  const int D_GAIN = 3;
  const int FIRST_ERROR_CM = 5;
  const int SECOND_ERROR_CM = 10;
  Regulator_PD<int> regulator(GAIN, D_GAIN, SETPOINT_CM, INSENSITIVITY_CM);

  regulator.action(
      SETPOINT_CM -
      FIRST_ERROR_CM); // primes derivative state (last_e = FIRST_ERROR_CM)
  TEST_ASSERT_EQUAL(GAIN * SECOND_ERROR_CM +
                        (SECOND_ERROR_CM - FIRST_ERROR_CM) * D_GAIN,
                    regulator.action(SETPOINT_CM - SECOND_ERROR_CM));
}

void test_pd_regulator_keeps_last_error_across_insensitivity_band(void) {
  const int GAIN = 1;
  const int D_GAIN = 1;
  const int FIRST_ERROR_CM = 5;
  const int THIRD_ERROR_CM = 6;
  Regulator_PD<int> regulator(GAIN, D_GAIN, SETPOINT_CM, INSENSITIVITY_CM);

  regulator.action(
      SETPOINT_CM -
      FIRST_ERROR_CM); // primes derivative state (last_e = FIRST_ERROR_CM)
  regulator.action(SETPOINT_CM); // e = 0, inside insensitivity band, derivative
                                 // state left untouched
  // derivative term uses the stale last_e (FIRST_ERROR_CM) rather than the
  // skipped e (0)
  TEST_ASSERT_EQUAL(GAIN * THIRD_ERROR_CM +
                        (THIRD_ERROR_CM - FIRST_ERROR_CM) * D_GAIN,
                    regulator.action(SETPOINT_CM - THIRD_ERROR_CM));
}

int main(int argc, char **argv) {
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
