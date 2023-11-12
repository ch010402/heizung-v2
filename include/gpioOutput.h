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

// gpioOutput(string ioName, int gpioPin)
class gpioOutput {
public:
  gpioOutput(std::string ioName, int gpioPin);
  gpioOutput() = default;
  ~gpioOutput();

  void on();
  void off();
  void toggle();

  std::string getName() const;
  int getPin() const;
  bool getInitialized() const;
private:
  int gpioPin_;
  bool initialized_;
  std::string ioName_;
  bool status_;
  static std::shared_ptr<gpioChipCommunication> gpioChipCommunicationInstance;
  struct gpiod_line* line;

  void initialize();
};

#endif // !GPIO_OUTPUT_H
