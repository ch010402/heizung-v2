/*
* Source file for the mischer class that controls a mischer of hot and cold water
* Christoph Latzer 2023-10-29
* heating controls IO's on a raspberryPi that control my house heating
* this is the second version based on libgpiod as wiringPi is depreciated and no longer in the distro
* the current version is (libgpiod) v1.6.3 supplied with debian bookworm arm64
*
* GNU GPL v3.0
* 
* to prcompile use:
* g++ -c ../src/mischer.cpp -o mischer.o -I../include
*/
#include "mischer.h"

//constructor

//mischer(string mischerName, gpioOutput opener, gpioOutput closer, int cycleduration, int steps)
//set the cycleduration in seconds from fully close to fully open, choose a sane value for steps (steps <3 will be defaulted to 5)
mischer::mischer(std::string mischerName, gpioOutput opener, gpioOutput closer, int cycleDuration, int steps)
: mischerName_(mischerName), opener_(opener), closer_(closer), cycleDuration_(cycleDuration), steps_(steps) {
  inititalized_ = false;
  currentStep_ = 0;
  oneStep_ = cycleDuration_ / steps_;
  if (steps_ < 3){
    steps_ = 5;
    };
}
// Destructor
mischer::~mischer() {
  //close the mischer all the way
  setStep(1);
  inititalized_ = false;
}

// int getCurrentStep() returns the current step as int
int mischer::getCurrenStep() {
  return currentStep_;
}

// string getName() returns the name as string
std::string mischer::getName() {
  return mischerName_;
}

// setStep(int) lets you set the step to be at
void mischer::setStep(int newStep) {
  if (!inititalized_) {
    initialize();
  }
  if (newStep == currentStep_) {
    return;
  };
  if (newStep < 1) {
    return;
  };
  if (newStep > steps_) {
    mischer::setStep(steps_);
    return;
  };
  if (newStep < currentStep_) {
    for (int i = 0; i <= currentStep_ - newStep; i++) {
      mischer::close();
    }
  };
  if (newStep > currentStep_) {
    for (int i = 0; i <= newStep - currentStep_; i++) {
      mischer::open();
    }
  };
}

// initializes the mischer to position 1 
void mischer::initialize() {
  opener_.off();
  closer_.on();
  usleep(cycleDuration_*1000000);
  closer_.off();
  inititalized_ = true;
  currentStep_ = 1;
}

// open() opens the mischer one step
void mischer::open() {
  if (!inititalized_) {
    initialize();
  }
  closer_.off();
  opener_.on();
  usleep(oneStep_*1000000);
  opener_.off();
  currentStep_++;
}

// close() closes the mischer one step
void mischer::close() {
  if (!inititalized_) {
    initialize();
  }
  opener_.off();
  closer_.on();
  usleep(oneStep_*1000000);
  closer_.off();
  currentStep_--;
}
