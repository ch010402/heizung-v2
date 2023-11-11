/*
* Header file for the mischer class that open or closes a mixer of hot and cold water based on two outputs. Only one output can be active at a time 
* Christoph Latzer 2023-10-21
* heating controls IO's on a raspberryPi that control my house heating
* this is the second version based on libgpiod as wiringPi is depreciated and no longer in the distro
* the current version is (libgpiod) v1.6.3 supplied with debian bookworm arm64
*
* GNU GPL v3.0
*/

#ifndef MISCHER_H

#define MISCHER_H

#include "gpioOutput.h"
#include <unistd.h>

//mischer(string mischerName, gpioOutput opener, gpioOutput closer, int cycleduration, int steps)
//set the cycleduration in seconds from fully close to fully open, choose a sane value for steps (steps <3 will be defaulted to 5)
class mischer {
public:
  mischer(std::string mischerName, gpioOutput opener, gpioOutput closer, int cycleDuration, int steps);
  ~mischer();
  
  int getCurrenStep();
  std::string getName();
  void setStep(int currentStep_);
private:
  std::string mischerName_;
  gpioOutput opener_;
  gpioOutput closer_;
  int cycleDuration_;
  int steps_;
  double oneStep_;
  int currentStep_;
  void initialize();
  bool inititalized_;
  void open();
  void close();
};

#endif //!MISCHER_H