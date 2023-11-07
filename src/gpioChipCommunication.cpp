/*
* Source file for the gpioChipCommunication class that bouild up a communication channel to a gpio interface
* Christoph Latzer 2023-10-21
* heating controls IO's on a raspberryPi that control my house heating
* this is the second version based on libgpiod as wiringPi is depreciated and no longer in the distro
* the current version is (libgpiod) v1.6.3 supplied with debian bookworm arm64
*
* GNU GPL v3.0
*
* to pecompile use:
* g++ -c ../src/gpioChipCommunication.cpp -o gpioChipCommunication.o -lgpiod -I../include
*/

#include "gpioChipCommunication.h"

// Constructor 
// gpioChipCommunication() opens the communication to the gpio chip "gpiochip0"
gpioChipCommunication::gpioChipCommunication() : chipName_("gpiochip0"), chip(nullptr) {
  // Open the connection to the chip
  chip = gpiod_chip_open_by_name(chipName_);
}

// Destructor 
// gpioChipCommunication() closes the communication to the gpio chip "gpiochip0"
gpioChipCommunication::~gpioChipCommunication() {
  // Close the connection to the chip if no line is open 
  if (chip) {
	gpiod_chip_close(chip);
  }
  std::cout << "Connection to chip closed, good bye." << std::endl;
}