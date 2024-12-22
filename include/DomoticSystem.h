#ifndef DOMOTICSYSTEM_H
#define DOMOTICSYSTEM_H

class DomoticSystem {
  public:
    DomoticSystem();
    void handleInput(std::string input_);
    vector<Device> devices_;
  private:
    TimeManaget timeManager_;
    void buildDevices();
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
}

#endif // DOMOTICSYSTEM_H
