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

// classes 
class io {
public:
  std::string name;
  int ioPin;
  bool status = false;
  bool initialized = false;
  std::string type;
  // constructor
  io(std::string ioName, int gpioPin, std::string ioType) {
	name = ioName;
	ioPin = gpioPin;
	type = ioType;
  }
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
  void destroy() {
	gpiod_line_release(line); // close GPIO line
	openLineCounter--;
	std::cout << openLineCounter << std::endl;
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
	std::cout << openLineCounter << std::endl;
	initialized = true;
  }
};

// fuction
bool checkName(std::string str) {
  if (!std::regex_match(str, std::regex("[A-Za-z0-9\\-_]+")))
	throw std::invalid_argument("Name should only contain letters -_ and numbers");
  return 0;
};


// MAIN
int main(int argc, char** argv) {
  // setup
  io rot1("rot1", 23, "led");
  io blue("blue", 22, "led");
  int i = 0;
  // loop
  while (i < 11) {
	rot1.on();
	blue.on();
	std::cout << "output " << rot1.name << " status " << rot1.status << std::endl;
	usleep(0.1 * 1000000); //in sec
	rot1.off();
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