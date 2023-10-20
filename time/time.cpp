#include <chrono>
#include <iostream>
#include <strstream>

int hour;
auto now = std::chrono::system_clock::now();
std::time_t in_time_t = std::chrono::system_clock::to_time_t(now);
std::tm buffer;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
//define something for Windows (32-bit and 64-bit, this part is common)
localtime_s(&buffer, &in_time_t);
#elif defined __linux__
localtime_r(&in_time_t, &buffer);
#else
#   error "Unknown compiler"
#endif
std::stringstream ss;
ss << std::put_time(&buffer, "%H");
ss >> hour;

std::cout << hour;