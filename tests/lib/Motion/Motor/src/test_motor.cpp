#include <zephyr/ztest.h>
#include "Motor.h"

ZTEST_SUITE(motor_tests, NULL, NULL, NULL, NULL, NULL);

ZTEST(exp_fileter_tests, test_step_N_4_int)
{
    Motor motor;
    motor.initialize();

    int spd = -100;
    while(true) {
        for(spd = -100; spd <= 100; spd+=5) {
        motor.set_speed(spd);
        k_msleep(1000);
    }

    for(spd = 100; spd >= -100; spd-=5) {
        motor.set_speed(spd);
        k_msleep(1000);
    }
  }
}



