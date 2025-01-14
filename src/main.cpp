// LORENZO PANIZZOLO

#include <fstream>

#include "../include/DomoticSystem.h"

using namespace std;

int main() {
  DomoticSystem domoticSystem;  // Object managing the domotic system.
  string log = "";
  ofstream logFile("log.txt");  // Output file stream for logging.
  while (domoticSystem.getSystemState()) {
    string command = domoticSystem.getCustomInput();  // Get user input.
    log += "\n>> " + command + "\n";                  // Log the command.
    log += domoticSystem.handleInput(command);  // Log the system's response.
  }

  // Save the log to the file.
  domoticSystem.log(log, logFile);
  logFile.close();
  return 0;
}