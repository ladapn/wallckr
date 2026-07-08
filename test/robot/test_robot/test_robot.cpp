#include "../../common/simulated_uart.h"
#include "../../common/stop_simavr.h"
#include "MotionConstants.h"
#include "OvladackaParser.h"
#include "Robot.h"
#include "RobotPrinter.h"
#include <unity.h>

namespace {

class StubDistanceSensor : public IDistanceSensor {
public:
  unsigned long raw_cm = 100;
  unsigned long filtered_cm = 100;
  int raw_calls = 0;
  int filtered_calls = 0;

  bool trigger_measurement() override { return true; }
  unsigned long get_distance_raw_cm() override {
    raw_calls++;
    return raw_cm;
  }
  unsigned long get_distance_filtered_cm() override {
    filtered_calls++;
    return filtered_cm;
  }
};

class StubBatterySensor : public IBatterySensor {
public:
  uint16_t voltage_mVDC = 0;
  uint16_t get_battery_voltage_adc() override { return 0; }
  uint16_t get_battery_voltage_mVDC() override { return voltage_mVDC; }
};

class StubMotorController : public IMotorController {
public:
  int last_speed = 0;
  int set_speed_calls = 0;

  bool set_speed(int speed) override {
    last_speed = speed;
    set_speed_calls++;
    return true;
  }
  void stop() override {}
};

class StubSteeringServo : public ISteeringServo {
public:
  int last_angle = 0;
  int set_angle_calls = 0;

  bool set_angle(int angle) override {
    last_angle = angle;
    set_angle_calls++;
    return true;
  }
};

class StubRobotIndicators : public IRobotIndicators {
public:
  RobotIndicatorState last_state = RobotIndicatorState::IDLE;
  int indicate_robot_state_calls = 0;
  int indicate_battery_warning_calls = 0;

  int indicate_robot_state(RobotIndicatorState state) override {
    last_state = state;
    indicate_robot_state_calls++;
    return 0;
  }
  int indicate_battery_warning() override {
    indicate_battery_warning_calls++;
    return 0;
  }
};

// Pass-through regulator/filter stand-ins, same as test_auto_steering.cpp, so
// AutoSteering's output is deterministic and easy to predict here.
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

const int SIDE_DISTANCE_SETPOINT_CM = 25;
const int TURNING_RADIUS_CM = 15;
const int AVOIDING_DISTANCE_THR_CM =
    SIDE_DISTANCE_SETPOINT_CM + TURNING_RADIUS_CM;
const int FAR_DISTANCE_CM = 100; // clearly outside any threshold used below
const uint16_t BATTERY_CUTOFF_mVDC = 450;
const uint16_t BATTERY_HYSTERESIS_mVDC = 50;

} // namespace

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_perform_status_check_battery_ok_does_not_warn(void) {
  StubDistanceSensor front_sensor, right_front_sensor, right_center_sensor;
  DistanceSensors distance_sensors{front_sensor, right_front_sensor,
                                   right_center_sensor};
  SimulatedUART sensing_stream;
  RobotPrinter sensing_printer(sensing_stream);
  Sensing robot_sensing(distance_sensors, sensing_printer);

  StubBatterySensor battery_sensor;
  battery_sensor.voltage_mVDC = BATTERY_CUTOFF_mVDC + 1;
  SimulatedUART battery_stream;
  RobotPrinter battery_printer(battery_stream);
  Battery robot_battery(battery_sensor, battery_printer, BATTERY_CUTOFF_mVDC,
                        BATTERY_HYSTERESIS_mVDC);

  StubMotorController motor;
  StubSteeringServo servo;
  Motion robot_motion(motor, servo);

  IdentityRegulator regulator;
  IdentityFilter filter;
  AutoSteering<int> wall_following_steering(SIDE_DISTANCE_SETPOINT_CM,
                                            regulator, filter);

  SimulatedUART command_stream;
  OvladackaParser command_parser;
  ExternalCommandDecoder external_command_decoder(command_stream,
                                                  command_parser);

  StubRobotIndicators indicators;

  Robot robot(robot_sensing, robot_battery, robot_motion,
              wall_following_steering, external_command_decoder, indicators);

  robot.perform_status_check(0);

  TEST_ASSERT_EQUAL(0, indicators.indicate_battery_warning_calls);
}

