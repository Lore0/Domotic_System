#include "../include/Scheduler.h"

using namespace std;

Scheduler::Scheduler(double maxPower)
    : MAX_POWER(maxPower), inUsePower(0), currTime("00:00") {
  // nothing
}

/**
 * @brief Gets the current time as a string.
 * @return A string containing the current time in the format [HH:MM].
 */
string Scheduler::getStringCurrTime() {
  if (currTime == Clock("24:00"))  // we avoid displaying "24:00" and instead
                                   // use "00:00" for better readability
    currTime = Clock("00:00");
  return "[" + currTime.to_string() + "]";
}

/**
 * @brief Retrieves the current time.
 * @return The current time as a Clock object.
 */
Clock Scheduler::getCurrTime() { return currTime; }

/**
 * @brief Attempts to turn on a device and logs the event.
 * @param device Pointer to the Device object to be turned on.
 * @return A string containing the log of the operation, including the current
 * time, device name, and status of the operation.
 */
string Scheduler::setOn(Device *device) {
  string log = "";
  int targetIndex = searchDevice(waiting_, device);
  int i = 0;
  // set-on condition: the power in use with the new device must not exceed the
  // maximum power.
  if (inUsePower + device->getPowerConsumption() + MAX_POWER >= 0) {
    // search for the correct position to insert the new device
    while (i < running_.size() &&
           !(device->getFinish() < running_.at(i)->getFinish()))
      i++;
    device->turnOn();
    Clock temp = currTime;
    inUsePower += device->getPowerConsumption();
    running_.insert(running_.begin() + i, device);
    log = getStringCurrTime() + " Il dispositivo " + device->getName() +
          " si è acceso" +
          "\n";  //[13:00] Il dispositivo ‘${DEVICENAME}’ si è acceso
  } else {       // if the power is insufficient, return an error message
    log = getStringCurrTime() +
          " ERRORE: non c'è corrente sufficiente per accendere-> " +
          device->getName() + "\n";
  }
  // In any case, remove the device from the waiting list.
  if (targetIndex != -1) {
    waiting_.erase(waiting_.begin() + targetIndex);
    device->setHasTimer(false);
  }
  return log;
}

/**
 * @brief Turns off a device and logs the event.
 * @param device Pointer to the Device object to be turned off.
 * @return A string containing the log of the operation, including the current
 * time, device name, and status of the operation.
 */
string Scheduler::setOff(Device *device) {
  int targetIndex = searchDevice(running_, device);
  // remove the device from the running list and set the flags
  running_.erase(running_.begin() + targetIndex);
  device->turnOff();
  device->setHasTimer(false);
  device->setFinish(currTime);
  inUsePower -= device->getPowerConsumption();
  string log = getStringCurrTime() + " Il dispositivo " + device->getName() +
               " si è spento" + "\n";
  // create a base copy of the device and insert it into the finished list
  Device temp{device->getId(), device->getName(), device->getPowerConsumption(),
              device->getStart(), device->getFinish()};
  finished_.push_back(temp);
  // if a device that was producing power in the system is removed, check that
  // the power in use does not exceed the maximum limit and take appropriate
  // action accordingly
  if (inUsePower + MAX_POWER < 0) {
    int indexMaxStartTime = 0;
    for (int i = 0; i < running_.size(); i++) {
      if (running_.at(i)->getStart() >
          running_.at(indexMaxStartTime)->getStart())
        indexMaxStartTime = i;
    }
    log += setOff(running_.at(indexMaxStartTime));
  }
  return log;
}

/**
 * @brief Sets a timer for a device and logs the event.
 * @param device Pointer to the Device object to set a timer for.
 * @return A string containing the log of the operation, including the current
 * time, device name, and timer details (start and finish times).
 */
string Scheduler::setDeviceTimer(Device *device) {
  int i = 0;
  device->setHasTimer(true);
  // search for the correct position to insert the new device
  while (i < waiting_.size() &&
         !(device->getStart() < waiting_.at(i)->getStart()))
    i++;
  waiting_.insert(waiting_.begin() + i, device);
  return getStringCurrTime() + " Impostato un timer per il dispositivo " +
         device->getName() + " dalle " + device->getStart().to_string() +
         " alle " + device->getFinish().to_string() + "\n";
}

/**
 * @brief Removes the timer from a device in the waiting list and logs the
 * event.
 * @param device Pointer to the Device object from which the timer is to be
 * removed.
 * @return A string containing the log of the operation, including the current
 * time, device name, and status of the operation.
 */
string Scheduler::removeTimerFromWaiting(Device *device) {
  int targetIndex = searchDevice(waiting_, device);
  device->setHasTimer(false);
  if (targetIndex == -1)
    return getStringCurrTime() + " Errore: dispositivo non in attesa" + "\n";
  waiting_.erase(waiting_.begin() + targetIndex);
  return getStringCurrTime() + " Rimosso il timer dal dispositivo " +
         device->getName() + "\n";
}

