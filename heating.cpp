/*
* GNU GPL v3.0
* heating controlls IO's on a raspberryPi that controll my house heating 
* this is the second version based on libgpiod as wiringPi is depriciated and no longer in the distro
* the current version is (libgpiod) v1.6.3 supplied with debian bookworm arm64
*/

#include <iostream> // used for interacting with the console (cout)
#include <gpiod.h> // library to access GPIOs on a device 
#include <unistd.h> // used for usleep //myabe replace by chrono
#include <string> // used for strings

/*global  variables*/

// counter for how many lines to the gpio chip are open 
int openLineCounter = 0;

/*enumerations*/

// ioType enumerate possible values {pump, valve, breaker, led}
enum ioType {
  pump,
  valve,
  switcher,
  mixer
};

/*classes*/

// CLASS io (string ioName, int gpioPin, enum ioType)
// has functions on(), off(), toggle(), destroy()
// has accessable variable state_ (true/false) ioType 
class io {
private:
  // int ioPin_ the pin to controll the I/O
  int gpioPin_;
  // bool initilaized_ tells if it is initialized or not
  bool initilized_ = false;
public:
  // string ioName_ keeps the name of the IO
  std::string ioName_;
  // bool status_ default to false meaning off not set, reports the status as boolean 
  bool status_ = false;
  // enum ioType enumerate possible values {pump, valve, switch, mixer}
  ioType type_;

  //// default constructor 
  io(std::string ioName, int gpioPin, ioType type) : ioName_(ioName), gpioPin_(gpioPin), type_(type) {}

  //// functions

  // io.on() function switches the output on
  void on() {
	if (!initilized_)
	  initilaize();
	gpiod_line_set_value(line, 1);
	status_ = true;
  }
  // io.off() function switches the output off
  void off() {
	if (!initilized_)
	  initilaize();
	gpiod_line_set_value(line, 0);
	status_ = false;
  }
  // io.toggle() function toggles the output on or off 
  void toggle() {
	if (!initilized_)
	  initilaize();
	if (status_) {
	  gpiod_line_set_value(line, 1);
	  status_ = true;
	}
	else {
	  gpiod_line_set_value(line, 0);
	  status_ = false;
	}
  }
  // io.destroy() function closes the line to the io and if it is the last line closes the connection to the chip
  void destroy() {
	// close the line to the IO
	gpiod_line_release(line);
	openLineCounter--;
	// closes the connectin to the chip if no line is open
	if (openLineCounter == 0)
	  gpiod_chip_close(chip);
	initilized_ = false;
  }
private:
  // set the chip name as const as we use a raspberryPi we have a "gpiochip0" 
  // this is currently static, if we would like to use a different board this value neeeds to be adjusted 
  const char* chipName_ = "gpiochip0";
  // build gpiod chip
  struct gpiod_chip* chip;
  // build gpiod lines
  struct gpiod_line* line;
    // io.initialize() builds up the connection to the chip and creates a line to the io
  void initilaize() {
	// open the connection to the chip
	chip = gpiod_chip_open_by_name(chipName_);
	// open a GPIO line
	line = gpiod_chip_get_line(chip, gpioPin_);
	// request a line as an output and default it to 0/false 
	gpiod_line_request_output(line, "output", 0);
	openLineCounter++;
	initilized_ = true;
  }
};

// CLASS  mixer (string mxName, int gpioPin1, int gpioPin2, int openCloseDuration, int steps)
// has functions open(), close(), destroy()
// has accessable variables state_ (true/false) currentStep_
class mixer {
private:
  // int openCloseDuration_ is the duration the mixer requires to move from fully closed to fully opend
  int openCloseDuration_;
  // int steps_ the total number of steps
  int steps_;
  // int currentStep_ stores the current step the mixer is at
  int currentStep_ = 0;
  // io ioOpen ioClose as a mixer needs two output one to open and one to close 
  io ioOpen, ioClose;
  // bool initilaized_ true if the postion of the mixer is known, default to false 
  bool initilized_ = false;
public:
  // string mxName_ keeps the name of the mixer
  std::string mxName_;

  //// default constructor 
  mixer(std::string mxName, int gpioPin1, int gpioPin2, int openCloseDuration, int steps) :
	openCloseDuration_(openCloseDuration),
	steps_(steps),
	ioOpen(mxName + ".open", gpioPin1, switcher),
	ioClose(mxName + ".close", gpioPin2, switcher),
	mxName_(mxName) {
	initialize();
  }

  //// functions

  // mixer.open() opens the mixer by one step
  void open() {
	if (!initilized_)
	  initialize();
	// ensures that ioClose is off, switches ioOpen on and waits one step and then turns ioOpen off
	if (currentStep_ < steps_) {
	  ioClose.off();
	  ioOpen.on();
	  waitOneStep();
	  ioOpen.off();
	  currentStep_++;
	}
  }
  // mixer.close() closes the mixer by one step
  void close() {
	if (!initilized_)
	  initialize();
	// ensures that ioOpen is off, switches ioClose on and waits one step and then turns ioClose off
	if (currentStep_ > 1) {
	  ioOpen.off();
	  ioClose.on();
	  waitOneStep();
	  ioClose.off();
	  currentStep_--;
	}
  }
  //mixer.destroy() function closes the line to the io's and if it is the last line closes the connection to the chip
  void destroy() {
	ioOpen.destroy();
	ioClose.destroy();
  }
private:
  // mixer.waitOneStep waits 1/step of the full open close cycle in seconds
  void waitOneStep() {
	usleep(openCloseDuration_ * 1000000 / steps_);
  }
  // mixer.initialize() turn the mixer all to the closed position by waiting a full turn set the currentStep to 1 
  void initialize() {
	ioOpen.off();
	// turn the mixer all to the closed position by waiting a full turn 
	for (int i = 1; i <= steps_; i++) {
	  ioClose.on();
	  waitOneStep();
	  ioClose.off();
	}
	currentStep_ = 1;
	initilized_ = true;
  }
};

/*main*/

int main(int argc, char** argv) {

  return 0;
}