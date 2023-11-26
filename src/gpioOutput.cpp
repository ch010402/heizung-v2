/*
* Header file for the gpioOutput class that build up a communication line to a gpio interface
* Christoph Latzer 2023-10-21
* heating controls IO's on a raspberryPi that control my house heating
* this is the second version based on libgpiod as wiringPi is depreciated and no longer in the distro
* the current version is (libgpiod) v1.6.3 supplied with debian bookworm arm64
*
* GNU GPL v3.0
* 
* to prcompile use:
* g++ -c ../src/gpioOutput.cpp -o gpioOutput.o -lgpiod -I../include
*/

#include "gpioOutput.h"

// Define the static member
std::shared_ptr<gpioChipCommunication> gpioOutput::gpioChipCommunicationInstance;

// Constructor 
gpioOutput::gpioOutput(const GpioOutData& data) :
  gpioPin_(data.gpioPin), initialized_(false), ioName_(data.ioName), status_(false), line(nullptr) {
}
// Destructor
gpioOutput::~gpioOutput() {
  // Ensure the IO is turned off before releasing the line
  gpioOutput::off();
  if (line) {
    gpiod_line_release(line);
    }
  initialized_ = false;
}

// functions

//void on() sets an output on if it is not already on
void gpioOutput::on() {
  if (!initialized_) { 
    initialize(); 
    }
  if (status_ == false) {
    gpiod_line_set_value(line, 1);
    status_ = true;
    }
    setTime_ = checkTime::getTimeInt();
}
//void off() sets the output off if it is not already off
void gpioOutput::off() {
  if (!initialized_) { initialize(); }
  if (status_ == true) {
    gpiod_line_set_value(line, 0);
    status_ = false;
    }
  setTime_ = checkTime::getTimeInt();
}
//void toggle() stwitches the state of the output
void gpioOutput::toggle() {
  if (!initialized_) { 
    initialize(); 
    }
  gpiod_line_set_value(line, status_ ? 0:1);
  setTime_ = checkTime::getTimeInt();
  status_ = !status_;
}
//string getName() retuns the name as string value 
std::string gpioOutput::getName() const {
	return ioName_;
}
//int getPin() returns the pin number
int gpioOutput::getPin() const {
  return gpioPin_;
}
//bool getInitialized() returns TRUE or FALSE, status of initialization of the gpio
bool gpioOutput::getInitialized() const {
  return initialized_;
}
//bool getStatus() returs TRUE or FALSE, status of the gpioOutput
bool gpioOutput::getStatus() const {
  return status_;
}
// PRIVATE void initialize() initializes the gpio output
void gpioOutput::initialize() {
  std::cout << "Initialisiere Output: " << ioName_;
  if (!gpioChipCommunicationInstance) {
    gpioChipCommunicationInstance = std::make_shared<gpioChipCommunication>();
    }
  line = gpiod_chip_get_line(gpioChipCommunicationInstance->chip, gpioPin_);
  gpiod_line_request_output(line, "output", 0);
  initialized_ = true;
  std::cout << " <- erledigt.\n";
}