// FEDERICO COMPAGNO

#ifndef DEVICE_H
#define DEVICE_H
#include <iostream>
#include <string>

#include "Clock.h"

// Device class to represent an electronic device with a timer.
class Device {
 protected:
  std::string name_;  // Name of the device.
  std::string id_;    // Unique identifier for the device.
  double power_;      // Power consumption of the device in kWh.
  bool status_;       // Status of the device (on = true, off = false).
  bool hasTimer_;     // Indicates if the device has a timer.
  Clock start_;       // Start time of the device.
  Clock finish_;      // Finish time of the device.

 public:
  // Constructor to initialize the device with id, name, power, start time, and finish time.
  Device(const std::string& id, const std::string& name, double power,
         Clock start, Clock finish);

  // Returns the status of the device.
  bool getStatus() const;

  // Returns whether the device has a timer.
  bool hasTimer() const;

  // Returns the name of the device.
  const std::string& getName() const;

  // Returns the unique identifier of the device.
  const std::string& getId() const;

  // Returns the start time of the device.
  Clock getStart() const;

  // Returns the finish time of the device.
  Clock getFinish() const;

  // Returns the power consumption of the device.
  double getPowerConsumption() const;

  // Sets whether the device has a timer.
  void setHasTimer(bool status);

  // Sets the start time of the device (can be overridden by derived classes).
  virtual void setStart(Clock start);

  // Sets the finish time of the device (can be overridden by derived classes).
  virtual void setFinish(Clock finish);

  // Turns the device on.
  void turnOn();

  // Turns the device off.
  void turnOff();

  // Destructor (default implementation) for dynamic_cast.
  virtual ~Device() = default;
};

// Compares if two Device objects are equal.
bool operator==(const Device& d1, const Device& d2);

#endif  // DEVICE_H