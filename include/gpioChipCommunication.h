/*
* Header file for the gpioChipCommunication class that bouild up a communication channel to a gpio interface
* Christoph Latzer 2023-10-21 
* heating controlls IO's on a raspberryPi that controll my house heating 
* this is the second version based on libgpiod as wiringPi is depriciated and no longer in the distro
* the current version is (libgpiod) v1.6.3 supplied with debian bookworm arm64
* 
* GNU GPL v3.0
*/

#ifndef GPIO_CHIP_COMMUNICTAION_H

#define GPIO_CHIP_COMMUNICATION_H

#include <gpiod.h>  // library to access GPIOs on a device 
#include <iostream>
#include "gpioOutput.h"

class gpioChipCommunication {
public:
  gpioChipCommunication();
  ~gpioChipCommunication();
private:
  const char* chipName_;
  struct gpiod_chip* chip;
};

#endif // !GPIO_CHIP_COMMUNICTAION_H
