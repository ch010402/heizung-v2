#include <iostream>
#include <chrono>
#include <ctime>

int main() {
  auto currentTime = std::chrono::system_clock::now();

  std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

  std::string currenTimeStr = std::ctime(&currentTime_t);

  std::count << currenTimeStr;

  return 0;
}