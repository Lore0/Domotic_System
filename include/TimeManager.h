#ifndef DOMOTICSYSTEM_H
#define DOMOTICSYSTEM_H
#include <vector>
#include "Device.h"

class TimeManager {
  public:
    // implement methods
  private:
    std::vector<Device> running_;
    std::vector<Device> waiting_;
    std::vector<Device> finished_;
};

#endif // DOMOTICSYSTEM_H
