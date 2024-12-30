#ifndef DEVICE_H
#define DEVICE_H
#include <string>
#include "Time.h"

class Device {
  protected:
    std::string name_;
    std::string id_;
    double power_;
    bool status_; //on = true , off = false
    Time start_;
    Time finish_;

  public:
    //costruttore
    Device(const std::string& id, const std::string& name, double power, Time start, Time finish);
    
    //get
    bool getStatus() const;

    const std::string& getName() const;

    const std::string& getId() const;

    Time getStart();

    Time getFinish();

    double getPowerConsumption() const;

   

    //accendo il device
    void turnOn();

    //spengo il device
    void turnOff();

    Device(const Device&) = delete;

    Device& operator=(const Device&) = delete;

    virtual ~Device() = default;
};

bool operator==(Device d1, Device d2);

#endif
