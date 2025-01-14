// LORENZO PANIZZOLO

#include "../include/DomoticSystem.h"

using namespace std;

/**
 * @brief Constructor that initializes scheduler, system status, and
 * builds the devices.
 */
DomoticSystem::DomoticSystem() : scheduler_(kMaxPower), status_(true) {
  buildDevices();
}

/**
 * @brief Calls Scheduler function to turn on a device and logs the action.
 * @param device_ Reference to the device to be turned on.
 * @return A string containing the action log.
 */
string DomoticSystem::setOn(Device &device_) {
  return logTime() + scheduler_.setOn(&device_);
}

/**
 * @brief Calls Scheduler function to turn off a device and logs the action.
 * @param device_ Reference to the device to be turned off.
 * @return A string containing the action log.
 */
string DomoticSystem::setOff(Device &device_) {
  return logTime() + scheduler_.setOff(&device_);
}

/**
 * @brief Call Scheduler function to set a timer for a device and logs the
 * action.
 * @param device_ Reference to the device.
 * @return A string containing the action log.
 */
string DomoticSystem::setDeviceTimer(Device &device_) {
  return logTime() + scheduler_.setDeviceTimer(&device_);
}

/**
 * @brief Call Scheduler function to remove a device from the running list and
 * logs the action.
 * @param device_ Reference to the device.
 * @return A string containing the action log.
 */
string DomoticSystem::removeFromRunning(Device &device_) {
  return logTime() + scheduler_.removeTimerFromRunning(&device_);
}

/**
 * @brief Call Scheduler function to remove a device from the waiting list and
 * logs the action.
 * @param device_ Reference to the device.
 * @return A string containing the action log.
 */
string DomoticSystem::removeFromWaiting(Device &device_) {
  return logTime() + scheduler_.removeTimerFromWaiting(&device_);
}

/**
 * @brief Displays the current energy consumption or production of a device.
 * @param device_ Reference to the Device object whose energy data is to be
 * displayed.
 * @return A string containing the log time, current time, device name, and the
 * amount of energy consumed or produced by the device.
 */
string DomoticSystem::showDevice(Device &device_) {
  string time = logTime();
  Device *device = &device_;
  double tot = scheduler_.showDevice(device);
  if (device->getPowerConsumption() < 0) {
    tot *= (-1);
    return time + scheduler_.getStringCurrTime() + " Il dispositivo " +
           device->getName() + " ha attualmente consumato " + roundTo2(tot) +
           " kWh\n";
  } else {
    return time + scheduler_.getStringCurrTime() + " Il dispositivo " +
           device->getName() + " ha attualmente prodotto " + roundTo2(tot) +
           " kWh\n";
  }
}

/**
 * @brief Generates a summary of the energy consumption and production of all
 * devices in the system.
 * @return A string containing the current time, total energy produced or
 * consumed by the system, and detailed information about each device's energy
 * usage.
 */
string DomoticSystem::showAll() {
  string time = logTime();
  double totConsumed = 0;
  double totProduced = 0;
  string eachDevice = "";
  for (Device d : manual_) {
    double consumpt = scheduler_.showDevice(&d);
    if (d.getPowerConsumption() < 0) {
      consumpt *= (-1);
      totConsumed += consumpt;
      eachDevice += "\n  - Il dispositivo " + d.getName() + " ha consumato " +
                    roundTo2(consumpt) + " kWh";
    } else {
      totProduced += consumpt;
      eachDevice += "\n  - Il dispositivo " + d.getName() + " ha prodotto " +
                    roundTo2(consumpt) + " kWh";
    }
  }
  for (Device d : prefixCycle_) {
    double consumpt = scheduler_.showDevice(&d);
    if (d.getPowerConsumption() < 0) {
      consumpt *= (-1);
      totConsumed += consumpt;
      eachDevice += "\n  - Il dispositivo " + d.getName() + " ha consumato " +
                    roundTo2(consumpt) + " kWh";
    } else {
      totProduced += consumpt;
      eachDevice += "\n  - Il dispositivo " + d.getName() + " ha prodotto " +
                    roundTo2(consumpt) + " kWh";
    }
  }
  return time + scheduler_.getStringCurrTime() +
         " Attualmente il sistema ha prodotto " + roundTo2(totProduced) +
         " kWh e ha consumato " + roundTo2(totConsumed) +
         " kWh. Nello specifico:" + eachDevice + "\n";
}

/**
 * @brief Call Scheduler function to set a new time in the system.
 * @param time_ The new time to set.
 * @return A string containing the action log.
 */
string DomoticSystem::setTime(Clock &time_) {
  string time = logTime();
  return time + scheduler_.setTime(time_);
}

/**
 * @brief Call Scheduler function to resets the time to its initial state.
 * @return A string containing the action log.
 */
string DomoticSystem::resetTime() { return logTime() + scheduler_.resetTime(); }

