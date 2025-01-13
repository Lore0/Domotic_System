#ifndef DOMOTICSYSTEM_H
#define DOMOTICSYSTEM_H

#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include "Clock.h"
#include "Manual.h"
#include "PrefixCycle.h"
#include "Scheduler.h"

// DomoticSystem: Class for managing the domotic system.
class DomoticSystem {
 public:
  DomoticSystem();

  // Handles user input commands
  std::string handleInput(const std::string& input_);

  // Retrieve custom input from
  std::string getCustomInput();

  // Gets the current system state
  bool getSystemState() const;

  // Logs a message to the provided log file
  void log(const std::string& message_, std::ofstream& logFile);

 private:
  Scheduler scheduler_;
  std::vector<Manual> manual_;
  std::vector<PrefixCycle> prefixCycle_;

  static constexpr double kMaxPower = 3.5;
  bool status_ = false;

  // Set the system state
  void setSystemState(bool state);

  // Builds the initial list of devices (manual and prefix_cycle)
  void buildDevices();

  // Searches for a Manual device by its name
  Manual* searchManual(const std::string& name_);

  // Searches for a PrefixCycle device by its name
  PrefixCycle* searchPrefixCycle(const std::string& name_);

  // Round double at 2 decimal digits, return as a string
  std::string roundTo2(double val);

  // Logs the current time as a string
  std::string logTime();

  // Logs an error message
  std::string logError(const std::string& error);

  // Device management functions
  std::string setOn(Device& device_);
  std::string setOff(Device& device_);
  std::string setDeviceTimer(Device& device_);
  std::string removeFromRunning(Device& device_);
  std::string removeFromWaiting(Device& device_);
  std::string showDevice(Device& device_);
  std::string showAll();

  // Time management functions
  std::string setTime(Clock& time_);
  std::string resetTime();
  std::string resetTimers();
  std::string resetAll();
};

#endif  // DOMOTICSYSTEM_H
