#include <unity.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "../../common/simulated_uart.h"
#include "../../common/stop_simavr.h"
#include "RobotPackets.h"
#include "RobotCommand.h"
#include "MotionConstants.h"


void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_status_packet(void)
{
    const int CURRENT_TICK = 1000; 
    const int BATTERY_VOLTAGE = 100;
    const int TOTAL_CURRENT = 0;
    const int MOTOR_CURRENT = 0; 
    StatusPacket status_packet(CURRENT_TICK, BATTERY_VOLTAGE, TOTAL_CURRENT, MOTOR_CURRENT);
    status_packet.version_ID = 0xAB;
    // NOTE: AVR is little endian, i.e. LSB goes first
    uint8_t expected[] = {0x50,                     // id 
                          0xE8, 0x03, 0x00, 0x00,   // tick
                          0xAB, 0x00, 0x00, 0x00,   // version ID
                          0x64, 0x00,               // battery voltage
                          0x00, 0x00,               // total current
                          0x00, 0x00,               // motor current
                          0x74};                    // CRC    

    SimulatedUART sim_uart;
    RobotPrinter printer(sim_uart);

    printer.add_CRC_and_print(reinterpret_cast<uint8_t*>(&status_packet), sizeof(status_packet));

    uint8_t actual[sizeof(expected)];
    for(unsigned i = 0; i < sizeof(expected); i++)
    {
        actual[i] = sim_uart.read();
    }    
    
    TEST_ASSERT_EQUAL_MEMORY(expected, actual, sizeof(expected));
}

void test_sonar_packet(void)
{    
    const int PACKET_ID = 101; 
    const unsigned long CURRENT_TICK = 1000; 
    const unsigned long MEASUREMENT = 0xdeadbeef;

    SonarPacket sonar_packet(PACKET_ID, CURRENT_TICK, MEASUREMENT); 
    // NOTE: AVR is little endian, i.e. LSB goes first
    uint8_t expected[] = {0x65,                     // id 
                          0xE8, 0x03, 0x00, 0x00,   // tick
                          0xEF, 0xBE, 0xAD, 0xDE,   // measurement
                          0xAC};                    // CRC    

    SimulatedUART sim_uart;
    RobotPrinter printer(sim_uart);

    printer.add_CRC_and_print(reinterpret_cast<uint8_t*>(&sonar_packet), sizeof(sonar_packet));

    uint8_t actual[sizeof(expected)];
    for(unsigned i = 0; i < sizeof(expected); i++)
    {
        actual[i] = sim_uart.read();
    }    
    
    TEST_ASSERT_EQUAL_MEMORY(expected, actual, sizeof(expected));
}

int main( int argc, char **argv) {
    UNITY_BEGIN();
   
    RUN_TEST(test_status_packet);
    RUN_TEST(test_sonar_packet);
    
    UNITY_END();

    stop_simavr();

    return 0; 
}
