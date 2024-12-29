#ifndef DOMOTICSYSTEM_H
#define DOMOTICSYSTEM_H

#include "Device.h"
#include "Scheduler.h"
#include "Time.h"
#include <string>
#include <vector>

class DomoticSystem {
  public:
    DomoticSystem(Scheduler s);
    std::vector<Device> devices_;
    void handleInput(const std::string input_);
    std::string getCustomInput();
  private:
    Scheduler scheduler_;
    std::string parseTime(std::string time_);
    void buildDevices();
    Device searchDevice(std::string name_);
    void setOn(Device& device_); // must put null time end
    void setOff(Device& device_); // must set null all time
    void setDeviceTimer(Device& device_); // need to set the time on Device class
    void removeFromRunning(Device& device_); // must check if device is on or not
    void removeFromWaiting(Device& device_); // must check if device is on or not
    // show
    // show device
    void setTime(Time& time_);
    void resetTime();
    void resetTimers();
    void resetAll(); // call resetTime and resetTimers
};

#endif // DOMOTICSYSTEM_H
