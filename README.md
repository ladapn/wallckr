# wallckr
wallckr stands for **wall** following mobile robot with A**ckr**man steering. In other words, wallckr is a small indoor autonomous vehicle with "car like" steering that goes around your walls.

<p align="center">
  <img src="./img/wallckr_posing.jpeg" alt="wallckr, the robot" width="400"/>
</p>


## Why?
The main purpose of this project is to have fun building such a robot and developing its code. If anyone got inspired by this to build a similar project, it would be a very nice side effect. 

## Getting Started
### Hardware Prerequisities
Well, this is the tricky part, because you need to have your own wallckr robot and as far as I know there happens to be only one - the one in my possession :) 

Here is at least a list of the main HW components: 
- Arduino Mega2560 (or compatible)
- Motor Shield L298P for Arduino 
- Hitec HS-422 servo
- 3x HY-SRF05 ultrasound distance sensors
- Custom PCB to distribute power 
- Custom PCB to extend IO capabilities 

### Software Prerequisities
- PlatformIO
  - Is used as a build management tool 
  - [Install platformio](https://platformio.org/install) either as a standalone tool or as an extension for your favorite IDE
    - In case you want to use it as a standalone tool consider adding it to your $PATH: `export PATH=$PATH:$HOME/.platformio/penv/bin`
- Arduino libraries
  - NOTE: These libraries are defined in `platformio.ini` and platformio downloads them automagically during first build, there is no need to download them manually 
  - [NewPing](https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home)
  - [Servo](https://github.com/arduino-libraries/Servo)
- [Optional] [simavr](https://github.com/buserror/simavr)
  - Is used to executed unit tests on simulated target
  - Install it following the steps in the link above and make sure it is in your $PATH

### Building and Uploading the Code
You can either execute the below listed commands or click the corresponding buttons in your IDE
#### Building
- `pio run`
#### Uploading
- `pio run --target upload`
#### Executing Unit Tests
On simulated target:
- `pio test --without-uploading`
- simavr has to be present, see [prerequisities](#software-prerequisities)

On real HW:
- `pio test`


## Controlling your Robot
[ovladacka](https://github.com/ladapn/ovladacka) - the official tool to communicate with wallckr

NOTE: currently only auto steering (i.e. ability to automatically control angle of front wheels) and enabled by default. Robot speed is zero by default and has to be controlled by ovladacka, auto speed is not implemented. 

## Contributing
In case you would like to fix or improve anything, feel free to create a pull request. 

## License
This project is licensed under the MIT license.
