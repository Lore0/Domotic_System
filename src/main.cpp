#include "../include/DomoticSystem.h"

#include <fstream>

using namespace std;

/**
 * @brief Main function to run the domotic system.
 * 
 * The main function initializes a `DomoticSystem` object and continuously processes
 * user commands while the system is active. Commands are logged, and the system's
 * responses are appended to the log. Upon exiting the loop, the log is saved to a file.
 * 
 * @return int Returns 0 to indicate successful execution.
 */
int main() {
  DomoticSystem domoticSystem;                          // Object managing the domotic system
  string log = "";
  ofstream logFile("log.txt");                          // Output file stream for logging 
  while (domoticSystem.getSystemState()) {
    string command = domoticSystem.getCustomInput();    // Get user input
    log += "\n>> " + command + "\n";                    // Log the command
    log += domoticSystem.handleInput(command);          // Log the system's response
  }
  
  // Save the log to the file
  domoticSystem.log(log, logFile);
  logFile.close();
  return 0;
}