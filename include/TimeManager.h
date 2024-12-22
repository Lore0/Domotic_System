#ifndef DOMOTICSYSTEM_H
#define DOMOTICSYSTEM_H

class TimeManager {
  public:
    // implement methods
  private:
    vectro<Device> running_;
    vectro<Device> waiting_;
    vectro<Device> finished_;
}

#endif // DOMOTICSYSTEM_H
