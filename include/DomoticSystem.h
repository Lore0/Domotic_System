#ifndef DOMOTICSYSTEM_H
#define DOMOTICSYSTEM_H

#include "Device.h"
#include "Scheduler.h"
#include "Time.h"
#include <string>
#include <vector>

class DomoticSystem {
  public:
    DomoticSystem();
    std::vector<Device> devices_;
    std::string handleInput(const std::string input_);
    std::string getCustomInput();
  private:
    Scheduler scheduler_;
    double kMaxPower = 3.5;
    std::string parseTime(const std::string& time_);
    bool isValidTimeFormat(const std::string& time);
    void buildDevices();
    void log(const std::string& message_);
    Device* searchDevice(const std::string& name_);
    std::string setOn(Device& device_); 
    std::string setOff(Device& device_); 
    std::string setDeviceTimer(Device& device_); 
    std::string removeFromRunning(Device& device_); 
    std::string removeFromWaiting(Device& device_); 
    // show
    // show device
    std::string setTime(Time& time_);
    std::string resetTime();
    std::string resetTimers();
    std::string resetAll(); 
};

#endif // DOMOTICSYSTEM_H
