#include "../include/DomoticSystem.h"
#include "../include/Manual.h"
#include "../include/PrefixCycle.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

DomoticSystem::DomoticSystem()
  : scheduler_(kMaxPower)
{
  buildDevices();
}

string DomoticSystem::setOn(Device& device_) {
  string log = scheduler_.setOn(device_);
  return log;
}

string DomoticSystem::setOff(Device& device_) {
  string log = scheduler_.setOff(device_);
  return log;
}

string DomoticSystem::setDeviceTimer(Device& device_) {
  string log = scheduler_.setDeviceTimer(device_);
  return log; 
}

string DomoticSystem::removeFromRunning(Device& device_) {
  string log = setOff(device_);
  return log;
}

string DomoticSystem::removeFromWaiting(Device& device_) {
  string log = scheduler_.removeTimerFromWaiting(device_);
  return log;
}

// show methods

string DomoticSystem::setTime(Time& time_) {
  string log = scheduler_.setTime(time_);
  return log;
}

string DomoticSystem::resetTime() {
  string log = scheduler_.resetTime();
  return log;
}

string DomoticSystem::resetTimers() {
  string log = scheduler_.resetTimers();
  return log;
}

string DomoticSystem::resetAll() {
  resetTimers();
  resetTime();
  return "Comando resetAll() eseguito con successo";
  // bisogna fare il log (perché non chiamo riccardo)
}

string DomoticSystem::parseTime(const string& time_) {
  int hours = 0;
  int minutes = 0;
  istringstream iss(time_);
  string temp;

  while (iss >> temp) {
    if (temp == "ora" || temp == "ore") {
      iss >> hours;
    } else if (temp == "minuto" || temp == "minuti") {
      iss >> minutes;
    }
  }

  // costruisco il res come "HH:MM"
  ostringstream res;
  res << hours << ":" << (minutes < 10 ? "0" : "") << minutes;
  return res.str();
}

bool DomoticSystem::isValidTimeFormat(const string& time) {
  regex timeFormat("^([0-1][0-9]|2[0-3]):([0-5][0-9])$");
  return regex_match(time, timeFormat);
}

void DomoticSystem::buildDevices() {
  ifstream file("devices.txt");
  if (!file.is_open()) {
    cout << "Errore: impossibile aprire il file devices.txt" << endl;
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

    Time null("-1");

    // gestione di M o CP
    if (type == "Manuale") {
      devices_.push_back(Manual(id, name, power, null, null));
    } else {
      Time cycleDuration(parseTime(type));
      devices_.push_back(PrefixCycle(id, name, power, null, cycleDuration));
    }
  }

  file.close();
}

Device* DomoticSystem::searchDevice(const string& name_) {
  for (Device& d : devices_) {
    if (d.getName() == name_) {
      return &d; // Return the address of the found device
    }
  }
  cout << "[Errore] Dispositivo non trovato" << endl;
  return nullptr; // Return nullptr if the device is not found
}

string DomoticSystem::handleInput(const string input_) {
  istringstream iss(input_);
  string command, deviceName;
  string log;
  iss >> command;

  if (command == "set") {
    iss >> deviceName;
    string action;
    iss >> action;

    Device* d = searchDevice(deviceName);
    if (d == nullptr) {
      return "[Errore] Dispositivo non trovato";
    }

    if (action == "on") {
      if (d->getStatus()) {
        cout << "[Errore] Dispositivo già acceso" << endl;
      } else {
        log = setOn(*d);
        return log;
      }
    } else if (action == "off") {
      if (!d->getStatus()) {
        cout << "[Errore] Dispositivo già spento o mai acceso" << endl;
      } else {
        log = setOff(*d);
        return log;
      }
    } else if (action == "time") {
      string time;
      iss >> time;
      if (!isValidTimeFormat(time)) {
        cout << "[Errore] Formato orario non valido" << endl;
        return;
      }
      Time t(time);
      log = setTime(t);
      return log;
    } else {
      string startTime = action;
      string endTime;
      if (iss >> endTime) {
        if (Manual* m = dynamic_cast<Manual*>(d)) {
          // need to check if time is valid input
          if (!isValidTimeFormat(startTime) || !isValidTimeFormat(endTime)) {
            cout << "[Errore] Formato orario non valido" << endl;
            return;
          }
          m->setStart(Time(startTime));
          m->setFinish(Time(endTime));
          log = setDeviceTimer(*m);
          return log;
        } else {
          cout << "[Errore] Non puoi dare orario di fine ad un device CP" << endl;
        }
      } else {
        if (Manual* m = dynamic_cast<Manual*>(d)) {
          if (!isValidTimeFormat(startTime)) {
            cout << "[Errore] Formato orario non valido" << endl;
            return;
          }
          m->setStart(Time(startTime));
          log = setDeviceTimer(*m);
          return log;
        } else {
          PrefixCycle* p = dynamic_cast<PrefixCycle*>(d);
          if (!isValidTimeFormat(startTime)) {
            cout << "[Errore] Formato orario non valido" << endl;
            return;
          }
          p->setStartFinish(Time(startTime));
          log = setDeviceTimer(*p);
          return log;
        }
      }
    }
  } else if (command == "reset") {
    string target;
    iss >> target;

    if (target == "time") {
      log = resetTime();
      return log;
    } else if (target == "timers") {
      log = resetTimers();
      return log;
    } else if (target == "all") {
      log = resetAll();
      return log;
    } else {
      cout << "[Errore] Comando non riconosciuto" << endl;
    }
  } else if (command == "show") {
    // do search method call
  } else if (command == "rm") {   //NO, DEVE RIMUOVERE IL TIMER NO IL DISPOSITIVO
    iss >> deviceName;
    Device* d = searchDevice(deviceName);
    if (d->getStatus() == true) {
      log = removeFromRunning(*d);
      return log;
    } else {
      log = removeFromWaiting(*d);
      return log;
    }
  } else {
    cout << "[Errore] Comando non riconosciuto" << endl;
  }
}

string DomoticSystem::getCustomInput() {
  cout << ">> ";
  string input;
  getline(cin, input);
  return input;
}