/**
 * @brief Removes the timer from a device in the running list and logs the
 * event. The device stays on until the end of the day.
 * @param device Pointer to the Device object from which the timer is to be
 * removed.
 * @return A string containing the log of the operation, including the current
 * time, device name, and status of the operation.
 */
string Scheduler::removeTimerFromRunning(Device *device) {
  int targetIndex = searchDevice(running_, device);
  if (targetIndex == -1)  // if the device is not present, return an error
    return getStringCurrTime() + " Errore: dispositivo non in funzione" + "\n";
  // stays on until the end of the day
  device->setFinish(Clock("24:00"));
  device->setHasTimer(false);
  running_.erase(running_.begin() + targetIndex);
  running_.push_back(device);
  return getStringCurrTime() + " Rimosso il timer dal dispositivo " +
         device->getName() + "\n";
}

/**
 * @brief Sets the current time and logs the events related to devices
 * operation.
 * @param time Reference to the Clock object representing the new current time.
 * @return A string containing the log of the operations, including the current
 * time, device names, and status of each operation.
 */
string Scheduler::setTime(Clock &time) {
  if (currTime > time)  // invalid hour
    return getStringCurrTime() + " Errore: ora minore a quella attuale" + "\n";
  // check if the lists are empty
  // check which one is smaller
  // if they are equal, remove from running first
  // continue until both are less than time or both lists are empty
  string log = "";
  while (!(currTime > time)) {
    // End check (both lists empty)
    if ((running_.empty() && waiting_.empty())) {
      currTime = time;
      break;
    }  // if one of the two is empty
    else if (running_.empty() || waiting_.empty()) {
      if (running_.empty())  // se running è vuota
      {
        // if the first in waiting is greater than time
        if (waiting_.at(0)->getStart() > time) {
          currTime = time;
          break;
        } else  // waiting is not empty and a device needs to be started
        {
          currTime = waiting_.at(0)->getStart();
          log += setOn(waiting_.at(0));
        }
      } else  // waiting empty
      {
        // if the first in running is greater than time
        if (running_.at(0)->getFinish() > time) {
          currTime = time;
          break;
        } else  // running is not empty and a device needs to be started
        {
          currTime = running_.at(0)->getFinish();
          log += setOff(running_.at(0));
        }
      }
    } else  // both running and waiting are not empty
    {
      // waiting < running
      if (waiting_.at(0)->getStart() < running_.at(0)->getFinish()) {
        currTime = waiting_.at(0)->getStart();
        log += setOn(waiting_.at(0));
      } else  // waiting >= running
      {
        currTime = running_.at(0)->getFinish();
        log += setOff(running_.at(0));
      }
    }
  }
  return log + getStringCurrTime() + " L'orario attuale è " +
         getCurrTime().to_string() + "\n";
}

/**
 * @brief Resets the current time to 00:00 and updates the state of all devices.
 * @return A string containing the log of the operation, including the current
 * time and the status of the devices.
 */
string Scheduler::resetTime() {
  currTime.setClock("00:00");
  // if the timer has not been fully consumed, restore it
  for (Device *d : running_) {
    if (d->hasTimer())
      setDeviceTimer(d);
    else {
      d->setHasTimer(false);
      d->turnOff();
    }
  }
  running_.clear();
  finished_.clear();
  inUsePower = 0;
  return getStringCurrTime() + " L'orario attuale è 00:00" + "\n";
}

/**
 * @brief Resets all timers for devices in the waiting and running lists, and
 * logs the events.
 * @return A string containing the log of the operations, including the current
 * time, device names, and status of each operation.
 */
string Scheduler::resetTimers() {
  string log = "";
  for (auto &device : waiting_) {
    log += removeTimerFromWaiting(device);
  }
  for (auto &device : running_) {
    log += removeTimerFromRunning(device);
  }
  return log;
}

/** @brief Calculates the total energy consumption of a device.
 * @param device Pointer to the Device object whose energy consumption is to be
 * calculated.
 * @return A double that rapresent the total energy consumed by the device in
 * kWh.
 */
double Scheduler::showDevice(Device *device) {
  double tothour = 0;
  int targetIndex = searchDevice(running_, device);
  // if the device is in running, calculate the power it has consumed so far
  if (targetIndex != -1) {
    tothour += currTime.getDistance(running_.at(targetIndex)->getStart());
  }
  int i = 0;
  // sum the power consumptions
  while (i < finished_.size()) {
    if (device->getId() == finished_.at(i).getId())
      tothour +=
          finished_.at(i).getFinish().getDistance(finished_.at(i).getStart());
    i++;
  }
  return tothour * device->getPowerConsumption();
}

/** @brief Searches for a device in a list and returns its index.
 * @param list The vector of Device pointers to search through.
 * @param target Pointer to the target Device to search for.
 * @return The index of the target device in the list, or -1 if not found.
 */
int searchDevice(vector<Device *> list, Device *target) {
  for (int i = 0; i < list.size(); i++) {
    if (list.at(i)->getId() == target->getId()) return i;
  }
  return -1;
}
