#ifndef RISC_V_EMULATOR_DEVICE_EXCEPTION_H
#define RISC_V_EMULATOR_DEVICE_EXCEPTION_H

#include <exception>
#include <string>

class DeviceException: public std::exception {
    std::string m_error;
public:
    explicit DeviceException(std::string_view error) : m_error{error} {}

    const char* what() const noexcept override { return m_error.c_str(); }
};

#endif //RISC_V_EMULATOR_DEVICE_EXCEPTION_H
