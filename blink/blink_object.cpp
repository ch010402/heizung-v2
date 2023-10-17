/* see license file
* compile with: g++ -Wall -o blink10 blink10.cpp -lgpiod
*/

#include <gpiod.h> // library to access the GPIO Pins on a raspberryPI
#include <unistd.h> // used for usleep
#include <string> // string class
#include <iostream> // cout

//// variable

// counter for how many lines to the io chip are opend 
int openLineCounter = 0;
// enables disbles debug console output 
bool debugOutput = true;

//// enumeration

// ioType enumerate possible values {pump, valve, breaker, led}
enum ioType {
  pump, valve, breaker, led
};

//// classes

// io class (string ioName, int gpioPin, enum ioType { pump, valve, breaker })
class io {
private:
  // GPIO pin to access 
  int ioPin;
  // boolean to check wheter the io is setup or needs to bi initilaized 
  bool initialized = false;
public:
  // io status default to false meaning off not set, reports the status as boolean 
  bool status = false;
  // ioType enumerate possible values {pump, valve, breaker, led}
  ioType typ;
  // constructor
//io(std::string ioName, int gpioPin, ioType type) {
  io(int gpioPin, ioType type) {
	ioPin = gpioPin;
	typ = type;
  }
  // io.on() function switches the io on 
  void on() {
	if (!initialized) initialize();
	gpiod_line_set_value(line, 1);
	status = true;
  }
  // io.off() function switches the io off 
  void off() {
	if (!initialized) initialize();
	gpiod_line_set_value(line, 0);
	status = false;
  }
  // io.toggle() function switches the state of the io from off to on and from on to off
  void toggle() {
	if (!initialized) initialize();
	if (status == false) {
	  gpiod_line_set_value(line, 1);
	  status = true;
	}
	else if (status == true) {
	  gpiod_line_set_value(line, 0);
	  status = false;
	}
  }
  // io.destroy() function closes the line to the io and if it is the last line the connection to the chip 
  void destroy() {
	gpiod_line_release(line); // close GPIO line
	openLineCounter--;
	if (debugOutput)
	  std::cout << "IO line " << openLineCounter + 1 << " closed, " << openLineCounter << " remain." << std::endl;
	if (openLineCounter == 0) {
	  gpiod_chip_close(chip); // close chip
	  if (debugOutput)
		std::cout << "Connection to chip closed" << std::endl;
	}
	initialized = false;
  }
private:
  // build gpiod lines
  struct gpiod_chip* chip; 
  // build gpiod lines
  struct gpiod_line* line;
  // io.initialize() builds up the connection to the chip and creates a line to the io
  void initialize() {
	// set the chip name as const
	const char* chipname = "gpiochip0"; 
	// open GPIO chip
	chip = gpiod_chip_open_by_name(chipname); 
	// open GPIO line
	line = gpiod_chip_get_line(chip, ioPin); 
	// request line as output
	gpiod_line_request_output(line, "output", 0); 
	openLineCounter++;
	if (debugOutput)
	  std::cout <<"IO line " << openLineCounter << " opend." << std::endl;
	initialized = true;
  }
};

// mixer class (string mxName, int gpioPin1, int gpioPin2, int openCloseDuration)
class mixer {
private:
  int range, step;
  io ioOpen, ioClose;
public:
  // constructor 
  mixer(int gpioPin1, int gpioPin2, int openCloseDuration, int steps)
	: ioOpen(gpioPin1, breaker), ioClose(gpioPin2, breaker) {
	range = openCloseDuration;
	////step = steps;
  }
public:
  void open() {
	ioClose.off();
	ioOpen.toggle();
	waitstep();
	ioOpen.off();
  }
  void close() {
	ioOpen.off();
	ioClose.toggle();
	waitstep();
	ioClose.off();
  }
  void destroy() {
	ioOpen.destroy();
	ioClose.destroy();
  }
private:
  void waitstep() {
	// wait 1/10 of the full open close cycle in seconds
	usleep(range * 1000000 / step);
  }
};

//// MAIN
int main(int argc, char** argv) {
  //// setup

  // build io objects 
  io rot1(23, pump), 
	blue(22, led);

  mixer elektro(5, 24, 82, 10);
  
  // loop; i = loop counter
  int i = 0;
  while (i < 6) {
	rot1.toggle();
	blue.on();
//	std::cout << "output " << rot1.name << " status " << rot1.status << std::endl;
	usleep(0.3 * 1000000); //in sec
	rot1.toggle();
	blue.off();
//	std::cout << "output " << blue.name << " status " << blue.status << std::endl;
	usleep(0.7 * 1000000); //in sec
	elektro.close();
	elektro.open();
	i++;
	std::cout << "Loop end." << std::endl;
  }
  //clean up
  elektro.destroy();
  rot1.destroy();
  blue.destroy();
  // end
  return 0;
}