#include "../include/PrefixCycle.h"

PrefixCycle::PrefixCycle(const std::string& id, const std::string& name, double power, Time cycleDuration, Time start)
    :   Device(id, name, power, start, start.addTime(cycleDuration)), cycleDuration_(cycleDuration)
{
    //left intentionally blank
}





