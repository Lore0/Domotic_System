#include "../include/DomoticSystem.h"

#include <iostream>
#include <fstream>

using namespace std;

int main() {
  DomoticSystem domoticSystem;
  string log = "";
  Time t1("12:00");
  Time t2("14:00");
  t1 = t2;
  cout << t1.to_string() << endl;
  ofstream logFile("log.txt", ios::app);
  while (true) {
    log += domoticSystem.handleInput(domoticSystem.getCustomInput());
  }
  domoticSystem.log(log, logFile);
  logFile.close();
  return 0;
}
