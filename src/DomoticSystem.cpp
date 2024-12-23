#include "../include/DomoticSystem.h"

using namespace std;

DomoticSystem::DomoticSystem()
{
  buildDevices();
}

void DomoticSystem::setOn(Device& device_) {
}

void DomoticSystem::setOff(Device& device_) {
}

void DomoticSystem::setDeviceTimer(Device& device_) {
}

void DomoticSystem::removeFromRunning(Device& device_) {
}

void::DomoticSystem::removeFromWaiting(Device& device_) {
}

// show methods

void DomoticSystem::setTime(Time& time_) {
}

void DomoticSystem::resetTime() {
}

void DomoitcSystem::resetTimers() {
}

void DomoticSystem::resetAll() {
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
    cout << "Errore: impossibile aprire il file " << file.name() << endl;
    return;
  }

  string line;
  while (getline(file, line)) {
    stringstream ss(line);
    string id, name, type, pow;
    double power_;

    // leggo dati separati da virgola
    getline(ss, id, ',');
    getline(ss, name, ',');
    getline(ss, type, ',');
    getline(ss, pow, ',');

    power = stod(pow);

    // gestione di M o CP
    if (type == "Manuale") {
      devices_.push_back(Manual(id, name, power, NULL, NULL);
    } else {
      Time cycleDuration(parseTime(type));
      devices_.push_back(PrefixCycle(id, name, power, cycleDuration, NULL, NULL));
    }
  }

  file.close();
}

Device DomoitcSystem::searchDevice(string name_) {
  for (Device& d : devices_) {
    if (d.getName() == name_) {
      return d;
    }
  }
  return NULL;
}

void DomoticSystem::handleDevices(const std::string input_) {
  istringstream iss(input_);
  string command, deviceName;
  iss >> command;

  if (command == "set") {
    iss >> deviceName;
    string action;
    iss >> action;

    Device d = searchDevice(deviceName);

    if (action == "on") {
      setOn(d); // change Time start to now
    } else if (action == "off") {
      setOff(d); // change Time end to now
    } else {
      string startTime = action;
      string endTime;
      if (iss >> endTime) {
        Manual m(d.getId(), d.getName(), d.getPowerConsumption(), startTime, endTime);
        // manca check per vedere se é manual 
        // non posso eseguire questo comando se é CP
        setDeviceTimer(m);
      } else {
        cout << "[Errore] Occorre orario di fine" << endl;
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
  } else if (command == "rm") {
    iss >> deviceName;
    // remove from running or from waiting
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
