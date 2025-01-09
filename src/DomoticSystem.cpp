#include "../include/DomoticSystem.h"

using namespace std;

DomoticSystem::DomoticSystem()
    : scheduler_(3.5) // da sistemare brutto value
{
  buildDevices();
}

string DomoticSystem::setOn(Device &device_)
{
  string time = logTime() + "\n";
  return time + scheduler_.setOn(&device_);
}

string DomoticSystem::setOff(Device &device_)
{
  string time = logTime() + "\n";
  return time + scheduler_.setOff(&device_);
}

string DomoticSystem::setDeviceTimer(Device &device_)
{
  string time = logTime() + "\n";
  return time + scheduler_.setDeviceTimer(&device_);
}

string DomoticSystem::removeFromRunning(Device &device_)
{
  string time = logTime() + "\n";
  return time + scheduler_.removeTimerFromRunning(&device_);
}

string DomoticSystem::removeFromWaiting(Device &device_)
{
  string time = logTime() + "\n";
  return time + scheduler_.removeTimerFromWaiting(&device_);
}

string DomoticSystem::showDevice(Device &device_)
{
  string time = logTime() + "\n";
  Device* device = &device_;
  double tot = scheduler_.showDevice(device);
  if (tot <= 0) {
    tot *= (-1);
    return time + scheduler_.getStringCurrTime() + " Il dispositivo " + device->getName() + " ha attualmente consumato " + to_string(tot) + " kWh";
  } else {
    return time + scheduler_.getStringCurrTime() + " Il dispositivo " + device->getName() + " ha attualmente prodotto " + to_string(tot) + " kWh";
  }
}

string DomoticSystem::showAll()
{
  string time = logTime() + "\n";
  double tot = 0;
  string eachDevice = "";
  for (Device d : manual_) {
    double consumpt = scheduler_.showDevice(&d);
    tot += consumpt;
    if (consumpt <= 0) {
      consumpt *= (-1);
      eachDevice += "\n  - Il dispositivo " + d.getName() + " ha consumato " + to_string(consumpt) + " kWh";
    } else {
      eachDevice += "\n  - Il dispositivo " + d.getName() + " ha prodotto " + to_string(consumpt) + " kWh";
    }
  }
  for (Device d : prefixCycle_) {
    double consumpt = scheduler_.showDevice(&d);
    tot += consumpt;
    if (consumpt <= 0) {
      consumpt *= (-1);
      eachDevice += "\n  - Il dispositivo " + d.getName() + " ha consumato " + to_string(consumpt) + " kWh";
    } else {
      eachDevice += "\n  - Il dispositivo " + d.getName() + " ha prodotto " + to_string(consumpt) + " kWh";
    }
  }
  if (tot >= 0) {
    return time + scheduler_.getStringCurrTime() + "Attualmente il sistema ha prodotto " + to_string(tot) + " kWh. Nello specifico:" + eachDevice;
  } else {
    tot *= (-1);
    return time + scheduler_.getStringCurrTime() + "Attualmente il sistema ha consumato " + to_string(tot) + " kWh. Nello specifico:" + eachDevice;
  }
}

string DomoticSystem::setTime(Time &time_)
{
  string time = logTime() + "\n";
  return time + scheduler_.setTime(time_);
}

string DomoticSystem::resetTime()
{
  string time = logTime() + "\n";
  return time + scheduler_.resetTime();
}

string DomoticSystem::resetTimers()
{
  string time = logTime() + "\n";
  return time + scheduler_.resetTimers();
}

string DomoticSystem::resetAll()
{
  resetTimers();
  resetTime();
  string time = logTime() + "\n";
  string log = time + scheduler_.getStringCurrTime() + " Comando resetAll() eseguito con successo";
  return log;
}

string DomoticSystem::logTime()
{
  return scheduler_.getStringCurrTime() + " L'orario attuale è " + scheduler_.getCurrTime().to_string();
}

bool DomoticSystem::isValidTimeFormat(const string &time)
{
  regex timeFormat("^([0-1][0-9]|2[0-3]):([0-5][0-9])$");
  return regex_match(time, timeFormat);
}

// TODO: da vedere se aprire e chiudere il file ogni volta
void DomoticSystem::log(const string &message, ofstream &logFile)
{
  // ofstream logFile("log.txt", ios::app); da fare nel main
  if (logFile.is_open())
  {
    logFile << message << endl;
    // logFile.close(); da fare nel main
  }
}

void DomoticSystem::buildDevices()
{
  ifstream file("../docs/device.txt");
  if (!file.is_open())
  {
    cout << "Errore: impossibile aprire il file device.txt" << endl;
    return;
  }

  string line;
  while (getline(file, line))
  {
    stringstream ss(line);
    string id, name, type, pow;

    // leggo dati separati da virgola
    getline(ss, id, ',');
    getline(ss, name, ',');
    getline(ss, type, ',');
    getline(ss, pow, ',');

    double power = stod(pow);

    Time null("00:00");

    // gestione di M o CP
    if (type == "Manuale")
    {
      manual_.push_back(Manual(id, name, power, null, null));
    }
    else
    {
      Time cycleDuration(type);
      prefixCycle_.push_back(PrefixCycle(id, name, power, null, cycleDuration));
    }
  }

  file.close();
}

Manual *DomoticSystem::searchManual(const string &name)
{
  for (auto &m : manual_)
  {
    if (m.getName() == name) return &m;
  }
  return nullptr;
}

