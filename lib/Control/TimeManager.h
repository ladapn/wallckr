#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

/**
 * Class to check if it is time to perform certain action
 */
class TimeManager {
  unsigned long last_status_millis = 0;
  unsigned long last_command_millis = 0;

  bool first_pass_status = true;
  bool first_pass_command = true;

  static const int STATUS_PRINT_INTERVAL_MS = 1000;
  static const int COMMAND_INTERVAL_MS = 100;

public:
  /**
   * Check if enough time elapsed to perform status check
   * @param[in] current_millis current system time in ms
   * @return true if time elapsed from last status check is greater than
   * threshold, false otherwise
   */
  bool is_time_for_status_check(unsigned long current_millis) {
    if ((current_millis - last_status_millis > STATUS_PRINT_INTERVAL_MS) ||
        first_pass_status) {
      last_status_millis = current_millis;
      first_pass_status = false;
      return true;
    }

    return false;
  }

  /**
   * Check if enough time elapsed to perform automatic operation
   * @param[in] current_millis current system time in ms
   * @return true if time elapsed from last automatic operation is greater than
   * threshold, false otherwise
   */
  bool is_time_for_automatic_command(unsigned long current_millis) {
    if ((current_millis - last_command_millis > COMMAND_INTERVAL_MS) ||
        first_pass_command) {
      last_command_millis = current_millis;
      first_pass_command = false;
      return true;
    }

    return false;
  }
};

#endif // TIME_MANAGER_H
