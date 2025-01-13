#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Device.h"
#include <vector>
#include <iostream>

class Scheduler {
  public:
    Scheduler(double maxPower);
    Clock getCurrTime();
    std::string getStringCurrTime();
    std::string setOn(Device* device_); //così
    std::string setOff(Device* device_);
    std::string setDeviceTimer(Device* device_);
    std::string removeTimerFromRunning(Device* device_);
    std::string removeTimerFromWaiting(Device* device_);
    double showDevice(Device* device_);
    std::string showAll();
    std::string setTime(Clock& time_);
    std::string resetTime();
    std::string resetTimers();
    //void resetAll(); //fa pane con reset
  private:
    const double MAX_POWER;
    double inUsePower;
    Clock currTime;
    std::vector<Device*> running_;
    std::vector<Device*> waiting_;
    std::vector<Device> finished_;
};
int searchDevice(std::vector<Device*> list, Device* target);

#endif // SCHEDULER_H
