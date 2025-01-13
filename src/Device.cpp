//COMPAGNO FEDERICO

#include "../include/Device.h"

using namespace std;

/**
 * @brief Constructs a Device object with specified parameters.
 * @param id The unique identifier for the device.
 * @param name The name of the device.
 * @param power The power consumption of the device in kiloWatts.
 * @param start The start time of the device's timer.
 * @param finish The finish time of the device's timer.
 */
Device::Device(const string& id, const string& name, double power, Clock start, Clock finish)
    : id_(id),
      name_(name),
      power_(power),
      status_(false),
      hasTimer_(false),
      start_(start),
      finish_(finish) 
{
  // Left intentionally blank
}

/**
 * @brief Returns the unique identifier of the device.
 * @return A const reference to the device's ID.
 */
const string& Device::getId() const { 
    return id_; 
}

/**
 * @brief Returns the name of the device.
 * @return A const reference to the device's name.
 */
const string& Device::getName() const { 
    return name_; 
}

/**
 * @brief Returns the start time of the device's timer.
 * @return A Clock object representing the start time.
 */
Clock Device::getStart() const { 
    return start_; 
}

/**
 * @brief Returns the finish time of the device's timer.
 * @return A Clock object representing the finish time.
 */
Clock Device::getFinish() const { 
    return finish_; 
}

/**
 * @brief Returns the power consumption of the device.
 * @return The power consumption in watts.
 */
double Device::getPowerConsumption() const { 
    return power_; 
}

/**
 * @brief Checks whether the device is currently on.
 * @return True if the device is on, false otherwise.
 */
bool Device::getStatus() const { 
    return status_; 
}

/**
 * @brief Checks whether the device has a timer enabled.
 * @return True if the timer is enabled, false otherwise.
 */
bool Device::hasTimer() const { 
    return hasTimer_; 
}

/**
 * @brief Sets the timer functionality of the device.
 * @param status True to enable the timer, false to disable it.
 */
void Device::setHasTimer(bool status) { 
    hasTimer_ = status; 
}

/**
 * @brief Sets the start time for the device's timer.
 * @param start The start time to be set.
 */
void Device::setStart(Clock start) { 
    start_ = start; 
}

/**
 * @brief Sets the finish time for the device's timer.
 * @param finish The finish time to be set.
 */
void Device::setFinish(Clock finish) { 
    finish_ = finish; 
}

/**
 * @brief Turns on the device. If the device is already on, a message is printed.
 */
void Device::turnOn() {
  if (status_ == true) {
    cout << "Il dispositivo è già ACCESO. \n";
    return;
  }
  status_ = true;
}

/**
 * @brief Turns off the device. If the device is already off, a message is printed.
 */
void Device::turnOff() {
  if (status_ == false) {
    cout << "Il dispositivo è già SPENTO. \n";
    return;
  }
  status_ = false;
}

/**
 * @brief Compares two devices for equality based on their IDs.
 * @param d1 The first device to compare.
 * @param d2 The second device to compare.
 * @return True if the devices have the same ID, false otherwise.
 */
bool operator==(const Device& d1, const Device& d2) {
  if (d1.getId().compare(d2.getId()) != 0) {
    return false;
  } else {
    return true;
  }
}