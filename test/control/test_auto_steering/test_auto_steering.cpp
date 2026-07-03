#include <unity.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "AutoSteering.h"
#include "../../common/stop_simavr.h"

namespace {
    // Pass-through regulator/filter stand-ins so tests can exercise
    // AutoSteering's FOLLOWING/AVOIDING state machine in isolation from
    // the regulator and filter math, which are covered by their own tests.
    class IdentityRegulator : public Regulator<int> {
    protected:
        int regulator_equation(int e) override { return e; }

    public:
        IdentityRegulator() : Regulator<int>(1, 0, 0) {}
    };

    class IdentityFilter : public IFilter<int> {
    public:
        int next(int input) override { return input; }
    };
}

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_starts_in_following_state(void) {
    IdentityRegulator regulator;
    IdentityFilter filter;
    AutoSteering<int> auto_steering(25, regulator, filter);

    TEST_ASSERT_EQUAL(static_cast<int>(SteeringState::FOLLOWING),
                      static_cast<int>(auto_steering.get_steering_state()));
}

void test_following_enters_avoiding_on_close_front_obstacle(void) {
    IdentityRegulator regulator;
    IdentityFilter filter;
    AutoSteering<int> auto_steering(25, regulator, filter);

    // AVOIDING_DISTANCE_THR_CM = setpoint (25) + turning radius (15) = 40
    DistanceMeasurements measurements;
    measurements.front_distance_cm = 39; // < 40
    measurements.right_front_distance_cm = 100;
    measurements.right_distance_cm = 25;

    auto_steering.get_steering_command(measurements);

    TEST_ASSERT_EQUAL(static_cast<int>(SteeringState::AVOIDING),
                      static_cast<int>(auto_steering.get_steering_state()));
}

void test_following_enters_avoiding_on_close_right_front_obstacle(void) {
    IdentityRegulator regulator;
    IdentityFilter filter;
    AutoSteering<int> auto_steering(25, regulator, filter);

    DistanceMeasurements measurements;
    measurements.front_distance_cm = 100;
    measurements.right_front_distance_cm = 14; // < right front threshold (15)
    measurements.right_distance_cm = 25;

    auto_steering.get_steering_command(measurements);

    TEST_ASSERT_EQUAL(static_cast<int>(SteeringState::AVOIDING),
                      static_cast<int>(auto_steering.get_steering_state()));
}

void test_avoiding_commands_max_left_steering(void) {
    IdentityRegulator regulator;
    IdentityFilter filter;
    AutoSteering<int> auto_steering(25, regulator, filter);

    DistanceMeasurements measurements;
    measurements.front_distance_cm = 10;
    measurements.right_front_distance_cm = 100;
    measurements.right_distance_cm = 25;

    auto_steering.get_steering_command(measurements); // FOLLOWING -> AVOIDING
    auto servo = auto_steering.get_steering_command(measurements); // stays AVOIDING

    TEST_ASSERT_EQUAL(SERVO_MAX_LEFT, servo);
}

void test_avoiding_returns_to_following_once_clear(void) {
    IdentityRegulator regulator;
    IdentityFilter filter;
    AutoSteering<int> auto_steering(25, regulator, filter);

    DistanceMeasurements close_measurements;
    close_measurements.front_distance_cm = 10;
    close_measurements.right_front_distance_cm = 100;
    close_measurements.right_distance_cm = 25;
    auto_steering.get_steering_command(close_measurements); // enter AVOIDING

    // Recovery needs front > 40 + 20 = 60 and right_front > 40 - 15 = 25
    DistanceMeasurements clear_measurements;
    clear_measurements.front_distance_cm = 61;
    clear_measurements.right_front_distance_cm = 26;
    clear_measurements.right_distance_cm = 25;

    auto_steering.get_steering_command(clear_measurements);

    TEST_ASSERT_EQUAL(static_cast<int>(SteeringState::FOLLOWING),
                      static_cast<int>(auto_steering.get_steering_state()));
}

void test_avoiding_stays_avoiding_until_both_thresholds_clear(void) {
    IdentityRegulator regulator;
    IdentityFilter filter;
    AutoSteering<int> auto_steering(25, regulator, filter);

    DistanceMeasurements close_measurements;
    close_measurements.front_distance_cm = 10;
    close_measurements.right_front_distance_cm = 100;
    close_measurements.right_distance_cm = 25;
    auto_steering.get_steering_command(close_measurements); // enter AVOIDING

    // Front is clear, but right front is still too close
    DistanceMeasurements partially_clear_measurements;
    partially_clear_measurements.front_distance_cm = 61;
    partially_clear_measurements.right_front_distance_cm = 24; // <= 25
    partially_clear_measurements.right_distance_cm = 25;

    auto_steering.get_steering_command(partially_clear_measurements);

    TEST_ASSERT_EQUAL(static_cast<int>(SteeringState::AVOIDING),
                      static_cast<int>(auto_steering.get_steering_state()));
}

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_starts_in_following_state);
    RUN_TEST(test_following_enters_avoiding_on_close_front_obstacle);
    RUN_TEST(test_following_enters_avoiding_on_close_right_front_obstacle);
    RUN_TEST(test_avoiding_commands_max_left_steering);
    RUN_TEST(test_avoiding_returns_to_following_once_clear);
    RUN_TEST(test_avoiding_stays_avoiding_until_both_thresholds_clear);

    UNITY_END();

    // Needed to stop simavr simulator after the test cases
    cli();
    sleep_cpu();

    return 0;
}
