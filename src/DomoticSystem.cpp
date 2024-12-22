#include "../include/DomoticSystem.h"

using namespace std;

DomoticSystem::DomoticSystem()
{
  buildDevices();
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
      Time cycleDuration = parseTime(type);
      devices_.push_back(PrefixCycle(id, name, power, cycleDuration, NULL, NULL));
    }
  }

  file.close();
}

void DomoticSystem::handleDevices(const std::string input_) {
  istringstream iss(input_);
  string command, deviceName;
  iss >> command;

  if (command == "set") {
    iss >> deviceName;
    string action;
    iss >> action;

    if (action == "on") {
      setOn(); // pass a Device (search on devices_)
    } else if (action == "off") {
      setOff(); // pass a device (dunno if make check)
    } else {
      cout << "[Erorre] Comando non riconosciuto" << endl;
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
