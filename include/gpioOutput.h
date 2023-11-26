/*
* Header file for the gpioOutput class that build up a communication line to a gpio interface
* Christoph Latzer 2023-10-21
* heating controls IO's on a raspberryPi that control my house heating
* this is the second version based on libgpiod as wiringPi is depreciated and no longer in the distro
* the current version is (libgpiod) v1.6.3 supplied with debian bookworm arm64
*
* GNU GPL v3.0
*/

#ifndef GPIO_OUTPUT_H

#define GPIO_OUTPUT_H

#include <string>
#include <gpiod.h>
#include <memory>
#include "gpioChipCommunication.h"
#include "checkTime.h"

// struct GpioOutData(string ioName, int gpioPin)
struct GpioOutData {
  std::string ioName;
  int gpioPin;
};

// gpioOutput(string ioName, int gpioPin)
class gpioOutput {
public:
  gpioOutput(const GpioOutData& data);
  gpioOutput() = default;
  ~gpioOutput();

  void on();
  void off();
  void toggle();

  std::string getName() const;
  int getPin() const;
  bool getInitialized() const;
private:
protected:
  int gpioPin_;
  std::string ioName_;
  GpioOutData data;
  static std::shared_ptr<gpioChipCommunication> gpioChipCommunicationInstance;
  int setTime_;
  bool status_;
  bool initialized_;
  void initialize();
  struct gpiod_line* line;
  //checkTime* setTime;
};

#endif // !GPIO_OUTPUT_H
