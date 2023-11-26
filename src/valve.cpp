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
/*
void valve::on() {
  if (!initialized_) { initialize(); }
  gpiod_line_set_value(line, 1);
  setTime_ = checkTime::getTimeInt();
  std::cout << setTime_ << std::endl;
  status_ = true;
}

void valve::off() {
    if (!initialized_) { initialize(); }
    gpiod_line_set_value(line, 0);
    setTime_ = checkTime::getTimeInt();
    std::cout << setTime_ << std::endl;
    status_ = false;
  }
*/

valve::valve(const GpioOutData& data) : 
  gpioOutput(data) {
}