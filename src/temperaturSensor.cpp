/*
* Source file for the tempSensor class that reads a temperature value of a ds18b20 1-wire temperature sensor
* Christoph Latzer 2023-10-29
* heating controls IO's on a raspberryPi that control my house heating
* this is the second version based on libgpiod as wiringPi is depreciated and no longer in the distro
* the current version is (libgpiod) v1.6.3 supplied with debian bookworm arm64
*
* GNU GPL v3.0
* 
* to prcompile use:
* g++ -c ../src/temperaturSensor.cpp -o temperaturSensor.o -I../include
*/

#include <iostream>
#include "temperaturSensor.h"

// ANSI escape codes for text color
#define RED_TEXT "\033[1;31m"
#define RESET_COLOR "\033[0m"

// define statics 


//Constructor

// temperaturSensor(string sensorName, string sensorAddress, double offset)
temperaturSensor::temperaturSensor(std::string sensorName, std::string sensorAddress, double offset) :
sensorName_(sensorName), sensorAddresss_(sensorAddress), tempOffset_(offset) {
  baseDir_ = "/sys/bus/w1/devices/";
  tempFile_ = "/w1_slave";
  path_ = baseDir_ + sensorAddresss_ + tempFile_;
  //debug text std::cout << sensorName_ << " on " << path_ << " erstellt." << std::endl;
}

//Destructor
temperaturSensor::~temperaturSensor() {}

// functions

// double getTemp() returns the temparature as a double in °C
double temperaturSensor::getTemp() {
  std::ifstream infile_(path_);
  if (infile_) {
    buffer_ << infile_.rdbuf();
    infile_.close();
    rawData_ = buffer_.str();
  }
  else {
    infile_.close();
    std::cout << RED_TEXT << "Error could not read sensor file: " << RESET_COLOR << sensorName_ << " ";
    return -100;
  }
  size_t crcCheck_ = rawData_.find("YES");
  if (crcCheck_ == std::string::npos) {
    std::cout << "Error CRC not valid: " << sensorName_ << " ";
    return -101;
  }
  size_t temparaturValue_ = rawData_.find("t=");
  if (temparaturValue_ == std::string::npos) {
    std::cout << "Failed to find a temaparatur value: " << sensorName_ << " ";
    return -102;
  }
  std::string strTemparatur_ = rawData_.substr(temparaturValue_+2);
  double temperatur_ = stod(strTemparatur_) / 1000;
  return temperatur_;
}

// string getName() returns the sensor name
std::string temperaturSensor::getName() {
  return sensorName_;
}

// double getOffset returns the offset of the temperatur sensor in °C
double temperaturSensor::getOffset() {
  return tempOffset_;
}