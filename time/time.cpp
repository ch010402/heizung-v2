#include <chrono>
#include <iostream>

auto now = std::chrono::system_clock::now();
std::time_t in_time_t = std::chrono::system_clock::to_time_t(now);
std:tm buffer;
localtime_r(&in_time_t, &buffer);

std::cout << now;