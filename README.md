# wallckr
wallckr stands for **wall** following mobile robot with A**ckr**man steering. In other words, wallckr is a small indoor autonomous vehicle with "car like" steering that goes around your walls.

## Why?
The main purpose of this project is to have fun building such a robot and developing its code. If anyone got inspired by this to build a similar project, it would be a very nice side effect. 

## Getting Started
### Hardware Prerequisities
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

## Contributing

## License
