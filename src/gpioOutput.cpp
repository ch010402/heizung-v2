/*
* Header file for the gpioOutput class that build up a communication line to a gpio interface
* Christoph Latzer 2023-10-21
* heating controlls IO's on a raspberryPi that controll my house heating
* this is the second version based on libgpiod as wiringPi is depriciated and no longer in the distro
* the current version is (libgpiod) v1.6.3 supplied with debian bookworm arm64
*
* GNU GPL v3.0
*/

#include "gpioOutput.h"

// Define the static member
std::shared_ptr<gpioChipCommunication> gpioOutput::gpioChipCommunicationInstance;

int gpioOutput::instanceCount = 0;

// Constructor 
gpioOutput::gpioOutput(std::string ioName, int gpioPin) :
  gpioPin_(gpioPin), initialized_(false), ioName_(ioName), status_(false), line(nullptr) {
  instanceCount++;
}
// Destructor
gpioOutput::~gpioOutput() {
  instanceCount--;
  // Ensure the IO is turned off before releasing the line
  off();
  if (line) {
	gpiod_line_release(line);
  }
  initialized_ = false;
}
// functions
void gpioOutput::on() {
  if (!initialized_) { initialize(); }
  gpiod_line_set_value(line, 1);
  status_ = true;
}
void gpioOutput::off() {
  if (!initialized_) { initialize(); }
  gpiod_line_set_value(line, 0);
  status_ = false;
}
void gpioOutput::toggle() {
  if (!initialized_) { initialize(); }
  gpiod_line_set_value(line, status_ ? 0:1);
  status_ = !status_;
}
int gpioOutput::getInstanceCount() {
  return instanceCount;
}
void gpioOutput::initialize() {
  if (!gpioChipCommunicationInstance) {
	gpioChipCommunicationInstance = std::make_shared<gpioChipCommunication>();
  }
  line = gpiod_chip_get_line(gpioChipCommunicationInstance->chip, gpioPin_);
  gpiod_line_request_output(line, "output", 0);
  initialized_ = true;
}