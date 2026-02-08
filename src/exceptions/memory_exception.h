#ifndef RISC_V_EMULATOR_MEMORY_EXCEPTION_H
#define RISC_V_EMULATOR_MEMORY_EXCEPTION_H

#include "src/exceptions/device_exception.h"

class MemoryException : public DeviceException {
public:
    using DeviceException::DeviceException;
    using DeviceException::what;
};

#endif //RISC_V_EMULATOR_MEMORY_EXCEPTION_H
