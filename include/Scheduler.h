#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Device.h"
#include <vector>

class Scheduler {
  public:
    Scheduler(int maxPower);
    std::string getCurrTime();
    std::string setOn(Device& device_);
    std::string setOff(Device& device_);
    std::string setDeviceTimer(Device& device_);
    std::string removeTimerFromRunning(Device& device_);  
    std::string removeTimerFromWaiting(Device& device_);
    //show
    //showdevice;
    std::string setTime(Time& time_);
    std::string resetTime();
    std::string resetTimers();
    //void resetAll(); //fa pane con reset
  private:
    const int MAX_POWER;
    int inUsePower;
    Time currTime;
    std::vector<Device> running_;
    std::vector<Device> waiting_;
    std::vector<Device> finished_;
};

#endif // SCHEDULER_H
