#ifndef PREFIXCYCLE_H
#define PREFIXCYCLE_H

#include "Device.h"

class PrefixCycle : public Device{
    private:
        Time cycleDuration_;
    public:
        PrefixCycle(const std::string& name, const std::string& id, double power, Time cycleDuration, Time start, Time finish);
}

#endif
