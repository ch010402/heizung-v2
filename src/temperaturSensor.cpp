/*
* Source file for the tempSensor class that reads a temperature value of a ds18b20 1-wire temperature sensor
* Christoph Latzer 2023-10-29
* heating controls IO's on a raspberryPi that control my house heating
* this is the second version based on libgpiod as wiringPi is depreciated and no longer in the distro
* the current version is (libgpiod) v1.6.3 supplied with debian bookworm arm64
*
* GNU GPL v3.0
*/

#include <iostream>
#include "temperaturSensor.h"

// define statics 
std::string baseDir_ = "/sys/bus/w1/devices/";
std::string tempFile_ = "/w1_slave";


//Constructor
temperaturSensor::temperaturSensor(std::string sensorName, std::string sensorAddress, double offset) :
sensorName_(sensorName), sensorAddresss_(sensorAddress), tempOffset_(offset) {
  path_ = baseDir_ + sensorAddresss_ + tempFile_;
  std::cout << sensorName_ << " on " << path_ << " erstellt." << std::endl;
}

//Destructor
temperaturSensor::~temperaturSensor() {}

// functions

// returns the temparature as a double in °C
double temperaturSensor::getTemp(){
  std::ifstream infile_(path_);
  if (infile_) {
    buffer_ << infile_.rdbuf();
    infile_.close();
    rawData_ = buffer_.str();
  }
  else {
    infile_.close();
    std::cout << "Error could not read sensor: " << sensorName_ << std::endl;
    return -100;
  }
  size_t crcCheck_ = rawData_.find("YES");
  if (crcCheck_ == std::string::npos) {
    std::cout << "Error CRC not valid: " << sensorName_ << std::endl;
    return -101;
  }
  size_t temparaturValue_ = rawData_.find("t=");
  if (temparaturValue_ == std::string::npos) {
    std::cout << "Failed to find a temaparatur value: " << sensorName_ << std::endl;
  }
  std::string strTemparatur_ = rawData_.substr(temparaturValue_+2);
  double temperatur_ = stod(strTemparatur_) / 1000;
  return temperatur_;
}
// returns the sensor name
std::string temperaturSensor::getName(){
  return sensorName_;
}
// returns the offset of the temperatur sensor
double temperaturSensor::getOffset() {
  return tempOffset_;
}