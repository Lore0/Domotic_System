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
  return scheduler_.setOn(device_);
}

string DomoticSystem::setOff(Device& device_) {
  return scheduler_.setOff(device_);
}

string DomoticSystem::setDeviceTimer(Device& device_) {
  return scheduler_.setDeviceTimer(device_);
}

string DomoticSystem::removeFromRunning(Device& device_) {
  return setOff(device_);
}

string DomoticSystem::removeFromWaiting(Device& device_) {
  return scheduler_.removeTimerFromWaiting(device_);
}

// show methods

string DomoticSystem::setTime(Time& time_) {
  return scheduler_.setTime(time_);
}

string DomoticSystem::resetTime() {
  return scheduler_.resetTime();
}

string DomoticSystem::resetTimers() {
  return scheduler_.resetTimers();
}

string DomoticSystem::resetAll() {
  resetTimers();
  resetTime();
  string log = scheduler_.getCurrTime() + " Comando resetAll() eseguito con successo";
  return log;
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

void DomoticSystem::log(const string& message, ofstream logFile) {
  // ofstream logFile("log.txt", ios::app); da fare nel main
  if (logFile.is_open()) {
    logFile << message << endl;
    // logFile.close(); da fare nel main
  }
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
  return nullptr; // Return nullptr if the device is not found
}

string DomoticSystem::handleInput(const string input_) {
  istringstream iss(input_);
  string command, deviceName;
  iss >> command;
  string log;

  if (command == "set") {
    iss >> deviceName;
    string action;
    iss >> action;

    Device* d = searchDevice(deviceName);
    if (d == nullptr) {
      log = scheduler_.getCurrTime() + " Errore : Dispositivo non trovato";
      cout << log << endl;
      return log;
    }

    if (action == "on") {
      if (d->getStatus()) {
        log = scheduler_.getCurrTime() + " Errore : " + d->getName() + " è già acceso";
        cout << log << endl;
        return log;
      } else {
        log = setOn(*d);
        cout << log << endl;
        return log;
      }
    } else if (action == "off") {
      if (!d->getStatus()) {
        log = scheduler_.getCurrTime() + " Errore : " + d->getName() + " è già spento";
        cout << log << endl;
        return log;
      } else {
        log = setOff(*d);
        cout << log << endl;
        return log;
      }
    } else if (action == "time") {
      string time;
      iss >> time;
      if (!isValidTimeFormat(time)) {
        log = scheduler_.getCurrTime() + " Errore : Formato orario non valido";
        cout << log << endl;
        return log;
      }
      Time t(time);
      log = setTime(t);
      cout << log << endl;
      return log;
    } else {
      string startTime = action;
      string endTime;
      if (iss >> endTime) {
        if (Manual* m = dynamic_cast<Manual*>(d)) {
          // need to check if time is valid input
          if (!isValidTimeFormat(startTime) || !isValidTimeFormat(endTime)) {
            log = scheduler_.getCurrTime() + " Errore : Formato orario non valido";
            cout << log << endl;
            return log;
          }
          m->setStart(Time(startTime));
          m->setFinish(Time(endTime));
          log = setDeviceTimer(*m);
          cout << log << endl;
          return log;
        } else {
          log = scheduler_.getCurrTime() + " Errore : Non puoi dare orario di fine ad un device CP";
          cout << log << endl;
          return log;
        }
      } else {
        if (Manual* m = dynamic_cast<Manual*>(d)) {
          if (!isValidTimeFormat(startTime)) {
            log = scheduler_.getCurrTime() + " Errore : Formato orario non valido";
            cout << log << endl;
            return log;
          }
          m->setStart(Time(startTime));
          log = setDeviceTimer(*m);
          cout << log << endl;
          return log;
        } else {
          PrefixCycle* p = dynamic_cast<PrefixCycle*>(d);
          if (!isValidTimeFormat(startTime)) {
            log = scheduler_.getCurrTime() + " Errore : Formato orario non valido";
            cout << log << endl;
            return log;
          }
          p->setStartFinish(Time(startTime));
          log = setDeviceTimer(*p);
          cout << log << endl;
          return log;
        }
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
      log = scheduler_.getCurrTime() + " Errore : Comando non riconosciuto";
      cout << log << endl;
      return log;
    }
  } else if (command == "show") {
    // do search method call
  } else if (command == "rm") {   //NO, DEVE RIMUOVERE IL TIMER NO IL DISPOSITIVO
    iss >> deviceName;
    Device* d = searchDevice(deviceName);
    if (d->getStatus() == true) {
      log = removeFromRunning(*d);
      cout << log << endl;
      return log;
    } else {
      log = removeFromWaiting(*d);
      cout << log << endl;
      return log;
    }
  } else {
    log = scheduler_.getCurrTime() + " Errore : Comando non riconosciuto";
    cout << log << endl;
    return log;
  }
}

string DomoticSystem::getCustomInput() {
  cout << ">> ";
  string input;
  getline(cin, input);
  return input;
}
