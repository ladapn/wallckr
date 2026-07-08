#include "../../common/stop_simavr.h"
#include "Battery.h"
#include "IRobotIOStream.h"
#include "RobotPackets.h"
#include "RobotPrinter.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <unity.h>

namespace {
// Stub battery sensor with a settable voltage reading, so tests can
// simulate the pack draining/recovering without real hardware.
class StubBatterySensor : public IBatterySensor {
public:
  uint16_t voltage_mVDC = 0;
  uint16_t get_battery_voltage_adc() override { return 0; }
  uint16_t get_battery_voltage_mVDC() override { return voltage_mVDC; }
};

// Minimal IRobotIOStream that just captures what was written, so tests
// can inspect the telemetry Battery::is_ok sends out.
class CapturingIOStream : public IRobotIOStream {
public:
  uint8_t buffer[32] = {0};
  size_t bytes_written = 0;

  int available() override { return 0; }
  int read() override { return -1; }
  size_t write(uint8_t c) override {
    if (bytes_written < sizeof(buffer)) {
      buffer[bytes_written] = c;
    }
    bytes_written++;
    return 1;
  }
  size_t write(uint8_t *data, size_t size) override {
    for (size_t i = 0; i < size; i++) {
      write(data[i]);
    }
    return size;
  }
};

const uint16_t CUTOFF_mVDC = 450;
const uint16_t HYSTERESIS_mVDC = 50;
} // namespace

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_battery_ok_above_cutoff(void) {
  StubBatterySensor sensor;
  sensor.voltage_mVDC = CUTOFF_mVDC + 1;
  CapturingIOStream io_stream;
  RobotPrinter printer(io_stream);
  Battery battery(sensor, printer, CUTOFF_mVDC, HYSTERESIS_mVDC);

  TEST_ASSERT_TRUE(battery.is_ok(0));
}

void test_battery_dead_at_or_below_cutoff(void) {
  StubBatterySensor sensor;
  sensor.voltage_mVDC = CUTOFF_mVDC; // exactly at cutoff, not below
  CapturingIOStream io_stream;
  RobotPrinter printer(io_stream);
  Battery battery(sensor, printer, CUTOFF_mVDC, HYSTERESIS_mVDC);

  TEST_ASSERT_FALSE(battery.is_ok(0));
}

void test_battery_stays_dead_until_hysteresis_cleared(void) {
  StubBatterySensor sensor;
  CapturingIOStream io_stream;
  RobotPrinter printer(io_stream);
  Battery battery(sensor, printer, CUTOFF_mVDC, HYSTERESIS_mVDC);

  sensor.voltage_mVDC = CUTOFF_mVDC - 1;
  battery.is_ok(0); // drains below cutoff, enters dead state

  // Recovered above cutoff, but sitting exactly at (not past) the
  // hysteresis band - must not be considered ok yet.
  sensor.voltage_mVDC = CUTOFF_mVDC + HYSTERESIS_mVDC;
  TEST_ASSERT_FALSE(battery.is_ok(1000));
}

void test_battery_recovers_once_hysteresis_cleared(void) {
  StubBatterySensor sensor;
  CapturingIOStream io_stream;
  RobotPrinter printer(io_stream);
  Battery battery(sensor, printer, CUTOFF_mVDC, HYSTERESIS_mVDC);

  sensor.voltage_mVDC = CUTOFF_mVDC - 1;
  battery.is_ok(0); // drains below cutoff, enters dead state

  sensor.voltage_mVDC = CUTOFF_mVDC + HYSTERESIS_mVDC + 1;
  TEST_ASSERT_TRUE(battery.is_ok(1000));
}

void test_battery_sends_status_packet_with_measured_voltage(void) {
  StubBatterySensor sensor;
  sensor.voltage_mVDC = 1234;
  CapturingIOStream io_stream;
  RobotPrinter printer(io_stream);
  Battery battery(sensor, printer, CUTOFF_mVDC, HYSTERESIS_mVDC);

  battery.is_ok(2000);

  TEST_ASSERT_EQUAL(StatusPacket::SERIALIZED_SIZE, io_stream.bytes_written);

  // battery_voltage_mvdc is a little-endian uint16_t right after id (1
  // byte), tick (4 bytes) and version_ID (4 bytes) - see RobotPackets.h
  uint16_t reported_voltage_mVDC =
      static_cast<uint16_t>(io_stream.buffer[9]) |
      (static_cast<uint16_t>(io_stream.buffer[10]) << 8);
  TEST_ASSERT_EQUAL(1234, reported_voltage_mVDC);
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_battery_ok_above_cutoff);
  RUN_TEST(test_battery_dead_at_or_below_cutoff);
  RUN_TEST(test_battery_stays_dead_until_hysteresis_cleared);
  RUN_TEST(test_battery_recovers_once_hysteresis_cleared);
  RUN_TEST(test_battery_sends_status_packet_with_measured_voltage);

  UNITY_END();

  // Needed to stop simavr simulator after the test cases
  cli();
  sleep_cpu();

  return 0;
}
