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

// Satic member function to get the instance
checkTime &checkTime::get()
{
  static checkTime instance;
  return instance;
}

/// private Constructor
// void checkTime() creates a time object
checkTime::checkTime() {}

/// private Destructor 
checkTime::~checkTime() {}

/// functions

int checkTime::IgetYear() {
  auto currentTime = std::chrono::system_clock::now();
  std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
  std::string currentTimeStr = std::ctime(&currentTime_t);
  std::tm* currentTime_tm = std::localtime(&currentTime_t);
  return currentTime_tm->tm_year + 1900;
}

int checkTime::IgetMonth() {
  auto currentTime = std::chrono::system_clock::now();
  std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
  std::string currentTimeStr = std::ctime(&currentTime_t);
  std::tm* currentTime_tm = std::localtime(&currentTime_t);
  return currentTime_tm->tm_mon + 1;
}

int checkTime::IgetDay() {
  auto currentTime = std::chrono::system_clock::now();
  std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
  std::string currentTimeStr = std::ctime(&currentTime_t);
  std::tm* currentTime_tm = std::localtime(&currentTime_t);
  return currentTime_tm->tm_mday;
}

int checkTime::IgetHour() {
  auto currentTime = std::chrono::system_clock::now();
  std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
  std::string currentTimeStr = std::ctime(&currentTime_t);
  std::tm* currentTime_tm = std::localtime(&currentTime_t);
  return currentTime_tm->tm_hour;
}

int checkTime::IgetMinute() {
  auto currentTime = std::chrono::system_clock::now();
  std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
  std::string currentTimeStr = std::ctime(&currentTime_t);
  std::tm* currentTime_tm = std::localtime(&currentTime_t);
  return currentTime_tm->tm_min;
}

int checkTime::IgetSecond() {
  auto currentTime = std::chrono::system_clock::now();
  std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
  std::string currentTimeStr = std::ctime(&currentTime_t);
  std::tm* currentTime_tm = std::localtime(&currentTime_t);
  return currentTime_tm->tm_sec;
}

int checkTime::IgetWeekday() {
  auto currentTime = std::chrono::system_clock::now();
  std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
  std::string currentTimeStr = std::ctime(&currentTime_t);
  std::tm* currentTime_tm = std::localtime(&currentTime_t);
  return currentTime_tm->tm_wday;
}

int checkTime::IgetTimeInt()
{
  int timeInt = IgetHour() * 3600 + IgetMinute() * 60 + IgetSecond();
  return timeInt;
}

std::vector<int> checkTime::IgetTime() {
  std::vector<int> nowTime;
  nowTime.push_back(IgetHour());
  nowTime.push_back(IgetMinute());
  nowTime.push_back(IgetSecond());
  return nowTime;
}

std::vector<int> checkTime::IgetDate() {
  std::vector<int> nowDate;
  nowDate.push_back(IgetDay());
  nowDate.push_back(IgetMonth());
  nowDate.push_back(IgetYear());
  return nowDate;
}

bool checkTime::IcheckLowTarif() {
  int weekday_ = IgetWeekday();
  int hour_ = IgetHour();
  if (weekday_ == 0 || weekday_ == 6) {
	return true;
  }
  else if (hour_ < 7 || hour_ >= 20) {
	return true;
  }
  return false;
}

int checkTime::getYear()
{
  return get().IgetYear();
}

int checkTime::getMonth()
{
  return get().IgetMonth();
}

int checkTime::getDay()
{
  return get().IgetDay();
}

int checkTime::getHour()
{
  return get().IgetHour();
}

int checkTime::getMinute()
{
  return get().IgetMinute();
}

int checkTime::getSecond()
{
  return get().IgetSecond();
}

int checkTime::getWeekday()
{
  return get().IgetWeekday();
}

int checkTime::getTimeInt()
{
  return get().IgetTimeInt();
}

std::vector<int> checkTime::getTime()
{
  return get().IgetTime();
}

std::vector<int> checkTime::getDate()
{
  return get().IgetDate();
}

bool checkTime::checkLowTarif()
{
  return get().IcheckLowTarif();
}
