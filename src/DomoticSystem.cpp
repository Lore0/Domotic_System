#include "../include/DomoticSystem.h"

using namespace std;

DomoticSystem::DomoticSystem() : scheduler_(kMaxPower), status_(true) {
  buildDevices();
}

string DomoticSystem::setOn(Device &device_) {
  return logTime() + scheduler_.setOn(&device_);
}

string DomoticSystem::setOff(Device &device_) {
  return logTime() + scheduler_.setOff(&device_);
}

string DomoticSystem::setDeviceTimer(Device &device_) {
  return logTime() + scheduler_.setDeviceTimer(&device_);
}

string DomoticSystem::removeFromRunning(Device &device_) {
  return logTime() + scheduler_.removeTimerFromRunning(&device_);
}

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

string DomoticSystem::setTime(Clock &time_) {
  string time = logTime();
  return time + scheduler_.setTime(time_);
}

string DomoticSystem::resetTime() { return logTime() + scheduler_.resetTime(); }

string DomoticSystem::resetTimers() {
  return logTime() + scheduler_.resetTimers();
}

string DomoticSystem::resetAll() {
  resetTimers();
  resetTime();
  string time = logTime();
  string log = time + scheduler_.getStringCurrTime() +
               " Comando reset all eseguito con successo\n";
  return log;
}

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

    // leggo dati separati da virgola
    getline(ss, id, ',');
    getline(ss, name, ',');
    getline(ss, type, ',');
    getline(ss, pow, ',');

    double power = stod(pow);

    Clock null("00:00");

    // gestione di M o CP
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
  istringstream iss(input_);
  string command, deviceName;
  iss >> command;
  string log;

  try {
    if (command == "set") {
      iss >> deviceName;
      if (deviceName == "time") {
        string time;
        iss >> time;
        Clock t(time);
        if (t < scheduler_.getCurrTime()) {
          Clock end("24:00");
          log = setTime(end);
          log += "[00:00] FINE";
          cout << log << endl;
          setSystemState(false);
          return log;
        }
        log = setTime(t);
        cout << log << endl;
        return log;
      } else {
        string action;
        iss >> action;

        if (Manual *m = searchManual(deviceName)) {
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
          } else {
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
              m->setFinish(Clock("24:00"));  // andare avanti all'infinito
            }
            m->setStart(Clock(startTime));
            log = setDeviceTimer(*m);
            cout << log << endl;
            return log;
          }
        } else if (PrefixCycle *p = searchPrefixCycle(deviceName)) {
          if (action == "on") {
            if (p->getStatus()) {
              return logError(p->getName() + " è già acceso");
            } else {
              p->setStart(scheduler_.getCurrTime());
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
          } else {
            string startTime = action;
            string endTime;

            if (p->hasTimer()) {
              return logError(p->getName() + " ha già un timer");
            }

            if (Clock(startTime) < scheduler_.getCurrTime()) {
              return logError("Orario non valido");
            }

            if (iss >> endTime) {
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
    } else if (command == "reset") {
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
    } else if (command == "show") {
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
    } else if (command == "rm") {
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
    } else if (command == "exit") {
      setSystemState(false);
      log = "\n" + scheduler_.getStringCurrTime() + " FINE";
      cout << log << endl;
      return log;
    } else {
      return logError("Comando non riconosciuto");
    }
  } catch (const InvalidTimeFormat &e) {
    return logError("Formato orario non valido");
  } catch (const exception &e) {
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

void DomoticSystem::setSystemState(bool state) { status_ = state; }

bool DomoticSystem::getSystemState() const { return status_; }
