// FEDERICO COMPAGNO

#include "../include/Manual.h"

/**
 * @brief Constructs a Manual object with specified parameters.
 * @param id The unique identifier for the manual device.
 * @param name The name of the device.
 * @param power The power consumption of the device in kWh.
 * @param start The start time for the device.
 * @param finish The finish time for the device.
 */
Manual::Manual(const std::string& id, const std::string& name, double power, Clock start, Clock finish)
    : Device(id, name, power, start, finish) 
{
  // Left intentionally blank
}