#include "../include/DomoticSystem.h"

#include <iostream>
#include <fstream>

using namespace std;

int main() {
  DomoticSystem domoticSystem;
  string log = "";
  ofstream logFile("log.txt", ios::app);
  while (domoticSystem.getSystemState()) {
    log += domoticSystem.handleInput(domoticSystem.getCustomInput());
  }
  domoticSystem.log(log, logFile);
  logFile.close();
  return 0;
}
