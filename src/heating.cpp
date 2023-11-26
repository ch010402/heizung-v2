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
#include <csignal>
#include <vector>
#include <algorithm>
#include <functional>
//myClasses 
#include "gpioOutput.h"
#include "temperaturSensor.h"
#include "mischer.h"
#include "checkTime.h"
#include "valve.h"

/*misc*/

/*enumerations*/

/*classes*/

/*functions*/

// FUNCTION handleSigTerm()
//
void handleSigInt(int signum) {
  // Handle the SIGINT signal here
  std::cout << "\n\nCtrl+C program abort\n\n";
  // TODO: Call the destructor of gpioChipCommunication and any other cleanup needed
  
  // Exit the program gracefully (if desired)
  exit(0);
}


/*main*/

int main(int argc, char** argv) {
  //// handle SIGINT
  signal(SIGINT, handleSigInt);

//// setup

  /// Pumps
  // create a vector of GpioOutData structs for each pump we have in the system 
  std::vector<GpioOutData> pumpsData = {
    {"Elektropumpe", 26},
    {"Boilerpumpe", 20},
    {"Umlaufpumpe", 21},
    {"Ofenpumpe Stufe1", 19},
    {"Ofenpumpe Stufe2", 16},
    {"Ofenpumpe Stufe3", 12}
  };
  /// Switches
  // create a vector of GpioOutData structs for each switch we have in the system 
  std::vector<GpioOutData> switchData = {
    {"Elektrorelais", 23},
    {"Elektromischer opener", 25},
    {"Elektromischer closer", 24},
    {"Heizmischer opener", 13},
    {"Heizmischer closer", 6}
  };
  /// Valves
  // create a vector of ValveData structs for the valves in the system
  std::vector<ValveData> valveData = {
    {{"Boilerventil", 5}, 20},
    {{"Speicherventil", 22}, 15}
  };
  /// Reserve
  // create a vector of GpioOutData structs for the spare output 
   std::vector<GpioOutData> spareData = {
    {"reseve1", 18},
    {"reseve2", 27},
    {"reseve3", 17}
   };
  /// TemperaturSensors
  // create a vector of TempSensData structs for each temperature sensor we have in the system
  std::vector<TempSensData> tSensorData = {
    {"Aussen Temperatur", "28-0416a10c28ff", 0.0},
    {"Heizung Vorlauf", "28-0516a12e05ff", 0.0},
    {"Heizung Rücklauf", "28-0316a162a9ff", 0.0},
    {"Boiler Volrauf", "28-0516a1529bff", 0.0},
    {"Boiler Rücklauf", "28-0416a10f65ff", 0.0},
    {"Boiler Temperatur oben", "28-0416a13049ff", 0.0},
    {"Boiler Temperatur unten", "28-0416a1295fff", 0.0},
    {"Durchlauferhitzer Vorlauf", "28-0516a15419ff", 0.0},
    {"Durchlauferhitzer Rücklauf", "28-0316a15a36ff", 0.0},
    {"Ofen Vorlauf", "28-0416a12058ff", 0.0},
    {"Ofen Rücklauf", "28-0416a10e34ff", 0.0},
    {"Speicher oben", "28-0316a15f04ff", 0.0},
    {"Speicher mitte", "28-0416a10cc1ff", 0.0},
    {"Speicher unten", "28-0416a10de1ff", 0.0},
    {"Test Sensor 1", "28-3c01a8168c89", 0.0}
  };

  // create a vector with pointers to all pumps 
  std::vector<std::__shared_ptr<gpioOutput>> pumps;
  for (const auto& data : pumpsData) {
    pumps.push_back(std::make_shared<gpioOutput>(data));
  }
   // create a vector with pointers to all stwitches 
  std::vector<std::__shared_ptr<gpioOutput>> switches;
  for (const auto& data : switchData) {
    switches.push_back(std::make_shared<gpioOutput>(data));
  }
  // create a vector with pointers to all valves 
  std::vector<std::__shared_ptr<valve>> valves;
  for (const auto& data : valveData) {
    valves.push_back(std::make_shared<valve>(data));
  }
  // create a vector with pointers to all spares 
  std::vector<std::__shared_ptr<gpioOutput>> spares;
  for (const auto& data : spareData) {
    spares.push_back(std::make_shared<gpioOutput>(data));
  }
  // create a vector with pointers to all temperatur sensors 
  std::vector<std::__shared_ptr<temperaturSensor>> tSensor;
  for (const auto& data : tSensorData) {
    tSensor.push_back(std::make_shared<temperaturSensor>(data));
  }

  // give meaningful names to the pionters of the pump vector 
  std::__shared_ptr<gpioOutput> 
    elektroPumpe = pumps[0],
    boilerPumpe = pumps[1],
    umlaufPumpe = pumps[2],
    ofenPupmeS1 = pumps[3],
    ofenPupmeS2 = pumps[4],
    ofenPupmeS3 = pumps[5];
  
  // give meaningful names to the pionters of the switch vector 
  std::__shared_ptr<gpioOutput> 
    elektroRelais = switches[0],
    elektroMischerOpener = switches[1],
    elektroMischerCloser = switches[2],
    heizMischerOpener = switches[3],
    heizMischerCloser = switches[4];
   
  // give meaningful names to the pionters of the spares 
  std::__shared_ptr<gpioOutput> 
    reserve1 = spares[0],
    reserve2 = spares[1],
    reserve3 = spares[2];

  // give meaningful names to the pionters of the switch vector 
  std::__shared_ptr<valve> 
    boilerValve = valves[0],
    speicherValve = valves[1];

  // mischer
  mischer elektroMischer("Elektromischer", *elektroMischerOpener, *elektroMischerCloser, 5, 10), //default 82
    heizMischer("Heizmischer", *heizMischerOpener, *heizMischerCloser, 5, 10); //default 60

  // give meaningful names to the pionters of the tSensor vector
  std::__shared_ptr<temperaturSensor> 
    aussenTemperatur = tSensor[0],
    heizungVorlauf = tSensor[1],
    heizungRuecklauf = tSensor[2],
    boilerVorlauf = tSensor[3],
    boilerRuecklauf = tSensor[4],
    boilerOben = tSensor[5],
    boilerUnten = tSensor[6],
    elektroVorlauf = tSensor[7],
    elektroReucklauf = tSensor[8],
    ofenVorlauf = tSensor[9],
    ofenRuecklauf = tSensor[10],
    speicherOben = tSensor[11],
    speicherMitte = tSensor[12],
    speicherUnten = tSensor[13],
    testSensor = tSensor[14];

//// start
  // set all pumps to off
  for (const auto& p : pumps) {
    p->off();
  }
  // set all switches off
  for (const auto& s : switches) {
    s->off();
  }
  // set all spares to off
  for (const auto& sp : spares) {
    sp->off();
  }
  // close all valves
  for (const auto& v : valves) {
    v->off();
  }
  // close all mischer
  elektroMischer.setStep(1);
  heizMischer.setStep(1);
  
  // check all temperatur sensors and remove from vector if not found
  std::cout << tSensor.size() << " elements\n";
  tSensor.erase(std::remove_if(tSensor.begin(), tSensor.end(),
    [](const auto& sens) {
      try {
        sens->getTemp(); // Try accessing the temperature
        return false; // Keep the sensor in the vector
      } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return true; // Remove the sensor from the vector
      }
    }), tSensor.end());
  std::cout << tSensor.size() << " elements\n";
  
  // Print remaining sensors
  for (const auto& sens : tSensor) {
    std::cout << sens->getName() << ": " << sens->getTemp() << "°C\n";
  }

//// testing
//usleep(15 * 1000000);
std::cout << "switch on 1 \n";
try {
  boilerValve->on();
}
catch (const std::exception& e) {
  std::cerr << e.what() << std::endl;
  usleep(boilerValve->waitToSwitch * 1000000);
  boilerValve->on();
}
usleep(5 * 1000000);
std::cout << "switch on 2 \n";
try {
boilerValve->on();}
catch (const std::exception& e) {
  std::cerr << e.what() << std::endl;
  usleep(boilerValve->waitToSwitch * 1000000);
  boilerValve->on();
}
std::cout << "switch off \n";
try {
boilerValve->off();}
catch (const std::exception& e) {
  std::cerr << e.what() << std::endl;
  usleep(boilerValve->waitToSwitch * 1000000);
  boilerValve->off();
}
//// loop

//// close 
return 0;
}
