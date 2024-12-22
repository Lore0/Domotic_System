#ifndef MANUAL_H
#define MANUAL_H

#include "Device.h"

class Manual : public Device{
    private:
    public:
        Manual(const std::string& name, const std::string& id, double power, Time start, Time finish);
}

#endif