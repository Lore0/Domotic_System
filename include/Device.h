#ifndef DEVICE_H
#define DEVICE_H
#include <string>
#include "Clock.h"

class Device {
  protected:

    std::string name_;

    std::string id_;

    double power_;

    bool status_; //on = true , off = false

    bool hasTimer_;

    Clock start_;

    Clock finish_;

  public:

    Device(const std::string& id, const std::string& name, double power, Clock start, Clock finish);

    bool getStatus() const;

    bool hasTimer() const;

    void setHasTimer(bool status);

    const std::string& getName() const;

    const std::string& getId() const;

    Clock getStart() const;

    Clock getFinish() const;

    double getPowerConsumption() const;

    virtual void setStart(Clock start);

    virtual void setFinish(Clock finish);

    void turnOn();

    void turnOff();

    //Device(const Device&) = delete;

    //Device& operator=(const Device&) = delete;

    virtual ~Device() = default;
};

bool operator==(const Device& d1, const Device& d2);

#endif // DEVICE_H
