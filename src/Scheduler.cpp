#include "../include/Scheduler.h"

#include <algorithm>
#include <iostream>

//#include "../include/PrefixCycle.h"
using namespace std;



Scheduler::Scheduler(double maxPower)
    : MAX_POWER(maxPower), inUsePower(0), currTime("00:00") {
  // nothing
}

string Scheduler::setOn(Device *device)  
{
  string log = "";
  int targetIndex = searchDevice(waiting_, device);
  int i = 0;
  if (inUsePower + device->getPowerConsumption() + MAX_POWER >= 0) {
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
  } else {
    log = getStringCurrTime() +
          " ERRORE: non c'è corrente sufficiente per accendere-> " +
          device->getName() + "\n";
  }  // cosa fare in caso non c'è abbastanza potenza
  if (targetIndex != -1) {
    waiting_.erase(waiting_.begin() + targetIndex);
  }
  return log;
}

string Scheduler::setOff(Device *device) {
  int targetIndex = searchDevice(running_, device);
  running_.erase(running_.begin() + targetIndex);
  device->turnOff();
  device->setHasTimer(false);
  device->setFinish(currTime);
  inUsePower -= device->getPowerConsumption();
  string log = getStringCurrTime() + " Il dispositivo " + device->getName() +
               " si è spento" + "\n";  
  Device temp{device->getId(), device->getName(), device->getPowerConsumption(),
              device->getStart(), device->getFinish()};
  finished_.push_back(temp);
  if (inUsePower + MAX_POWER < 0) {
    int indexMaxStartTime = 0;
    for (int i = 0; i < running_.size(); i++) {
      if (running_.at(i)->getStart() >
          running_.at(indexMaxStartTime)->getStart())
        indexMaxStartTime = i;
    }
    log += setOff(running_.at(indexMaxStartTime)) + "\n";
  }
  return log;
}

string Scheduler::setDeviceTimer(Device *device) {
  int i = 0;
  device->setHasTimer(true);
  while (i < waiting_.size() &&
         !(device->getStart() < waiting_.at(i)->getStart()))
    i++;
  waiting_.insert(waiting_.begin() + i, device);
  return getStringCurrTime() + " Impostato un timer per il dispositivo " +
         device->getName() + " dalle " + device->getStart().to_string() +
         " alle " + device->getFinish().to_string() + "\n";  
}

// toglie il timer da waiting
string Scheduler::removeTimerFromWaiting(Device *device) {  
  int targetIndex = searchDevice(waiting_, device);
  device->setHasTimer(false);
  if (targetIndex == -1)
    return getStringCurrTime() + " Errore: dispositivo non in attesa" + "\n";
  waiting_.erase(waiting_.begin() + targetIndex);
  return getStringCurrTime() + " Rimosso il timer dal dispositivo " +
         device->getName() + "\n";  
}

// toglie il timer da running-- > va all'infinito
string Scheduler::removeTimerFromRunning(Device *device)  
{
  int targetIndex = searchDevice(running_, device);  // cerco il dispositivo in running
  if (targetIndex == -1)               // se non c'è do errore
    return getStringCurrTime() + " Errore: dispositivo non in funzione" + "\n";
  device->setFinish(Clock("24:00"));
  device->setHasTimer(false);
  running_.erase(running_.begin() + targetIndex);
  running_.push_back(device);
  return getStringCurrTime() + " Rimosso il timer dal dispositivo " +
         device->getName() + "\n";  
}

string Scheduler::setTime(Clock &time) {
  if (currTime > time)  // ora non valida
    return getStringCurrTime() + " Errore: ora minore a quella attuale" + "\n";
  // controllare se lista vuota
  // controllaere qual'è minore
  // se sono uguali tolgo prima da running
  // fino a che non sono entrambi < time
  string log = "";
  while (!(currTime > time)) {
    // controllo di fine (entrambe liste vuote)
    if ((running_.empty() && waiting_.empty())) {
      currTime = time;
      break;
    }  // se una delle due è vuote
    else if (running_.empty() || waiting_.empty()) {
      if (running_.empty())  // se running è vuota
      {
        // se primo waiting è già più grande
        if (waiting_.at(0)->getStart() > time) {
          currTime = time;
          break;
        } else  // waiting no vuota e un dispositivo si deve avviare
        {
          currTime = waiting_.at(0)->getStart();
          log += setOn(waiting_.at(0));
        }
      } else  // waiting vuota
      {
        // se primo running è già pèiù grande
        if (running_.at(0)->getFinish() > time) {
          currTime = time;
          break;
        } else  // running no vuota e un dispositivo si deve avviare
        {
          currTime = running_.at(0)->getFinish();
          log += setOff(running_.at(0));
        }
      }
    } else  // entrambe running waiting NO vuote
    {
      // waiting<running
      if (waiting_.at(0)->getStart() < running_.at(0)->getFinish()) {
        currTime = waiting_.at(0)->getStart();
        log += setOn(waiting_.at(0));
      } else  // waiting>=running
      {
        currTime = running_.at(0)->getFinish();
        log += setOff(running_.at(0));
      }
    }
  }
  if (currTime == Clock("24:00"))  // non vogliamo che venga scritto 24:00
    currTime = Clock("00:00");
  return log + getStringCurrTime() + " L'orario attuale è " +
         getCurrTime().to_string() + "\n";
}

string Scheduler::resetTime() {
  currTime.setTime("00:00");
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

string Scheduler::resetTimers()  // da rivedere
{
  string log = "";
  for (auto &device : waiting_) {
    log += removeTimerFromWaiting(device);
  }
  for (auto &device : running_) {
    log += removeTimerFromRunning(device);
  }
  return log;
}

string Scheduler::showAll() { return ""; }

double Scheduler::showDevice(Device *device) {
  double tothour = 0;
  int targetIndex = searchDevice(running_, device);
  cout<<device->getName()<<targetIndex<<"---"<<running_.at(0)->getName()<<endl;
  if (targetIndex != -1) {
    tothour += currTime.getDistance(running_.at(targetIndex)->getStart());
    cout<<tothour<<endl;
  }
  int i = 0;
  while (i < finished_.size()) {
    if (device->getId() == finished_.at(i).getId())
      tothour += finished_.at(i).getFinish().getDistance(finished_.at(i).getStart());
    i++;
  }
  return tothour * device->getPowerConsumption();
}

int searchDevice(vector<Device *> list, Device *target) {
  // Usa std::find per cercare il dispositivo
  auto iter = find(list.begin(), list.end(), target);

  if (iter != list.end()) {
    // Calcola e restituisci l'indice
    return distance(list.begin(), iter);
  }
  // Se non trovato, restituisce -1
  return -1;
}

string Scheduler::getStringCurrTime() {
  return "[" + currTime.to_string() + "]";
}
Clock Scheduler::getCurrTime() { return currTime; }