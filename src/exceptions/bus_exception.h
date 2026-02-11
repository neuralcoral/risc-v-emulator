#ifndef RISC_V_EMULATOR_BUS_EXCEPTION_H
#define RISC_V_EMULATOR_BUS_EXCEPTION_H

#include <exception>
#include <string>

class BusException: public std::exception {
    std::string m_error;
public:
    explicit BusException(std::string_view error) : m_error(error) {}

    const char* what() const noexcept override { return m_error.c_str(); }

};

#endif //RISC_V_EMULATOR_BUS_EXCEPTION_H
