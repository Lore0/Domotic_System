// FEDERICO COMPAGNO

#ifndef MANUAL_H
#define MANUAL_H

#include "Device.h"

// Manual class derived from Device class.
class Manual : public Device {
 private:
  // No additional members

 public:
  // Constructor to initialize the Manual device with id, name, power, start time and finish time.
  Manual(const std::string& id, const std::string& name, double power, Clock start, Clock finish);
};

#endif  // MANUAL_H