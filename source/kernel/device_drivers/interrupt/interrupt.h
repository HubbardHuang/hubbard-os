#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "type.h"

namespace hubbardos {
namespace kernel {

// enum { kInterruptVectorTotal = 256 };

// enum InterruptVectorNumber { kTimer = 32, kPageFault = 14 };

typedef struct SavedMessage {
    uint32_t ds;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t vector_num;
    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t useresp;
    uint32_t ss;
} __attribute__((packed)) SavedMessage;

typedef struct IdtGate {
    uint16_t base_address_low;
    uint16_t selector;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_address_high;
} __attribute__((packed)) IdtGate;

typedef struct IDTR {
    uint16_t limit;
    uint32_t base;

private:
} __attribute__((packed)) IDTR;

// typedef void (*InterruptHandler)(SavedMessage*);

class Interrupt {
public:
    enum Vector { kPageFault = 14, kTimer = 32 };
    static const int kVectorTotal = 256;
    using handler_t = void (*)(SavedMessage*);
    static void Initialize(void);
    static void RegisterHandler(uint8_t vector_num, handler_t new_handler);
    static void Open(void);
    static void Close(void);

private:
    static void InitializeChip8259A(void);
    static void SetGate(uint8_t vector_num, uint32_t entry, uint16_t selector,
                        uint8_t other);
    static void GoToSetIdtr(void);
    static IdtGate gate_[kVectorTotal];
    static IDTR idtr_;
    static bool initialized;
};

} // namespace kernel
} // namespace hubbardos

#endif