PrefixCycle *DomoticSystem::searchPrefixCycle(const string &name)
{
  for (auto &p : prefixCycle_)
  {
    if (p.getName() == name) return &p;
  }
  return nullptr;
}

string DomoticSystem::handleInput(const string &input_)
{
  istringstream iss(input_);
  string command, deviceName, target;
  iss >> command;
  string log;

  // da togliere auto
  auto logError = [this](const string &error)
  {
    string logM = scheduler_.getStringCurrTime() + " Errore : " + error;
    cout << logM << endl;
    return logM;
  };

  if (command == "set")
  {
    iss >> deviceName;
    if (deviceName == "time")
    {
      string time;
      iss >> time;
      if (!isValidTimeFormat(time))
      {
        return logError("Formato orario non valido");
      }
      Time t(time);
      log = setTime(t);
      cout << log << endl;
      return log;
    }
    else
    {
      string action;
      iss >> action;

      // new code
      if (Manual *m = searchManual(deviceName))
      {
        if (action == "on")
        {
          if (m->getStatus()) {
            logError(m->getName() + " è già acceso");
          } else {
            m->setStart(scheduler_.getCurrTime());
            m->setFinish(Time("24:00"));
            log = setOn(*m);
          }
          cout << log << endl;
          return log;
        }
        else if (action == "off")
        {
          log = !m->getStatus() ? logError(m->getName() + " è già spento") : setOff(*m);
          cout << log << endl;
          return log;
        }
        else
        {
          string startTime = action, endTime;
          iss >> endTime;
          if (m->hasTimer()) {
            return logError(m->getName() + " ha già un timer");
          }
          if (!isValidTimeFormat(startTime) || (!endTime.empty() && !isValidTimeFormat(endTime)))
          {
            return logError("Formato orario non valido");
          }
          m->setStart(Time(startTime));
          if (!endTime.empty())
          {
            m->setFinish(Time(endTime));
          }
          else
          {
            m->setFinish(Time("24:00")); // andare avanti all'infinito
          }
          if (m->getStatus()) {
            return logError(m->getName() + " è già acceso");
          }
          log = setDeviceTimer(*m);
          cout << log << endl;
          return log;
        }
      }
      else if (PrefixCycle *p = searchPrefixCycle(deviceName))
      {
        if (action == "on")
        {
          if (p->getStatus()) {
            logError(p->getName() + " è già acceso");
          } else {
            p->setStart(scheduler_.getCurrTime());
            p->setFinish(scheduler_.getCurrTime().addTime(p->getDuration()));
            log = setOn(*p);
          }
          cout << log << endl; // Stampa il risultato
          return log;
        }
        else if (action == "off")
        {
          log = !p->getStatus() ? logError(p->getName() + " è già spento") : setOff(*p);
          cout << log << endl; // Stampa il risultato
          return log;
        }
        else
        {
          string startTime = action;
          string endTime;

          if (p->hasTimer()) {
            return logError(p->getName() + " ha già un timer");
          }

          if (iss >> endTime)
          {
            return logError("Dispositivo CP non supporta orario di fine. Devi specificare solo l'orario di inizio");
          }

          if (!isValidTimeFormat(startTime))
          {
            return logError("Formato orario non valido");
          }
          Time start(startTime);
          p->setStart(start);
          p->setFinish(start.addTime(p->getDuration()));
          if (p->getStatus()) {
            return logError(p->getName() + " è già acceso");
          }
          log = setDeviceTimer(*p);
          cout << log << endl; // Stampa il risultato
          return log;
        }
      }
      else
      {
        return logError("Dispositivo non trovato");
      }
    }
  }
  else if (command == "reset")
  {
    string target;
    iss >> target;

    if (target == "time")
    {
      log = resetTime();
      cout << log << endl;
      return log;
    }
    else if (target == "timers")
    {
      log = resetTimers();
      cout << log << endl;
      return log;
    }
    else if (target == "all")
    {
      log = resetAll();
      cout << log << endl;
      return log;
    }
    else
    {
      return logError("Comando non riconosciuto");
    }
  }
  else if (command == "show")
  {
    if (iss >> deviceName)
    {
      if (Manual *m = searchManual(deviceName))
      {
        log = showDevice(*m);
        cout << log << endl;
        return log;
      }
      else if (PrefixCycle *p = searchPrefixCycle(deviceName))
      {
        log = showDevice(*p);
        cout << log << endl;
        return log;
      }
      return logError("Dispositivo non trovato");
    }
    else
    {
      log = showAll();
      cout << log << endl;
      return log;
    }
  }
  else if (command == "rm")
  { // NO, DEVE RIMUOVERE IL TIMER NO IL DISPOSITIVO
    iss >> deviceName;
    if (Manual *m = searchManual(deviceName))
    {
      log = m->getStatus() ? removeFromRunning(*m) : removeFromWaiting(*m);
      cout << log << endl;
      return log;
    }
    else if (PrefixCycle *p = searchPrefixCycle(deviceName))
    {
      log = p->getStatus() ? removeFromRunning(*p) : removeFromWaiting(*p);
      cout << log << endl;
      return log;
    }
    return logError("Dispositivo non trovato");
  }
  else
  {
    return logError("Comando non riconosciuto");
  }
  return logError("Errore");
}

string DomoticSystem::getCustomInput()
{
  cout << ">> ";
  string input;
  getline(cin, input);
  return input;
}
