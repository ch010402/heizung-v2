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
  // Pumps
  gpioOutput elektroPumpe("ElektroPumpe", 26), 
    boilerPumpe("Boilerpumpe", 20), 
    umlaufPumpe("Umlaufpumpe", 21),
    ofenPupmeS1("Ofenpumpe Stufe1", 19),
    ofenPupmeS2("Ofenpumpe Stufe2", 16),
    ofenPupmeS3("Ofenpumpe Stufe3", 12)
    ;
  // switches
  gpioOutput elektroRelais("Elektrorelais", 4),
    elektroMischerOpener("Elektromischer open", 6),
    elektroMischerCloser("Elektromischer closer", 5),
    heizMischerOpener("Heizmischer opener", 23),
    heizMischerCloser("Heizmischer closer", 22)
    ;
  // reserve
  gpioOutput reserve1("reseve1", 18),
    reserve2("reseve2", 27)
    //reserve3("reseve3", 18)
    ;
  // mischer
  mischer elektroMischer("Elektromischer", elektroMischerOpener, elektroMischerCloser, 82, 10),
    heizMischer("Heizmischer", heizMischerOpener, heizMischerCloser, 60, 10)
    ;

  //// start
  // set all pumps to off
  elektroPumpe.off();
  boilerPumpe.off();
  ofenPupmeS1.off();
  ofenPupmeS2.off();
  ofenPupmeS3.off();
  // close all mischer
  elektroMischer.setStep(1);
  heizMischer.setStep(1);
  // set all switches off
  elektroRelais.off();
  reserve1.off();
  reserve2.off();

  //// loop

  //// close 
  return 0;
}
