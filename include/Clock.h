#ifndef CLOCK_H
#define CLOCK_H

#include <sstream>
#include <stdexcept>
#include <string>

#include "InvalidTimeFormat.h"

class Clock {
 private:
  int hours_;

  int minutes_;

  void StringToTime(const std::string& time);

 public:
  explicit Clock(const std::string time);

  Clock();

  Clock addTime(const Clock& time);

  void setClock(const std::string& time);

  int getHours() const;

  int getMinutes() const;

  std::string to_string() const;

  double getDistance(const Clock& time) const;

  Clock& operator=(const Clock& time);
};

bool operator>(const Clock& t1, const Clock& t2);
bool operator<(const Clock& t1, const Clock& t2);
bool operator==(const Clock& t1, const Clock& t2);

#endif  // CLOCK_H
