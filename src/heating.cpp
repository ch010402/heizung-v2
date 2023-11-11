/*
* GNU GPL v3.0
* heating controls IO's on a raspberryPi that control my house heating 
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
* 
* g++ -c ../src/gpioChipCommunication.cpp -o gpioChipCommunication.o -lgpiod -I../include
* g++ -c ../src/gpioOutput.cpp -o gpioOutput.o -lgpiod -I../include
* g++ gpioChipCommunication.o gpioOutput.o ../src/heating.cpp -o heating -lgpiod -I../include
* 
*/					   

#include <iostream> // used for interacting with the console (cout)
#include <unistd.h> // used for usleep //myabe replace by chrono
#include <string>   // used for strings
#include <memory>
#include <csignal>
#include <vector>
#include <algorithm>
//myClasses 
#include "gpioOutput.h"
#include "temperaturSensor.h"
#include "mischer.h"
#include "checkTime.h"


/*misc*/

/*enumerations*/

/*classes*/

/*functions*/

// FUNCTION handleSigTerm()
//
void handleSigInt(int signum) {
  // Handle the SIGINT signal here
  // TODO: Call the destructor of gpioChipCommunication and any other cleanup needed
  std::cout << "\n\nCtrl+C program abort\n\n";
  // Exit the program gracefully (if desired)
  exit(0);
}


/*main*/

int main(int argc, char** argv) {
  //// handle SIGINT
  signal(SIGINT, handleSigInt);

  //// setup
  bool testBetrieb = true;
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
	{"blue",22},
  };
  std::vector<gpioOutput> productiveOuts{
	{"Elektropumpe",26},
	{"Elektrorelais",4},
	{"Elektromischer auf",25},
	{"Elektromischer zu",24},
	{"Boilerpumpe",20},
	{"Boilerventil", 5},
	{"Heizmischer auf",13},
	{"Heizmischer zu",  6},
	{"Speicherventl",  3},
	{"Umlaufpumpe", 21},
	{"Ofenpumpe Stufe1", 19},
	{"Ofenpumpe Stufe2", 16},
	{"Ofenpumpe Stufe3", 12},
	{"reserve 1", 18},
	{"reserve 2", 27},
  };
  std::vector<std::string> objectsToRemove{
    "green3",
  };

  if (__cplusplus == 201703L)
    std::cout << "C++17" << std::endl;
  else if (__cplusplus == 201402L)
    std::cout << "C++14" << std::endl;
  else if (__cplusplus == 201103L)
    std::cout << "C++11" << std::endl;
  else if (__cplusplus == 199711L)
    std::cout << "C++98" << std::endl;
  else
    std::cout << "pre-standard C++" << std::endl;


  //build all objects 
  for (const gpioOutput& io : testBetrieb ? testLEDs:productiveOuts) {
    gpioOutputs.push_back(io);
  } 
  std::cout << gpioOutputs.size() << " instances in vector." << "\n";
  for (const gpioOutput& io : gpioOutputs) {
	std::cout << "Element in vector: " << io.getName() << "\n";
  }

  for (gpioOutput& io : gpioOutputs) {
	io.on();
	usleep(0.1 * 1000000);
  }
  for (gpioOutput& io : gpioOutputs) {
	io.off();
	usleep(0.1 * 1000000);
  }

  //remove objects
  for (auto it = gpioOutputs.begin(); it != gpioOutputs.end();) {
	if (std::find(objectsToRemove.begin(), objectsToRemove.end(), it->getName()) != objectsToRemove.end()) {
	  it = gpioOutputs.erase(it);
	}
	else {
	  it++;
	}
  }

  std::cout << gpioOutputs.size() << " instances in vector." << "\n";

  for (gpioOutput& io : gpioOutputs) {
	io.on();
	usleep(0.2 * 1000000);
  }
  // Iterate through the remaining elements in the vector and print their names
  for (gpioOutput& io : gpioOutputs) {
	std::cout << "Element in vectro: " << io.getName() << "\n";
  }
  for (gpioOutput& io : gpioOutputs) {
	io.off();
	usleep(0.2 * 1000000);
  }
  
  usleep(2 * 1000000);
  // single led output
  for (gpioOutput& io : gpioOutputs) {
	if (io.getName() == "blue") {
	  std::cout << io.getName() <<" is ON @ pin: " << io.getPin() << " and is: "<< io.getInitialized() << "\n";
	  io.on();
	  usleep(2 * 1000000);
	  io.off();
	}
  }

  //// start

  temperaturSensor sens1("sens1", "28-3c01a8168c89", 0.0);
  std::cout << sens1.getTemp() << std::endl;
 
  std::cout << gpioOutputs.size() << " instances created." << std::endl;

  checkTime nowTime;
  for (int i: nowTime.getTime())
    std::cout << i << ":";
  if (nowTime.checkLowTarif()) {
    std::cout << " Niedertarif\n";
  }
  else {
    std::cout << "HOCHtarif\n";
  }

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
