#include "../include/DomoticSystem.h"
#include "../include/Manual.h"
#include "../include/PrefixCycle.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

DomoticSystem::DomoticSystem(Scheduler s)
  : scheduler_(s)
{
  buildDevices();
}

void DomoticSystem::setOn(Device& device_) {
  scheduler_.setOn(device_);
}

void DomoticSystem::setOff(Device& device_) {
  scheduler_.setOff(device_);
}

void DomoticSystem::setDeviceTimer(Device& device_) {
  scheduler_.setDeviceTimer(device_);
}

void DomoticSystem::removeFromRunning(Device& device_) {
  setOff(device_);
}

void::DomoticSystem::removeFromWaiting(Device& device_) {
  scheduler_.removeTimerFromWaiting(device_);
}

// show methods

void DomoticSystem::setTime(Time& time_) {
  scheduler_.setTime(time_);
}

void DomoticSystem::resetTime() {
  scheduler_.resetTime();
}

void DomoticSystem::resetTimers() {
  scheduler_.resetTimers();
}

void DomoticSystem::resetAll() {
  resetTimers();
  resetTime();
}

string DomoticSystem::parseTime(string time_) {
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

Device* DomoticSystem::searchDevice(string name_) {
  for (Device& d : devices_) {
    if (d.getName() == name_) {
      return &d; // Return the address of the found device
    }
  }
  cout << "[Errore] Dispositivo non trovato" << endl;
  return nullptr; // Return nullptr if the device is not found
}

void DomoticSystem::handleInput(const std::string input_) {
  istringstream iss(input_);
  string command, deviceName;
  iss >> command;

  if (command == "set") {
    iss >> deviceName;
    string action;
    iss >> action;

    Device* d = searchDevice(deviceName);

    if (action == "on") {
      if (d->getStatus()) {
        cout << "[Errore] Dispositivo già acceso" << endl;
      } else {
        setOn(*d);
      }
    } else if (action == "off") {
      if (!d->getStatus()) {
        cout << "[Errore] Dispositivo già spento o mai acceso" << endl;
      } else {
        setOff(*d);
      }
    } else if (action == "time") {
      string time;
      iss >> time;
      // check if time is valid
      Time t(time);
      setTime(t);
    } else {
      string startTime = action;
      string endTime;
      if (iss >> endTime) {
        if (Manual* m = dynamic_cast<Manual*>(d)) {
          // need to check if time is valid input
          m->setStart(Time(startTime));
          m->setFinish(Time(endTime));
          setDeviceTimer(*m);
        } else {
          cout << "[Errore] Non puoi dare orario di fine ad un device CP" << endl;
        }
      } else {
        if (Manual* m = dynamic_cast<Manual*>(d)) {
          m->setStart(Time(startTime));
          setDeviceTimer(*m);
        } else {
          PrefixCycle* p = dynamic_cast<PrefixCycle*>(d);
          p->setStartFinish(Time(startTime));
          setDeviceTimer(*p);
        }
      }
    }
  } else if (command == "reset") {
    string target;
    iss >> target;

    if (target == "time") {
      resetTime();
    } else if (target == "timers") {
      resetTimers();
    } else if (target == "all") {
      resetAll();
    } else {
      cout << "[Errore] Comando non riconosciuto" << endl;
    }
  } else if (command == "show") {
    // do search method call
  } else if (command == "rm") {   //NO, DEVE RIMUOVERE IL TIMER NO IL DISPOSITIVO
    iss >> deviceName;
    Device* d = searchDevice(deviceName);
    d->getStatus() == true ? removeFromRunning(*d) : removeFromWaiting(*d);
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
