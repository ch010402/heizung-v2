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
#include <unistd.h> // used for usleep //myabe replace by chrono
#include <string>   // used for strings
#include <chrono>   // used to access the system time 
#include <ctime>    // to manipulate the t_time 
#include <memory>   // 
#include <csignal>  //
#include <vector>	//
#include "gpioOutput.h"


/*misc*/

/*enumerations*/

/*classes*/

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
  gpioOutput ioOpen, ioClose;
  // bool initilaized_ true if the postion of the mixer is known, default to false 
  bool initilized_ = false;
public:
  // string mxName_ keeps the name of the mixer
  std::string mxName_;

  //// default constructor 
  mixer(std::string mxName, int gpioPin1, int gpioPin2, int openCloseDuration, int steps) :
	openCloseDuration_(openCloseDuration),
	steps_(steps),
	ioOpen(mxName + ".open", gpioPin1),
	ioClose(mxName + ".close", gpioPin2),
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

// FUNCTION handleSigTerm()
//
void handleSigInt(int signum) {
  // Handle the SIGINT signal here
  // Call the destructor of gpioChipCommunication or any other cleanup needed
  std::cout << " Ctrl+C program abort" << std::endl;
  // Exit the program gracefully (if desired)
  exit(0);
}

/*main*/

int main(int argc, char** argv) {
  //// handle SIGINT
  signal(SIGINT, handleSigInt);

  //// setup
  std::vector<gpioOutput> gpioOutputs;
  std::vector<gpioOutput> testLEDs{
	{"red1",23},
	{"red2",5},
	{"red3",24},
	{"yellow1",25},
	{"yellow2",12},
	{"yellow3",16},
	{"green1",19},
	{"green2",26},
	{"green3",20},
	{"blue",22}
  };

  for (const gpioOutput& io : testLEDs) {
	gpioOutputs.push_back(io);
  }

  for (gpioOutput& io : gpioOutputs) {
	io.on();
	usleep(0.2 * 1000000);
  }
  for (gpioOutput& io : gpioOutputs) {
	io.off();
	usleep(0.2 * 1000000);
  }
  /*
  gpioOutput blue("blue led", 22);
  mixer red("red", 23, 5, 5, 16);
  gpioOutput red3("HighTarif", 24), green1("LowTarif", 19);
  */
  //// start
 
  std::cout << gpioOutputs.size() << " instances created." << std::endl;

  /*
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
  */
  //// close 
  return 0;
}
