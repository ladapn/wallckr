#include "Battery.h"
#include "RobotPackets.h"
#include "RobotPrinter.h"

bool Battery::is_ok(uint32_t current_millis) {
  // TODO: add filtering
  auto battery_voltage_mVDC = battery_sensor.get_battery_voltage_mVDC();

  StatusPacket status_data(current_millis, battery_voltage_mVDC, 0, 0);

  battery_printer.print(status_data);

  if (battery_dead) {
    if (battery_voltage_mVDC >
        (cutoff_mVDC + hysteresis_mVDC)) // Add some hysteresis
    {
      battery_dead = false;
      return true;
    }

    return false;
  }

  if (battery_voltage_mVDC <= cutoff_mVDC) {
    battery_dead = true;
    return false;
  }

  return true;
}
