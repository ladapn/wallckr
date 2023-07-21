<div align="center">
 
  ![Build and tests](https://github.com/ladapn/wallckr/actions/workflows/build.yml/badge.svg)
  [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
</div>

# wallckr
wallckr is a small indoor autonomous vehicle with "car like" steering that goes around your walls and avoids obstacles. wallckr stands for **wall** following mobile robot with A**ck**e**r**mann steering. 

<div align="center">
  <img src="./img/wallckr_posing.jpeg" alt="wallckr, the robot" width="400"/>
</div>


## Motivation
The main purpose of this project is to have fun building such a robot and to enjoy developing its code. Also, I wanted to verify what is the minimum number of sensors that allows to perform wall following operation. Last but not least I wanted to have a platform for testing of control algorithms for Ackermann steering. Why Ackermann - i.e. car like - steering? Because it is more difficult to control than the usual differential steering and thus more fun.   

If anyone gets inspired by this to build a similar project, it would be a very nice side effect. 

## Features
- Wall following 
    - Controls distance from right-hand side wall maintain target distance of 25 cm
    - This is achieved by a controller commanding a servo to change steering angle of the front wheels. The servo command is based on measurements from right front and right distance sensors 
    - Currently only Proportional (P) and Proportional Derivative (PD) controllers are implemented and P is used by default
- Obstacle avoidance
    - If front sensor measurement is lower than 40 cm, or if right front sensor measurement is lower than 15 cm, obstacle avoidance mode is entered. In this mode, side distance is no longer maintained. Instead, the robot simply turns front wheels left to the maximal allowed angle. When the front measurement is less than 60 cm and the front right measurement is less than 25 cm, wall following mode is entered again. 
- Motor control
    - Currently open loop control is used - i.e. if the robot goes uphill or its batteries are weaker, the speed will decrease, because there's no speed feedback
- Communication via Bluetooth Low Energy (BLE) 
    - [ovladacka](https://github.com/ladapn/ovladacka) is the official communication tool
    - Accepts commands from remote device
    - Sends back distance measurements and robot status information
- Simplistic UI
    - 5 red LEDs that indicate current status on connection shield
      - LED1 on and LED2 off - wall following mode
      - LED1 off and LED2 of - obstacle avoiding mode
      - LED3 and 4 always off, reserved for future use
      - LED5 blinking - battery voltage too low, robot stops and is not allowed to move until recharged
    - 1 green LED on power board - on when power board is alive 

# Getting Started
## Hardware Prerequisities
Well, this is the tricky part, because you need to have your own wallckr robot and as far as I know there happens to be only one - the one in my possession :) Anyway, it should take only minor changes to make the software running on a similar car-like robot you can build or buy. 

Main HW components: 
- Arduino Mega2560
- Motor Shield L298P for Arduino 
- GM37 300 RPM DC motor
- Hitec HS-422 servo
- 3x HY-SRF05 ultrasound distance sensors
    - Front facing, front right facing and right facing one
    - 45 deg angular spacing between them
- Custom PCB to distribute power 
- Custom PCB to extend IO capabilities
- HM-10 BLE module 
- Chassis made of Merkur construction set
- 8x 1.2V NiMH batteries

## Software Prerequisities
- PlatformIO
  - Used as a build management tool 
  - [Install platformio](https://platformio.org/install) either as a standalone tool or as an extension for your favorite IDE
    - In case you want to use it as a standalone tool consider adding it to your PATH. E.g. in case of Linux OS: `export PATH=$PATH:$HOME/.platformio/penv/bin`
- Arduino libraries
  - NOTE: These libraries are defined in `platformio.ini` and PlatformIO downloads them automagically during first build, there is no need to download them manually 
  - [NewPing](https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home)
  - [Servo](https://github.com/arduino-libraries/Servo)
- [Optional] [simavr](https://github.com/buserror/simavr)
  - Used to execute unit tests on simulated target - i.e. without the actual hardware
  - Install it following the steps in the link above and make sure it is in your PATH. 

## Building and Uploading the Code
You can either execute the below listed commands or click the corresponding buttons in your IDE
### Building
- `pio run`
### Uploading
- `pio run --target upload`
### Executing Unit Tests
On simulated target:
- `pio test --without-uploading`
- simavr has to be present, see [prerequisities](#software-prerequisities)

On real HW:
- `pio test`
### Run static code analysis
- `pio check --skip-packages`


## Controlling your Robot
When you power your robot on, it does not move. It waits for a `up arrow` command to start. This command can be sent via [ovladacka](https://github.com/ladapn/ovladacka) - the official tool to communicate with wallckr. Once this command is received, the robot starts going forward and enters wall following mode (see [Features](#features) above). If no wall is present, it will keep turning right until a wall is found. 

In both - wall following and obstacle avoidance - modes, the steering angle of the front wheels is controlled automatically, while the speed is always user controlled via `up` and `down` arrows.  

You can disable/enable this automatic steering via ovladacka (press `left shift` to toggle it) and control the robot fully manually. This converts the robot into a BLE remote control car. 

## Contributing
In case you would like to fix or improve anything, feel free to create a pull request. 

## License
This project is licensed under the MIT license. Why? Because I wanted a permissive license that would give anybody a chance to do with this code whatever they please. Of course, with no warranty :) 
