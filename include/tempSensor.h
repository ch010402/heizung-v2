/*
* Header file for the tempSensor class that reads a temperature value of a ds18b20 1-wire temperature sensor
* Christoph Latzer 2023-10-21
* heating controls IO's on a raspberryPi that control my house heating
* this is the second version based on libgpiod as wiringPi is depreciated and no longer in the distro
* the current version is (libgpiod) v1.6.3 supplied with debian bookworm arm64
*
* GNU GPL v3.0
*/

#ifdef TEMPERATUR_SENSOR_H

#define TEMPERATUR_SENSOR_H

#include <string>
#include <stringstream>

class tempaparturSensor {
public:
  tempaparturSensor(std::string sensorName, std::string sensorAddress, double offset);
  ~tempaparturSensor();
  
  double getTemp();
  std::string getName();
private:
  std::string sensorName_;
  std::string sensorAddresss_;
  double tempOffset_;
  std::string baseDir_;
  std::string tempFile_;
  std::string path_;
};
#endif //!TEMPERATUR_SENSOR_H
