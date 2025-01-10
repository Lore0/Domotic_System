#ifndef DOMOTICSYSTEM_H
#define DOMOTICSYSTEM_H

#include "Manual.h"
#include "PrefixCycle.h"
#include "Scheduler.h"
#include "Clock.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <cmath>

class DomoticSystem {
  public:
    DomoticSystem();
    std::vector<Manual> manual_;
    std::vector<PrefixCycle> prefixCycle_;
    std::string handleInput(const std::string& input_);
    std::string getCustomInput();
    bool getSystemState() const;
    void log(const std::string& message_, std::ofstream& logFile);
  private:
    Scheduler scheduler_;
    const double kMaxPower = 3.5;
    bool status_ = false;
    void setSystemState(bool state);
    bool isValidTimeFormat(const std::string& time);
    void buildDevices();
    Manual* searchManual(const std::string& name_);
    PrefixCycle* searchPrefixCycle(const std::string& name_);
    std::string logTime();
    std::string logError(const std::string& error);
    std::string setOn(Device& device_);
    std::string setOff(Device& device_);
    std::string setDeviceTimer(Device& device_);
    std::string removeFromRunning(Device& device_);
    std::string removeFromWaiting(Device& device_);
    std::string showDevice(Device& device_);
    std::string showAll();
    std::string setTime(Clock& time_);
    std::string resetTime();
    std::string resetTimers();
    std::string resetAll();
};

#endif // DOMOTICSYSTEM_H
