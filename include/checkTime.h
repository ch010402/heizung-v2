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

// void checkTime() creates a time object
class checkTime {

public:
  static checkTime& get();
  
  // copy constructor and assignment operator to prevent cloning
  checkTime(const checkTime&) = delete;
  checkTime& operator=(const checkTime&) = delete;

  static int getYear();
  static int getMonth();
  static int getDay();
  static int getHour();
  static int getMinute();
  static int getSecond();
  static int getWeekday();
  static int getTimeInt();
  static std::vector<int> getTime();
  static std::vector<int> getDate();
  static bool checkLowTarif();

private:
  // Private constructor to prevent external instantiation
  checkTime();
  // Private destructor to prevent external deletion
  ~checkTime();
  
  // memeber functions
  int IgetYear();
  int IgetMonth();
  int IgetDay();
  int IgetHour();
  int IgetMinute();
  int IgetSecond();
  int IgetWeekday();
  int IgetTimeInt();
  std::vector<int> IgetTime();
  std::vector<int> IgetDate();
  bool IcheckLowTarif();
  // member structs
  struct actualTime;
  struct actualDate;
};

#endif //!CHECKTIME_H