/*
* Header file for the tempSensor class that reads a temperature value of a ds18b20 1-wire temperature sensor
* Christoph Latzer 2023-10-21
* heating controls IO's on a raspberryPi that control my house heating
* this is the second version based on libgpiod as wiringPi is depreciated and no longer in the distro
* the current version is (libgpiod) v1.6.3 supplied with debian bookworm arm64
*
* GNU GPL v3.0
*/

#ifndef TEMPERATUR_SENSOR_H

#define TEMPERATUR_SENSOR_H

#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <iostream>

//struct TempSensData (string sensorName, string sensorAddress, double offset)
struct TempSensData {
  std::string sensorName;
  std::string sensorAddress;
  double offset;
};

class temperaturSensor {
public:
  temperaturSensor(const TempSensData& data);
  ~temperaturSensor();
  
  double getTemp();
  std::string getName();
  double getOffset();
private:
  std::string sensorName_;
  std::string sensorAddresss_;
  double tempOffset_;
  TempSensData data;
  double temperatur_;
  std::string baseDir_;
  std::string tempFile_;
  std::string path_;
  std::string rawData_;
  std::ifstream infile_;
  std::stringstream buffer_;
  size_t crcCheck_;
  size_t temparaturValue_;
  std::string strTemparatur_;
};
#endif //!TEMPERATUR_SENSOR_H
