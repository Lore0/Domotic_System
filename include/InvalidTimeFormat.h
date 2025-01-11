#ifndef INVALIDTIMEFORMAT_H
#define INVALIDTIMEFORMAT_H

#include <stdexcept>
#include <string>

class InvalidTimeFormat : public std::exception{
    private:
        std::string message_;
    public:
        InvalidTimeFormat() : message_ ("Formato errato: deve essere hh:mm") {}
        explicit InvalidTimeFormat(const std::string& message) : message_(message) {}
        const char* what() const noexcept override{ return message_.c_str(); }
};



#endif // INVALIDTIMEFORMAT_H