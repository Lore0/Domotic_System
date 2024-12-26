#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include "Device.h"
#include <vector>

class TimeManager {
  public:
    // implement methods
  private:
    std::vector<Device> running_;
    std::vector<Device> waiting_;
    std::vector<Device> finished_;
};

#endif // DOMOTICSYSTEM_H