void test_perform_status_check_battery_dead_disables_motion_and_sensing(void) {
  StubDistanceSensor front_sensor, right_front_sensor, right_center_sensor;
  DistanceSensors distance_sensors{front_sensor, right_front_sensor,
                                   right_center_sensor};
  SimulatedUART sensing_stream;
  RobotPrinter sensing_printer(sensing_stream);
  Sensing robot_sensing(distance_sensors, sensing_printer);

  StubBatterySensor battery_sensor;
  battery_sensor.voltage_mVDC = BATTERY_CUTOFF_mVDC - 1;
  SimulatedUART battery_stream;
  RobotPrinter battery_printer(battery_stream);
  Battery robot_battery(battery_sensor, battery_printer, BATTERY_CUTOFF_mVDC,
                        BATTERY_HYSTERESIS_mVDC);

  StubMotorController motor;
  StubSteeringServo servo;
  Motion robot_motion(motor, servo);

  IdentityRegulator regulator;
  IdentityFilter filter;
  AutoSteering<int> wall_following_steering(SIDE_DISTANCE_SETPOINT_CM,
                                            regulator, filter);

  SimulatedUART command_stream;
  OvladackaParser command_parser;
  ExternalCommandDecoder external_command_decoder(command_stream,
                                                  command_parser);

  StubRobotIndicators indicators;

  Robot robot(robot_sensing, robot_battery, robot_motion,
              wall_following_steering, external_command_decoder, indicators);

  robot.perform_status_check(0);
  TEST_ASSERT_EQUAL(1, indicators.indicate_battery_warning_calls);

  // Sensing should now be disabled: distance sensors must not be touched.
  robot.perform_automatic_action(0);
  TEST_ASSERT_EQUAL(0, front_sensor.filtered_calls);
  TEST_ASSERT_EQUAL(0, right_front_sensor.raw_calls);
  TEST_ASSERT_EQUAL(0, right_center_sensor.raw_calls);

  // Motion should now be disabled: motor/servo must not be commanded.
  robot.apply_motion_command();
  TEST_ASSERT_EQUAL(0, motor.set_speed_calls);
  TEST_ASSERT_EQUAL(0, servo.set_angle_calls);
}

void test_perform_automatic_action_updates_command_when_automatic(void) {
  StubDistanceSensor front_sensor, right_front_sensor, right_center_sensor;
  front_sensor.filtered_cm = FAR_DISTANCE_CM;
  // IdentityRegulator's setpoint is 0 (see its constructor below), so a
  // right distance (the min of right_front/right_center) of 0 is what makes
  // it - and the identity filter - yield a zero steering correction, i.e. a
  // centered servo. right_front is kept far away on its own so it doesn't
  // separately trip AutoSteering's close-obstacle AVOIDING threshold.
  right_front_sensor.raw_cm = FAR_DISTANCE_CM;
  right_center_sensor.raw_cm = 0;
  DistanceSensors distance_sensors{front_sensor, right_front_sensor,
                                   right_center_sensor};
  SimulatedUART sensing_stream;
  RobotPrinter sensing_printer(sensing_stream);
  Sensing robot_sensing(distance_sensors, sensing_printer);

  StubBatterySensor battery_sensor;
  battery_sensor.voltage_mVDC = BATTERY_CUTOFF_mVDC + 1;
  SimulatedUART battery_stream;
  RobotPrinter battery_printer(battery_stream);
  Battery robot_battery(battery_sensor, battery_printer, BATTERY_CUTOFF_mVDC,
                        BATTERY_HYSTERESIS_mVDC);

  StubMotorController motor;
  StubSteeringServo servo;
  Motion robot_motion(motor, servo);

  IdentityRegulator regulator;
  IdentityFilter filter;
  AutoSteering<int> wall_following_steering(SIDE_DISTANCE_SETPOINT_CM,
                                            regulator, filter);

  SimulatedUART command_stream;
  OvladackaParser command_parser;
  ExternalCommandDecoder external_command_decoder(command_stream,
                                                  command_parser);

  StubRobotIndicators indicators;

  // Robot starts in AUTOMATIC by default, no external command needed.
  Robot robot(robot_sensing, robot_battery, robot_motion,
              wall_following_steering, external_command_decoder, indicators);

  robot.perform_automatic_action(0);
  robot.apply_motion_command();

  // Right distance sits exactly at the setpoint, so the identity
  // regulator/filter both yield zero and the servo stays centered.
  TEST_ASSERT_EQUAL(SERVO_CENTER, servo.last_angle);
  TEST_ASSERT_EQUAL(static_cast<int>(RobotIndicatorState::MOVING_FOLLOWING),
                    static_cast<int>(indicators.last_state));
}

