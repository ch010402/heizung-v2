/* see license file
* compile with: g++ -Wall -o blink10 blink10.cpp -lgpiod
*/

#include <gpiod.h> // library to access the GPIO Pins on a raspberryPI
#include <unistd.h> // used for usleep
#include <string> // string class
#include <regex> // used for checking regualar expressions
#include <iostream> // cout
#include <stdexcept>

// variable
int openLineCounter = 0;
bool debugOutput = false;

// classes 

// io class (string ioName, int gpioPin, string ioType)
class io {
private:
  int ioPin;
  bool initialized = false;
public:
  std::string name;
  bool status = false;
  std::string type;
  // constructor
  io(std::string ioName, int gpioPin, std::string ioType) {
	name = ioName;
	ioPin = gpioPin;
	type = ioType;
  }
  // member functinons
  void on() {
	if (!initialized) initialize();
	gpiod_line_set_value(line, 1);
	status = true;
  }
  void off() {
	if (!initialized) initialize();
	gpiod_line_set_value(line, 0);
	status = false;
  }
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
  void destroy() {
	gpiod_line_release(line); // close GPIO line
	openLineCounter--;
	if (debugOutput)
	  std::cout << "Line " << openLineCounter + 1 << " closed, " << openLineCounter << " remain." << std::endl;
	if (openLineCounter == 0) {
	  gpiod_chip_close(chip); // close chip
	  if (debugOutput)
		std::cout << "connection to chip closed" << std::endl;
	}
	initialized = false;
  }
private:
  struct gpiod_chip* chip; // build gpiod lines
  struct gpiod_line* line; // build gpiod lines
  void initialize() {
	const char* chipname = "gpiochip0"; // set the chip name as const
	chip = gpiod_chip_open_by_name(chipname); // open GPIO chip
	line = gpiod_chip_get_line(chip, ioPin); // open GPIO line
	gpiod_line_request_output(line, "output", 0); // request line as output
	openLineCounter++;
	if (debugOutput)
	  std::cout <<"Line " << openLineCounter << " opend." << std::endl;
	initialized = true;
  }
};

// MAIN
int main(int argc, char** argv) {
  // setup
  io rot1("rot1", 23, "led"), blue("blue", 22, "led");
  int i = 0;
  // loop
  while (i < 11) {
	rot1.toggle();
	blue.on();
	std::cout << "output " << rot1.name << " status " << rot1.status << std::endl;
	usleep(0.1 * 1000000); //in sec
	rot1.toggle();
	blue.off();
	std::cout << "output " << blue.name << " status " << blue.status << std::endl;
	usleep(1 * 1000000); //in sec
	i++;
	std::cout << "loop ende" << std::endl;
  }
  rot1.destroy();
  blue.destroy();
  // end
  return 0;
}