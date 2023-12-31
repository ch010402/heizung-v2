/*
* Header file for the valve class that open or closes a valve inhertits the gpioOuput class and extends it by a longer switching cycle 
* Christoph Latzer 2023-11-16
* heating controls IO's on a raspberryPi that control my house heating
* this is the second version based on libgpiod as wiringPi is depreciated and no longer in the distro
* the current version is (libgpiod) v1.6.3 supplied with debian bookworm arm64
*
* GNU GPL v3.0
*/

#include "valve.h"

void valve::on() {
  if (!initialized_) { 
    initialize(); 
    }
  newTime_ = checkTime::getTimeInt();
  if (status_ == false) {
    if (newTime_ - waitToSwitch >= setTime_) {
      gpiod_line_set_value(line, 1);
      status_ = true;
      }
    else {
      throw std::runtime_error("Failed to switch " + ioName_ + ", time between on and off too short: <" + std::to_string(waitToSwitch) + "s");
      }
  }
  setTime_ = newTime_;
}

void valve::off() {
  if (!initialized_) { 
    initialize(); 
    }
  newTime_ = checkTime::getTimeInt();
  if (status_ == true) {
    if (newTime_ - waitToSwitch >= setTime_) {
      gpiod_line_set_value(line, 0);
      status_ = false;
      }
    else {
      throw std::runtime_error("Failed to switch " + ioName_ + ", time between off and on too short: <" + std::to_string(waitToSwitch) + "s");
      }
  } 
  setTime_ = newTime_;
}


valve::valve(const ValveData& data) : 
  gpioOutput(data.gpioOut), waitToSwitch(data.timeToSwitch) {
    setTime_ = -1000;
}