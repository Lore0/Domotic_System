// FEDERICO COMPAGNO

#ifndef CLOCK_H
#define CLOCK_H

#include <sstream>
#include <stdexcept>
#include <string>
#include "InvalidTimeFormat.h"

// Clock class to handle time in hours and minutes.
class Clock {
 private:
  int hours_;       // Stores the hours.
  int minutes_;     // Stores the minutes.

  // Converts a string representation of time to hours and minutes.
  void StringToTime(const std::string& time);

 public:
  // Constructor that initializes the clock with a string time.
  explicit Clock(const std::string time);

  // Default constructor.
  Clock();

  // Adds another Clock object's time to the current object.
  Clock addTime(const Clock& time);

  // Sets the clock time using a string.
  void setClock(const std::string& time);

  // Returns the hours.
  int getHours() const;

  // Returns the minutes.
  int getMinutes() const;

  // Returns a string representation of the time.
  std::string to_string() const;

  // Calculates the time difference between two Clock objects.
  double getDistance(const Clock& time) const;

  // Assignment operator.
  Clock& operator=(const Clock& time);
};

// Compares if one Clock object is greater than another.
bool operator>(const Clock& t1, const Clock& t2);

// Compares if one Clock object is less than another.
bool operator<(const Clock& t1, const Clock& t2);

// Compares if two Clock objects are equal.
bool operator==(const Clock& t1, const Clock& t2);

#endif  // CLOCK_H