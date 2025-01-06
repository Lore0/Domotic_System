#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Device.h"
#include <vector>

class Scheduler {
  public:
    Scheduler(int maxPower);
    Time getCurrTime();
    std::string getStringCurrTime();
    std::string setOn(Device* device_); //così
    std::string setOff(Device* device_);
    std::string setDeviceTimer(Device* device_);
    std::string removeTimerFromRunning(Device& device_);
    std::string removeTimerFromWaiting(Device& device_);
    std::string showDevice(Device& device_);
    std::string showAll();
    std::string setTime(Time& time_);
    std::string resetTime();
    std::string resetTimers();
    //void resetAll(); //fa pane con reset
  private:
    const int MAX_POWER;
    int inUsePower;
    Time currTime;
    std::vector<Device*> running_;
    std::vector<Device*> waiting_;
    std::vector<Device*> finished_;
};
int searchDevice(std::vector<Device> &list, Device &target);

#endif // SCHEDULER_H
