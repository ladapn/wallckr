# Porting wallckr to new hardware

This document describes the hardware abstraction boundary in this codebase,
for anyone porting wallckr's firmware to different hardware (for example a
V2 robot on different MCU/RTOS, such as Zephyr). It reflects the current
Arduino/PlatformIO V1 architecture, not a promise about how a ported V2
codebase will be organized.

## The short version

Every `lib/` folder is either:

- **hardware-independent** - plain C++, no Arduino/AVR types, unit tested on
  the host (see [Testing](#testing) below), or
- **hardware-specific** - implements one of a small set of interfaces using
  Arduino APIs, named `<Something>Arduino`.

A port needs to replace every hardware-specific folder with an equivalent
implementing the same interface for the new hardware. Everything else -
control logic, protocol decoding, the `Robot` state machine - is meant to be
reused unchanged.

## Hardware-independent (reuse as-is)

| Folder | What it is |
| --- | --- |
| `lib/Control` | `Regulator`/`Regulator_P`/`Regulator_PD`, `AutoSteering` (FOLLOWING/AVOIDING state machine), `ExpFilter`, `TimeManager`, `RobotCommand` |
| `lib/Battery` | `Battery` health/cutoff-with-hysteresis logic, `IBatterySensor` interface |
| `lib/Sensing` | `Sensing` (aggregates distance measurements, sends telemetry), `IDistanceSensor` interface |
| `lib/Communication` | `ExternalCommandDecoder`, `OvladackaParser`/`BLEJoystickDecoder` protocol decoders, `IRobotIndicators` interface |
| `lib/Telemetry` | `RobotPackets` (wire format), `RobotPrinter` |
| `lib/IOStream` | `IRobotIOStream` interface |
| `lib/Robot` | `Robot` - the top-level state machine (AUTOMATIC/MANUAL, battery-triggered safety disable) |
| `lib/Motion` | `Motion`, plus the `IMotorController`/`ISteeringServo` interfaces (the concrete Arduino implementations live in `lib/MotionArduino`) |

## Hardware-specific (reimplement per-target)

| Folder | Implements | Needs |
| --- | --- | --- |
| `lib/MotionArduino` | `IMotorController`, `ISteeringServo` | `<Arduino.h>`, `<Servo.h>` |
| `lib/BatteryArduino` | `IBatterySensor` | `<Arduino.h>` (ADC read) |
| `lib/SensingArduino` | `IDistanceSensor` | `<NewPing.h>` |
| `lib/CommunicationArduino` | `IRobotIndicators`, plus `IRobotIOStream` for the BLE UART | `<Arduino.h>` |
| `lib/BoardConfig` | Pin assignments (`ArduinoBoardConfig.h`) | `<Arduino.h>` |

A port implements these five interfaces - `IMotorController`, `ISteeringServo`,
`IBatterySensor`, `IDistanceSensor`, `IRobotIndicators`, and `IRobotIOStream`
(used both for the BLE command stream and for outgoing telemetry) - for the
new hardware, and writes a new entry point (like `src/main.cpp`) that wires
them into `Robot` the same way `src/main.cpp` does today.

## Testing

The `native` PlatformIO environment (`platformio.ini`) builds and runs every
test suite directly on the host compiler, with no AVR toolchain, no simavr,
and nothing from the `*Arduino`/`BoardConfig` folders involved. If a test
fails to build there, something it depends on isn't actually
hardware-independent yet - that env exists specifically to catch this class
of coupling before it becomes a porting surprise (see PR #41 and PR #42 for
two examples this already caught: implementation files that were sharing a
library folder with hardware-independent headers, and a stray transitive
include).

wallckr's tests use the Unity framework via PlatformIO's test runner. A
Zephyr-based V2 build (running under plain `west`, not PlatformIO) would most
naturally use Zephyr's own Ztest + Twister instead - the two frameworks'
assert macros translate almost mechanically, but their test-registration
models differ enough (manual `RUN_TEST()` calls vs. auto-discovered
`ZTEST()`) that porting a test file is expected to mean rewriting it against
Ztest, not sharing one file between both frameworks.

## Known wrinkle: calibration constants live in "portable" files

`lib/Motion/MotionConstants.h` (`SERVO_MIN_RIGHT`, `SERVO_MAX_LEFT`,
`SERVO_CENTER`, `MAX_SPEED`, ...) is hardware-independent code, but its
values are calibrated for V1's specific servo and motor. A straight copy for
a V2 port will build and pass tests, but the numbers themselves will need
re-tuning for the new hardware. Same for the battery cell count/cutoff
constants in `src/main.cpp`. Neither is broken - just worth knowing they're
V1-specific values sitting in otherwise-reusable files.
