#ifndef RISC_V_EMULATOR_REGISTER_FILE_EXCEPTION_H
#define RISC_V_EMULATOR_REGISTER_FILE_EXCEPTION_H

#include <string>
#include <string_view>
#include <exception>

class RegisterFileException : public std::exception {
private:
    std::string m_error;
public:
    explicit RegisterFileException(std::string_view error) : m_error{ error } { }

    const char* what() const noexcept override { return m_error.c_str(); }
};


#endif //RISC_V_EMULATOR_REGISTER_FILE_EXCEPTION_H
