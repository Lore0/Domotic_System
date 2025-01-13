//COMPAGNO FEDERICO

#ifndef INVALIDTIMEFORMAT_H
#define INVALIDTIMEFORMAT_H

#include <stdexcept>
#include <string>

// InvalidTimeFormat class to handle exceptions for invalid time format
class InvalidTimeFormat : public std::exception {
 private:
  std::string message_;  // Stores the error message

 public:
  // Default constructor with a predefined error message
  InvalidTimeFormat() : message_("Formato errato: deve essere hh:mm") {}

  // Constructor that initializes the error message with a custom message
  explicit InvalidTimeFormat(const std::string& message) : message_(message) {}

  // Returns the error message
  const char* what() const noexcept override { return message_.c_str(); }
};

#endif  // INVALIDTIMEFORMAT_H