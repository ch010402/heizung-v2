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

#include "temperaturSensor.h"

// ANSI escape codes for text color
#define RED_TEXT "\033[1;31m"
#define RESET_COLOR "\033[0m"

// define statics 

//Constructor

// temperaturSensor(string sensorName, string sensorAddress, double offset)
temperaturSensor::temperaturSensor(const TempSensData& data)
  :  sensorName_(data.sensorName), 
  sensorAddresss_(data.sensorAddress), 
  tempOffset_(data.offset),
  baseDir_("/sys/bus/w1/devices/"),
  tempFile_("/w1_slave"),
  path_(baseDir_ + sensorAddresss_ + tempFile_) {
  //debug text std::cout << sensorName_ << " on " << path_ << " created." << std::endl;
}

//Destructor
temperaturSensor::~temperaturSensor() {}

// functions

// double getTemp() returns the temparature as a double in °C
double temperaturSensor::getTemp() {
  infile_.open(path_);
  if (!infile_) {
    throw std::runtime_error("Error could not read sensor file: " + sensorName_);
  }
  buffer_ << infile_.rdbuf();
  infile_.close();
  rawData_ = buffer_.str();
  
  crcCheck_ = rawData_.find("YES");
  if (crcCheck_ == std::string::npos) {
    throw std::runtime_error("Error CRC not valid: " + sensorName_);
  }

  temparaturValue_ = rawData_.find("t=");
  if (temparaturValue_ == std::string::npos) {
    throw std::runtime_error("Failed to find a temaparatur value: " + sensorName_);
  }

  strTemparatur_ = rawData_.substr(temparaturValue_+2);
  temperatur_ = stod(strTemparatur_) / 1000;
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