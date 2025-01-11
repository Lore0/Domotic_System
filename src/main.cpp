#include "../include/DomoticSystem.h"

#include <fstream>

using namespace std;

int main() {
  DomoticSystem domoticSystem;
  string log = "";
  ofstream logFile("log.txt");
  while (domoticSystem.getSystemState()) {
    string command = domoticSystem.getCustomInput();
    log += "\n>> " + command + "\n";
    log += domoticSystem.handleInput(command);
  }
  domoticSystem.log(log, logFile);
  logFile.close();
  return 0;
}