/**
 * @brief Call Scheduler function to reset all device timers in the system.
 * @return A string containing the action log.
 */
string DomoticSystem::resetTimers() {
  return logTime() + scheduler_.resetTimers();
}

/**
 * @brief Resets all system parameters including device timers and time.
 * @return A string containing the action log.
 */
string DomoticSystem::resetAll() {
  resetTimers();
  resetTime();
  string time = logTime();
  string log = time + scheduler_.getStringCurrTime() +
               " Comando reset all eseguito con successo\n";
  return log;
}

/**
 * @brief Logs the current time: [HH:MM] L'orario attuale è HH:MM.
 * @return A string containing the current time.
 */
string DomoticSystem::logTime() {
  return scheduler_.getStringCurrTime() + " L'orario attuale è " +
         scheduler_.getCurrTime().to_string() + "\n";
}

/**
 * @brief Rounds a double value to 2 decimal places and returns it as a string.
 * @param value The double value to be rounded and converted.
 * @return A string representation of the value rounded to 2 decimal places.
 */
string DomoticSystem::roundTo2(double value) {
  ostringstream oss;
  oss << fixed << setprecision(2) << value;
  return oss.str();
}

/**
 * @brief Logs a message to the specified log file.
 * @param message The message to be logged.
 * @param logFile The output file stream to write the log message to.
 */
void DomoticSystem::log(const string &message, ofstream &logFile) {
  if (logFile.is_open()) {
    logFile << message << endl;
  }
}

/**
 * @brief Reads device information from device.txt file and builds the device
 * objects.
 */
void DomoticSystem::buildDevices() {
  ifstream file("../docs/device.txt");
  if (!file.is_open()) {
    cout << "ERRORE: impossibile aprire il file device.txt" << endl;
    return;
  }

  string line;
  while (getline(file, line)) {
    stringstream ss(line);
    string id, name, type, pow;

    // read data
    getline(ss, id, ',');
    getline(ss, name, ',');
    getline(ss, type, ',');
    getline(ss, pow, ',');

    double power = stod(pow);

    Clock null("00:00");

    // M or CP handling
    if (type == "Manuale") {
      manual_.push_back(Manual(id, name, power, null, null));
    } else {
      Clock cycleDuration(type);
      prefixCycle_.push_back(PrefixCycle(id, name, power, null, cycleDuration));
    }
  }

  file.close();
}

/**
 * @brief Searches for a Manual object by its name.
 * @param name The name of the Manual object to search for.
 * @return Manual* pointer to the Manual object if found, otherwise nullptr.
 */
Manual *DomoticSystem::searchManual(const string &name) {
  for (auto &m : manual_) {
    if (m.getName() == name) return &m;
  }
  return nullptr;
}

/**
 * @brief Searches for a PrefixCycle object by its name.
 * @param name The name of the PrefixCycle object to search for.
 * @return PrefixCycle* pointer to the PrefixCycle object if found, otherwise
 * nullptr.
 */
PrefixCycle *DomoticSystem::searchPrefixCycle(const string &name) {
  for (auto &p : prefixCycle_) {
    if (p.getName() == name) return &p;
  }
  return nullptr;
}

/**
 * @brief Logs an error message.
 * @param error The error message to log.
 * @return A string containing the logged error message.
 */
string DomoticSystem::logError(const string &error) {
  string logM = scheduler_.getStringCurrTime() + " ERRORE : " + error + "\n";
  cout << logM << endl;
  return logM;
}

/**
 * @brief Handles input commands and performs corresponding actions.
 * @param input_ The input command string.
 * @return A string containing the result of the command execution.
 */
