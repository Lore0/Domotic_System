//COMPAGNO FEDERICO

#include "../include/PrefixCycle.h"

/**
 * @brief Constructs a PrefixCycle object with specified parameters.
 * @param id The unique identifier for the device.
 * @param name The name of the device.
 * @param power The power consumption of the device in kiloWatts.
 * @param start The start time of the device's operational cycle.
 * @param cycleDuration The duration of the device's operational cycle.
 */
PrefixCycle::PrefixCycle(const std::string& id, const std::string& name, double power, Clock start, Clock cycleDuration)
    : Device(id, name, power, start, start),
      cycleDuration_(cycleDuration)
{
  // Left intentionally blank
}

/**
 * @brief Sets the start time for the device's operational cycle.
 * This method overrides the setStart method in Device.
 * @param start The new start time for the device's cycle.
 */
void PrefixCycle::setStart(Clock start) {
  start_ = start;
  finish_ = start.addTime(getDuration());
}

/**
 * @brief Retrieves the duration of the device's operational cycle.
 * @return A Clock object representing the cycle duration.
 */
Clock PrefixCycle::getDuration() const { 
    return cycleDuration_; 
}