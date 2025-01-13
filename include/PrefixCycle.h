//COMPAGNO FEDERICO

#ifndef PREFIXCYCLE_H
#define PREFIXCYCLE_H

#include "Device.h"

// PrefixCycle class derived from Device class
class PrefixCycle : public Device {
 private:
  Clock cycleDuration_;  // Duration of the cycle

 public:
  // Constructor to initialize the PrefixCycle device with id, name, power, start time, and cycle duration
  PrefixCycle(const std::string& id, const std::string& name, double power, Clock start, Clock cycleDuration);

  // Sets the start time of the device (overrides the base class method)
  void setStart(Clock start) override;

  // Returns the duration of the cycle
  Clock getDuration() const;
};

#endif  // PREFIXCYCLE_H