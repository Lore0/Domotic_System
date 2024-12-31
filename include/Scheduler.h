#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Device.h"
#include <vector>

class Scheduler {
  public:
    Scheduler(int maxPower);
    void setOn(Device& device_);
    void setOff(Device& device_);
    void setDeviceTimer(Device& device_);
    //void removeFromRunning(Device& device_);  
    void removeFromWaiting(Device& device_);
    //show
    //showdevice;
    void setTime(Time& time_);
    void resetTime();
    void resetTimers();
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
