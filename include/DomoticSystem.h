#ifndef DOMOTICSYSTEM_H
#define DOMOTICSYSTEM_H

#include "Manual.h"
#include "PrefixCycle.h"
#include "Scheduler.h"
#include "Time.h"
#include <string>
#include <vector>

class DomoticSystem {
  public:
    DomoticSystem();
    std::vector<Manual> manual_;
    std::vector<PrefixCycle> prefixCycle_;
    std::string handleInput(const std::string& input_);
    std::string getCustomInput();
    void log(const std::string& message_, std::ofstream& logFile);
  private:
    Scheduler scheduler_;
    double kMaxPower = 3.5;
    bool isValidTimeFormat(const std::string& time);
    void buildDevices();
    Manual* searchManual(const std::string& name_);
    PrefixCycle* searchPrefixCycle(const std::string& name_);
    std::string logTime();
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
