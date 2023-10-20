/*
* GNU GPL v3.0
* heating controlls IO's on a raspberryPi that controll my house heating 
* this is the second version based on libgpiod as wiringPi is depriciated and no longer in the distro
* the current version is (libgpiod) v1.6.3 supplied with debian bookworm arm64
* 
* Output of: TEST board	   |  PRODUCTION board
*            red1     23   |  Elektropumpe         26
*            red2      5   |  Elektrorelais an/aus  4
*            red3     24   |  Elektromischer auf   25
*            yellow1  25   |  Elektromischer zu    24
*            yellow2  12   |  Boilerpumpe          20
*            yellow3  16   |  Boilerventil auf/zu   5
*            green1   19   |  Heizmischer auf      13
*            green2   26   |  Heizmischer zu        6
*            green3   20   |  Speicherventl auf/zu  3
*            blue     22   |  Umlaufpumpe          21
*                          |  Ofenpumpe Stufe1     19
*                          |  Ofenpumpe Stufe2     16
*                          |  Ofenpumpe Stufe3     12
*                          |  reserve 1	           18
*                          |  reserve 2	           27
* 
* (c) Christoph Latzer 2023-Oct-17
*/					   

#include <iostream> // used for interacting with the console (cout)
#include <gpiod.h>  // library to access GPIOs on a device 
#include <unistd.h> // used for usleep //myabe replace by chrono
#include <string>   // used for strings
#include <chrono>   // used to access the system time 
#include <ctime>    // to manipulate the t_time 


/*enumerations*/

// ioType enumerate possible values {pump, valve, breaker, led}
enum ioType {
  pump,
  valve,
  switcher,
  led
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
  io(std::string ioName, int gpioPin, ioType type) : gpioPin_(gpioPin), ioName_(ioName), type_(type) {
	instanceCount++;
  }

  //// default destructor 
  ~io() {
	 // decrease number of instances
	instanceCount--;
	// switch the io off in any case it would be on.
	off();
	// close the line to the IO
	gpiod_line_release(line);
	// closes the connectin to the chip if no line is open
	if (instanceCount == 0) {
	  gpiod_chip_close(chip);
	  std::cout << "Connection to chip closed, good bye." << std::endl;
	}
	initilized_ = false;
  }

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
  /*
  void destroy() {
	// switch the io off in any case it would be on.
	off();
	// close the line to the IO
	gpiod_line_release(line);
	openLineCounter--;
	// closes the connectin to the chip if no line is open
	if (openLineCounter == 0)
	  gpiod_chip_close(chip);
	initilized_ = false;
  }
  */
  static int getInstanceCount() {
	return instanceCount;
  }

private:
  // use for counting how many instances of this object where created to close connection to the chip
  static int instanceCount;
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
	//openLineCounter++;
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
  /*
  void destroy() {
	ioOpen.destroy();
	ioClose.destroy();
  }
  */
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

/*functions*/

// FUNCTION checkLowTarif()
// returns boolean ture or false
bool checkLowTarif() {
  // Get the current system time using the system clock
  auto currentTime = std::chrono::system_clock::now();
  // Convert the system time to a time_point object
  std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
  // Convert the time_point to a string representation
  std::string currentTimeStr = std::ctime(&currentTime_t);
  // Convert the time_point to a struct tm
  std::tm* currentTime_tm = std::localtime(&currentTime_t);

  // Extract individual components
  //  int year = currentTime_tm->tm_year + 1900;  // Years since 1900
  //  int month = currentTime_tm->tm_mon + 1;     // Month (0-based index)
  //  int day = currentTime_tm->tm_mday;          // Day of the month
  int hour = currentTime_tm->tm_hour;         // Hour
  //  int minute = currentTime_tm->tm_min;        // Minute
  //  int second = currentTime_tm->tm_sec;        // Second
  int weekday = currentTime_tm->tm_wday;      // days since sunday

  //debug std::cout << currentTimeStr << std::endl;

  if (weekday == 0 || weekday == 6) {
	return true;
  }
  else if (hour < 7 || hour >= 20) {
	return true;
  }
  return false;
}

/*global  variables*/

// counter for how many lines to the gpio chip are open 
//int openLineCounter = 0;

int io::instanceCount = 0;

/*main*/

int main(int argc, char** argv) {
  //// setup
  io blue("blue led", 22, led);
  mixer red("red", 23, 5, 5, 16);
  io red3("HighTarif", 24, led), green1("LowTarif", 19, led);
  
  //// start

  //test HT or LT
  if (checkLowTarif()) {
	green1.on();
  }
  else {
	red3.on();
  }

  //// loop

  //test io 
  for (int i = 0; i < 10; i++) {
	blue.on();
	usleep(0.1 * 1000000);
	blue.off();
	usleep(0.3 * 1000000);
  }

  //test mixer
  for (int j = 0; j < 10; j++) {
	red.open();
	red.close();
  }

  //// close 
  /*
  blue.destroy();
  red.destroy();
  red3.destroy();
  green1.destroy();
  */
  return 0;
}
