#include "../../common/stop_simavr.h"
#include "TimeManager.h"
#include <unity.h>

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_status_check_true(void) {
  TimeManager time_manager;

  // first pass should always return true
  TEST_ASSERT_TRUE(time_manager.is_time_for_status_check(0));

  const unsigned long TIME_FOR_STATUS = 1001;
  TEST_ASSERT_TRUE(time_manager.is_time_for_status_check(TIME_FOR_STATUS));
}

void test_status_check_false(void) {
  TimeManager time_manager;

  // first pass should always return true
  time_manager.is_time_for_status_check(0);

  const unsigned long NOT_TIME_FOR_STATUS = 100;
  TEST_ASSERT_FALSE(time_manager.is_time_for_status_check(NOT_TIME_FOR_STATUS));
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_status_check_true);
  RUN_TEST(test_status_check_false);

  UNITY_END();

  stop_simavr();

  return 0;
}
