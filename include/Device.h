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

    bool hasTimer_;

    Time start_;

    Time finish_;

  public:

    Device(const std::string& id, const std::string& name, double power, Time start, Time finish);

    bool getStatus() const;

    bool hasTimer() const;

    void setHasTimer(bool status);

    const std::string& getName() const;

    const std::string& getId() const;

    Time getStart() const;

    Time getFinish() const;

    double getPowerConsumption() const;

    virtual void setStart(Time start);

    virtual void setFinish(Time finish);

    void turnOn();

    void turnOff();

    //Device(const Device&) = delete;

    //Device& operator=(const Device&) = delete;

    virtual ~Device() = default;
};

bool operator==(const Device& d1, const Device& d2);

#endif // DEVICE_H
