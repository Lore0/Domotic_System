// CHELLIN RICCARDO

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <vector>

#include "Device.h"

//Scheduler: class that handle waiting and running lists
class Scheduler {
 public:
  Scheduler(double maxPower);

  //Return the current time of the system
  Clock getCurrTime();

  //Return the current time with the appropriate format
  std::string getStringCurrTime();

  //set a specified device on
  std::string setOn(Device* device_);  

  //set a specified device off
  std::string setOff(Device* device_);

  //set a timer for a specified device
  std::string setDeviceTimer(Device* device_);

  //remove a timer from a running specified device
  std::string removeTimerFromRunning(Device* device_);

  //remove a timer from a waiting specified device
  std::string removeTimerFromWaiting(Device* device_);

  //return the total power consumed by a specified device
  double showDevice(Device* device_);

  //Set the time and execute the necessary command to reach the specified time 
  std::string setTime(Clock& time_);

  //Reset the system time to 00:00 and restore all devices to their initial conditions
  std::string resetTime();

  //Remove the timers from all devices
  std::string resetTimers();

 private:
  const double MAX_POWER;
  double inUsePower;
  Clock currTime;

  //List of devices currently waiting to be set on, sorted from the one that will turn on first to the one that will turn on last
  std::vector<Device*> waiting_;

  //List of devices currently on, sorted from the one that will turn off first to the one that will turn off last
  std::vector<Device*> running_;

  //List of devices that have completed an execution, necessary for tracking energy consumption
  //Unlike the waiting and running lists, a device can appear multiple times in this list
  std::vector<Device> finished_;
};

//Returns the index of a specified device in a specified list
int searchDevice(std::vector<Device*> list, Device* target);

#endif  // SCHEDULER_H
