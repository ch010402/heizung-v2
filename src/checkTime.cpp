/*
* Source file for the time class that returns portions of now
* Christoph Latzer 2023-11-11
* heating controls IO's on a raspberryPi that control my house heating
* this is the second version based on libgpiod as wiringPi is depreciated and no longer in the distro
* the current version is (libgpiod) v1.6.3 supplied with debian bookworm arm64
*
* GNU GPL v3.0
* 
* to prcompile use:
* g++ -c ../src/mischer.cpp -o mischer.o -I../include
*/

#include "checkTime.h"

// Constructor 

// void checkTime() creates a time object
checkTime::checkTime() {
  
}

// Destructor 
checkTime::~checkTime()
{
}


// functions

int checkTime::getYear() {
  auto currentTime = std::chrono::system_clock::now();
  std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
  std::string currentTimeStr = std::ctime(&currentTime_t);
  std::tm* currentTime_tm = std::localtime(&currentTime_t);
  return currentTime_tm->tm_year + 1900;
}

int checkTime::getMonth() {
  auto currentTime = std::chrono::system_clock::now();
  std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
  std::string currentTimeStr = std::ctime(&currentTime_t);
  std::tm* currentTime_tm = std::localtime(&currentTime_t);
  return currentTime_tm->tm_mon + 1;
}

int checkTime::getDay() {
  auto currentTime = std::chrono::system_clock::now();
  std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
  std::string currentTimeStr = std::ctime(&currentTime_t);
  std::tm* currentTime_tm = std::localtime(&currentTime_t);
  return currentTime_tm->tm_mday;
}

int checkTime::getHour() {
  auto currentTime = std::chrono::system_clock::now();
  std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
  std::string currentTimeStr = std::ctime(&currentTime_t);
  std::tm* currentTime_tm = std::localtime(&currentTime_t);
  return currentTime_tm->tm_hour;
}

int checkTime::getMinute() {
  auto currentTime = std::chrono::system_clock::now();
  std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
  std::string currentTimeStr = std::ctime(&currentTime_t);
  std::tm* currentTime_tm = std::localtime(&currentTime_t);
  return currentTime_tm->tm_min;
}

int checkTime::getSecond() {
  auto currentTime = std::chrono::system_clock::now();
  std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
  std::string currentTimeStr = std::ctime(&currentTime_t);
  std::tm* currentTime_tm = std::localtime(&currentTime_t);
  return currentTime_tm->tm_sec;
}

int checkTime::getWeekday() {
  auto currentTime = std::chrono::system_clock::now();
  std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
  std::string currentTimeStr = std::ctime(&currentTime_t);
  std::tm* currentTime_tm = std::localtime(&currentTime_t);
  return currentTime_tm->tm_wday;
}

int checkTime::getTimeInt()
{
  int timeInt = getHour() * 3600 + getMinute() * 60 + getSecond();
  return timeInt;
}

std::vector<int> checkTime::getTime() {
  std::vector<int> nowTime;
  nowTime.push_back(getHour());
  nowTime.push_back(getMinute());
  nowTime.push_back(getSecond());
  return nowTime;
}

std::vector<int> checkTime::getDate() {
  std::vector<int> nowDate;
  nowDate.push_back(getDay());
  nowDate.push_back(getMonth());
  nowDate.push_back(getYear());
  return nowDate;
}

bool checkTime::checkLowTarif() {
  int weekday_ = getWeekday();
  int hour_ = getHour();
  if (weekday_ == 0 || weekday_ == 6) {
	return true;
  }
  else if (hour_ < 7 || hour_ >= 20) {
	return true;
  }
  return false;
}
