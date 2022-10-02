#include <unity.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "ExternalCommandDecoder.h"
#include "OvladackaParser.h"
#include "BLEJoystickDecoder.h"
#include "RobotCommand.h"
#include "simulated_uart.h"
#include "MotionConstants.h"
#include "../../common/stop_simavr.h"


void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_simulated_uart(void)
{
    char in_data[] = "abcdEFGH";
    auto num_of_elements = sizeof(in_data) / sizeof(char);
    SimulatedUART sim_uart(in_data, num_of_elements);

    char out_data[num_of_elements];
    for (unsigned long i = 0; i < num_of_elements; i++)
    {
        out_data[i] = sim_uart.read();
    }

    TEST_ASSERT_EQUAL_CHAR_ARRAY(in_data, out_data, num_of_elements);

}

void test_check_external_command_speed_ovladacka(void)
{
    OvladackaParser in_parser;
    char in_data[] = "a";
    SimulatedUART sim_uart(in_data, sizeof(in_data) / sizeof(char)); 
    ExternalCommandDecoder ext_comm_dec(sim_uart, in_parser);

    RobotCommand rob_cmd_actual;
    RobotCommand rob_cmd_expected;  

    rob_cmd_expected.desired_speed = SPD_INCREMENT;

    ext_comm_dec.check_external_command(rob_cmd_actual);

    TEST_ASSERT_EQUAL_MEMORY(&rob_cmd_expected, &rob_cmd_actual, sizeof(rob_cmd_actual));
}

void test_check_external_command_angle_ovladacka(void)
{
    OvladackaParser in_parser;
    char in_data[] = "b";
    SimulatedUART sim_uart(in_data, sizeof(in_data) / sizeof(char)); 
    ExternalCommandDecoder ext_comm_dec(sim_uart, in_parser);

    RobotCommand rob_cmd_actual;
    RobotCommand rob_cmd_expected;  

    rob_cmd_expected.desired_servo_angle = SERVO_CENTER - SERVO_INCREMENT;

    ext_comm_dec.check_external_command(rob_cmd_actual);

    TEST_ASSERT_EQUAL_MEMORY(&rob_cmd_expected, &rob_cmd_actual, sizeof(rob_cmd_actual));
}

void test_check_external_command_speed_BLE_joy(void)
{
    BLEJoystickDecoder in_parser;
    char in_data[] = "Aa0"; // Command to increase speed in BLE Joystick format
    SimulatedUART sim_uart(in_data, sizeof(in_data) / sizeof(char)); 
    ExternalCommandDecoder ext_comm_dec(sim_uart, in_parser);

    RobotCommand rob_cmd_actual;
    RobotCommand rob_cmd_expected;  

    rob_cmd_expected.desired_speed = 50;

    ext_comm_dec.check_external_command(rob_cmd_actual);

    TEST_ASSERT_EQUAL_MEMORY(&rob_cmd_expected, &rob_cmd_actual, sizeof(rob_cmd_actual));
}

void test_check_external_command_angle_BLE_joy(void)
{
    OvladackaParser in_parser;
    char in_data[] = "Bb0"; // Command to decrease servo angle in BLE Joystick format
    SimulatedUART sim_uart(in_data, sizeof(in_data) / sizeof(char)); 
    ExternalCommandDecoder ext_comm_dec(sim_uart, in_parser);

    RobotCommand rob_cmd_actual;
    RobotCommand rob_cmd_expected;  

    rob_cmd_expected.desired_servo_angle = 70;

    ext_comm_dec.check_external_command(rob_cmd_actual);

    TEST_ASSERT_EQUAL_MEMORY(&rob_cmd_expected, &rob_cmd_actual, sizeof(rob_cmd_actual));
}


int main( int argc, char **argv) {
    UNITY_BEGIN();
   
    RUN_TEST(test_simulated_uart);
    RUN_TEST(test_check_external_command_speed_ovladacka);
    //RUN_TEST(test_check_external_command_angle_ovladacka);

    //RUN_TEST(test_check_external_command_speed_BLE_joy);
    //RUN_TEST(test_check_external_command_angle_BLE_joy);
    
    UNITY_END();

    stop_simavr();

    return 0; 
}
