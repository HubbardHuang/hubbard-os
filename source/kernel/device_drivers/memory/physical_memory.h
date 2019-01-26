#ifndef PHYSICAL_MEMORY_H
#define PHYSICAL_MEMORY_H

#include "multiboot.h"
#include "type.h"

namespace hubbardos {
namespace kernel {

/*
class PhysicalMemory {
private:
    uint32_t start_address_;
    uint32_t end_address_;
    uint32_t page_total_;
    PhysicalMemory();
    PhysicalMemory(const PhysicalMemory&);
    PhysicalMemory& operator=(const PhysicalMemory&);

public:
    static const uint32_t kPageSize = 1024;
    static PhysicalMemory& Instance(void);
    void GetStartAddress(void) const;
    void GetEndAddress(void) const;
    void GetPageTotao(void) const;
    void ShowMap(void);
};
*/

class PhysialMemory {
public:
    static const size_t kPageSize_ = 4096;
    static const size_t kThreadStackSize_ = 4 * kPageSize_;
    static const size_t kMaxSize_ = 1 << 28 /*0x20000000*/; // 512MB
    static const size_t kPageMaxNumber_ = kMaxSize_ / kPageSize_;
    static const size_t kPageMask_ = 0xFFFFF000; // 4096
    static void Initialize(void);
    static uint32_t AllocatePage(void);
    static void FreePage(uint32_t page);
    static void ShowMap(void);
    static uint32_t GetPageTotal(void);

private:
    static uint32_t kernel_start_address_;
    static uint32_t kernel_end_address_;
    static bool initialized_;

    static uint32_t page_stack_[kPageMaxNumber_];
    static size_t stack_top_;
    static uint32_t page_total_;
};

} // namespace kernel
} // namespace hubbardos

#endif