string DomoticSystem::handleInput(const string &input_) {
  // stream to parse input
  istringstream iss(input_);
  string command, deviceName;
  iss >> command;
  string log;

  try {
    if (command == "set") {  // handle set command
      iss >> deviceName;
      if (deviceName == "time") {
        string time;
        iss >> time;
        Clock t(time);
        // new time in the past (end program)
        if (t < scheduler_.getCurrTime()) {
          Clock end("24:00");
          log = setTime(end);  // set time to end of the day
          log += "[00:00] FINE";
          cout << log << endl;
          setSystemState(false);  // deactivate system
          return log;
        }
        log = setTime(t);  // set the new time
        cout << log << endl;
        return log;
      } else {  // handle "set [$deviceName] [on|off|timer]"
        string action;
        iss >> action;

        if (Manual *m = searchManual(deviceName)) {  // search for manual device
          if (action == "on") {
            if (m->getStatus()) {
              return logError(m->getName() + " è già acceso");
            } else {
              m->setStart(scheduler_.getCurrTime());
              m->setFinish(Clock("24:00"));
              log = setOn(*m);
              cout << log << endl;
              return log;
            }
          } else if (action == "off") {
            if (!m->getStatus()) {
              return logError(m->getName() + " è già spento");
            } else {
              log = setOff(*m);
              cout << log << endl;
              return log;
            }
          } else {  // handle timer for manual device
            string startTime = action, endTime;
            iss >> endTime;
            if (m->hasTimer()) {
              return logError(m->getName() + " ha già un timer");
            }
            if (m->getStatus()) {
              return logError(m->getName() + " è già acceso");
            }
            if (Clock(startTime) < scheduler_.getCurrTime()) {
              return logError("Orario non valido");
            }
            if (!endTime.empty()) {
              if (Clock(startTime) > Clock(endTime)) {
                return logError("Orario non valido");
              }
              m->setFinish(Clock(endTime));
            } else {
              // set finish at the end of the day
              // if no end time is provided
              m->setFinish(Clock("24:00"));
            }
            m->setStart(Clock(startTime));
            log = setDeviceTimer(*m);
            cout << log << endl;
            return log;
          }
        } else if (PrefixCycle *p = searchPrefixCycle(deviceName)) {
          // search for prefix cycle device
          if (action == "on") {
            if (p->getStatus()) {
              return logError(p->getName() + " è già acceso");
            } else {
              p->setStart(scheduler_.getCurrTime());
              // calculate finish time
              p->setFinish(scheduler_.getCurrTime().addTime(p->getDuration()));
              log = setOn(*p);
              cout << log << endl;
              return log;
            }
          } else if (action == "off") {
            if (!p->getStatus()) {
              return logError(p->getName() + " è già spento");
            } else {
              setOff(*p);
              cout << log << endl;
              return log;
            }
          } else {  // handle timer for prefix cycle device
            string startTime = action;
            string endTime;

            if (p->hasTimer()) {
              return logError(p->getName() + " ha già un timer");
            }

            if (Clock(startTime) < scheduler_.getCurrTime()) {
              return logError("Orario non valido");
            }

            if (iss >> endTime) {  // CP device supports only start time
              return logError(
                  "Dispositivo CP non supporta orario di fine. Devi "
                  "specificare "
                  "solo l'orario di inizio");
            }

            if (p->getStatus()) {
              return logError(p->getName() + " è già acceso");
            }

            Clock start(startTime);
            p->setStart(start);
            p->setFinish(start.addTime(p->getDuration()));
            log = setDeviceTimer(*p);
            cout << log << endl;
            return log;
          }
        } else {
          return logError("Dispositivo non trovato");
        }
      }
    } else if (command == "reset") {  // handle reset command
      string target;
      iss >> target;

      if (target == "time") {
        log = resetTime();
        cout << log << endl;
        return log;
      } else if (target == "timers") {
        log = resetTimers();
        cout << log << endl;
        return log;
      } else if (target == "all") {
        log = resetAll();
        cout << log << endl;
        return log;
      } else {
        return logError("Comando non riconosciuto");
      }
    } else if (command == "show") {  // handle show command
      if (iss >> deviceName) {
        if (Manual *m = searchManual(deviceName)) {
          log = showDevice(*m);
          cout << log << endl;
          return log;
        } else if (PrefixCycle *p = searchPrefixCycle(deviceName)) {
          log = showDevice(*p);
          cout << log << endl;
          return log;
        }
        return logError("Dispositivo non trovato");
      } else {
        log = showAll();
        cout << log << endl;
        return log;
      }
    } else if (command == "rm") {  // handle "rm" command
      iss >> deviceName;
      if (Manual *m = searchManual(deviceName)) {
        log = m->getStatus() ? removeFromRunning(*m) : removeFromWaiting(*m);
        cout << log << endl;
        return log;
      } else if (PrefixCycle *p = searchPrefixCycle(deviceName)) {
        log = p->getStatus() ? removeFromRunning(*p) : removeFromWaiting(*p);
        cout << log << endl;
        return log;
      }
      return logError("Dispositivo non trovato");
    } else if (command == "exit") {  // handle exit command
      setSystemState(false);         // deactivate system
      log = scheduler_.getStringCurrTime() + " FINE";
      cout << log << endl;
      return log;
    } else {
      return logError("Comando non riconosciuto");
    }
  } catch (const InvalidTimeFormat &e) {  // handle invalid time format
    return logError("Formato orario non valido");
  } catch (const exception &e) {  // handle generic exception
    return logError("Errore: " + string(e.what()));
  }
  return logError("Errore");
}

/**
 * @brief Prompts the user for input and returns the entered string.
 * @return A string containing the user's input.
 */
string DomoticSystem::getCustomInput() {
  cout << ">> ";
  string input;
  getline(cin, input);
  return input;
}

/**
 * @brief Sets the state of the domotic system.
 * @param state true to activate the system, false to deactivate it.
 */
void DomoticSystem::setSystemState(bool state) { status_ = state; }

/**
 * @brief Gets the current state of the system.
 * @return true if the system is active, false otherwise.
 */
bool DomoticSystem::getSystemState() const { return status_; }
