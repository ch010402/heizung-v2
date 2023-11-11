/*
* Header file for the time class used to give system time and some more functions back 
* Christoph Latzer 2023-10-21
* heating controls IO's on a raspberryPi that control my house heating
* this is the second version based on libgpiod as wiringPi is depreciated and no longer in the distro
* the current version is (libgpiod) v1.6.3 supplied with debian bookworm arm64
*
* GNU GPL v3.0
*/

#ifndef CHECKTIME_H

#define CHECKTIME_H

#include <chrono>
#include <string>
#include <vector>

class checkTime {
public:
  checkTime();
  ~checkTime();

  int getYear();
  int getMonth();
  int getDay();
  int getHour();
  int getMinute();
  int getSecond();
  int getWeekday();
  std::vector<int> getTime();
  std::vector<int> getDate();
  bool checkLowTarif();
private:
  struct actualTime;
  struct actualDate;
};

#endif //!CHECKTIME_H