#ifndef POLLING_ROBOT_RUNNER_H
#define POLLING_ROBOT_RUNNER_H

#include "Robot.h"
#include "TimeManager.h"

class PollingRobotRunner {
  Robot &robot;
  TimeManager time_manager;

public:
  PollingRobotRunner(Robot &robot) : robot(robot) {}
  void run_once(uint32_t now_ms) {
    robot.check_external_command();
    robot.apply_motion_command();

    if (time_manager.is_time_for_status_check(now_ms)) {
      robot.perform_status_check(now_ms);
    }

    if (time_manager.is_time_for_automatic_command(now_ms)) {
      robot.perform_automatic_action(now_ms);
    }
  }
};

#endif // POLLING_ROBOT_RUNNER_H
