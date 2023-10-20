#include <iostream>
#include <chrono>
#include <ctime>

int main() {
  // Get the current system time using the system clock
  auto currentTime = std::chrono::system_clock::now();

  // Convert the system time to a time_point object
  std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

  std::string currenTimeStr = std::ctime(&currentTime_t);

  // Convert the time_point to a struct tm
  std::tm* currentTime_tm = std::localtime(&currentTime_t);

  // Extract individual components
  int year = currentTime_tm->tm_year + 1900; // Years since 1900
  int month = currentTime_tm->tm_mon + 1;     // Month (0-based index)
  int day = currentTime_tm->tm_mday;          // Day of the month
  int hour = currentTime_tm->tm_hour;         // Hour
  int minute = currentTime_tm->tm_min;        // Minute
  int second = currentTime_tm->tm_sec;        // Second

  // Print individual components
  std::cout << "Year: " << year << std::endl;
  std::cout << "Month: " << month << std::endl;
  std::cout << "Day: " << day << std::endl;
  std::cout << "Hour: " << hour << std::endl;
  std::cout << "Minute: " << minute << std::endl;
  std::cout << "Second: " << second << std::endl;
  
  
  std::cout << currenTimeStr;

  return 0;
}