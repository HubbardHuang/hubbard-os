#ifndef IO_PORT_H
#define IO_PORT_H

#include "type.h"

namespace hubbardos {
namespace kernel {

class IoPort {
public:
    static IoPort& Instance() {
        static IoPort singleton;
        return singleton;
    }

    inline void WriteByte(uint16_t port, uint8_t value) const {
        asm volatile("outb %1, %0" ::"dN"(port), "a"(value));
    }

    inline uint8_t ReadByte(uint16_t port) const {
        uint8_t value;
        asm volatile("inb %1, %0" : "=a"(value) : "dN"(port));
        return value;
    }

    inline uint16_t ReadWord(uint16_t port) const {
        uint16_t value;
        asm volatile("inw %1, %0" : "=a"(value) : "dN"(port));
        return value;
    }

private:
    IoPort(){};
    IoPort(const IoPort&);
    IoPort& operator=(const IoPort&);
    //~IoPort(){}; // 将析构函数设为私有就会报错
};
} // namespace kernel
} // namespace hubbardos

#endif