void test_perform_automatic_action_ignored_when_manual(void) {
  StubDistanceSensor front_sensor, right_front_sensor, right_center_sensor;
  // Close enough to trigger AVOIDING (and a servo angle far from center) if
  // automatic steering were mistakenly still applied.
  front_sensor.filtered_cm = AVOIDING_DISTANCE_THR_CM - 1;
  right_front_sensor.raw_cm = SIDE_DISTANCE_SETPOINT_CM;
  right_center_sensor.raw_cm = SIDE_DISTANCE_SETPOINT_CM;
  DistanceSensors distance_sensors{front_sensor, right_front_sensor,
                                   right_center_sensor};
  SimulatedUART sensing_stream;
  RobotPrinter sensing_printer(sensing_stream);
  Sensing robot_sensing(distance_sensors, sensing_printer);

  StubBatterySensor battery_sensor;
  battery_sensor.voltage_mVDC = BATTERY_CUTOFF_mVDC + 1;
  SimulatedUART battery_stream;
  RobotPrinter battery_printer(battery_stream);
  Battery robot_battery(battery_sensor, battery_printer, BATTERY_CUTOFF_mVDC,
                        BATTERY_HYSTERESIS_mVDC);

  StubMotorController motor;
  StubSteeringServo servo;
  Motion robot_motion(motor, servo);

  IdentityRegulator regulator;
  IdentityFilter filter;
  AutoSteering<int> wall_following_steering(SIDE_DISTANCE_SETPOINT_CM,
                                            regulator, filter);

  // ':f!' is the RIGHT_CIRCLE command, which toggles automatic operation off.
  char in_data[] = ":f!";
  SimulatedUART command_stream(in_data, sizeof(in_data) / sizeof(char));
  OvladackaParser command_parser;
  ExternalCommandDecoder external_command_decoder(command_stream,
                                                  command_parser);

  StubRobotIndicators indicators;

  Robot robot(robot_sensing, robot_battery, robot_motion,
              wall_following_steering, external_command_decoder, indicators);

  robot.check_external_command(); // switches to MANUAL
  robot.perform_automatic_action(0);
  robot.apply_motion_command();

  // In MANUAL, perform_automatic_action must neither update the desired
  // servo angle (still the RobotCommand default) nor drive the indicators.
  TEST_ASSERT_EQUAL(SERVO_CENTER, servo.last_angle);
  TEST_ASSERT_EQUAL(0, indicators.indicate_robot_state_calls);
}

void test_check_external_command_speed_applies_through_motion(void) {
  StubDistanceSensor front_sensor, right_front_sensor, right_center_sensor;
  DistanceSensors distance_sensors{front_sensor, right_front_sensor,
                                   right_center_sensor};
  SimulatedUART sensing_stream;
  RobotPrinter sensing_printer(sensing_stream);
  Sensing robot_sensing(distance_sensors, sensing_printer);

  StubBatterySensor battery_sensor;
  battery_sensor.voltage_mVDC = BATTERY_CUTOFF_mVDC + 1;
  SimulatedUART battery_stream;
  RobotPrinter battery_printer(battery_stream);
  Battery robot_battery(battery_sensor, battery_printer, BATTERY_CUTOFF_mVDC,
                        BATTERY_HYSTERESIS_mVDC);

  StubMotorController motor;
  StubSteeringServo servo;
  Motion robot_motion(motor, servo);

  IdentityRegulator regulator;
  IdentityFilter filter;
  AutoSteering<int> wall_following_steering(SIDE_DISTANCE_SETPOINT_CM,
                                            regulator, filter);

  // ':a!' is the UP command, which increases desired speed.
  char in_data[] = ":a!";
  SimulatedUART command_stream(in_data, sizeof(in_data) / sizeof(char));
  OvladackaParser command_parser;
  ExternalCommandDecoder external_command_decoder(command_stream,
                                                  command_parser);

  StubRobotIndicators indicators;

  Robot robot(robot_sensing, robot_battery, robot_motion,
              wall_following_steering, external_command_decoder, indicators);

  robot.check_external_command();
  robot.apply_motion_command();

  TEST_ASSERT_EQUAL(SPEED_INCREMENT, motor.last_speed);
  TEST_ASSERT_EQUAL(1, motor.set_speed_calls);
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_perform_status_check_battery_ok_does_not_warn);
  RUN_TEST(test_perform_status_check_battery_dead_disables_motion_and_sensing);
  RUN_TEST(test_perform_automatic_action_updates_command_when_automatic);
  RUN_TEST(test_perform_automatic_action_ignored_when_manual);
  RUN_TEST(test_check_external_command_speed_applies_through_motion);

  UNITY_END();

  stop_simavr();

  return 0;
}
