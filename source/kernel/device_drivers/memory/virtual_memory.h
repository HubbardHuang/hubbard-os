#ifndef VIRTUAL_MEMORY_H
#define VIRTUAL_MEMORY_H

#include "type.h"

namespace hubbardos {
namespace kernel {

class VirtualMemory {
public:
    static void InitializeFirstStep(void) __attribute__((section(".init.text")));
    static void InitializeSecondStep(void);
    static void MapTo(uint32_t virtual_address, uint32_t physical_addrress,
                      uint32_t page_table_item_flag);
    static void Unmap(uint32_t virtual_address);
    static uint32_t GetMapping(uint32_t virtual_address);

    static const uint32_t kOffset_ = 0xC0000000;
    static const uint32_t kPageSize_ = 4096;

    static const uint32_t kPagePresent_ = 1 << 0;
    static const uint32_t kPageWrite_ = 1 << 1;
    static const uint32_t kPageUser_ = 1 << 2;

    using pgd_t = uint32_t;
    using pte_t = uint32_t;

private:
    static const uint32_t kPageMask_ = 0xFFFFF000;

    // 页目录包含几项页表首地址
    static const uint32_t kPageDirectorySize_ = kPageSize_ / sizeof(pgd_t);
    // 一个页表包含几项物理页框首地址
    static const uint32_t kPageTableNumber_ = 128;
    static const uint32_t kPageTableSize_ = 1024;

    static pgd_t* const page_directory_temp_ __attribute__((section(".init.data")));
    static pte_t* const page_table_temp_low_ __attribute__((section(".init.data")));
    static pte_t* const page_table_temp_high_ __attribute__((section(".init.data")));

    static pgd_t kernel_page_directory_[kPageDirectorySize_]
      __attribute__((aligned(kPageSize_)));
    static pte_t kernel_page_table_[kPageTableNumber_][kPageTableSize_]
      __attribute__((aligned(kPageSize_)));
};

} // namespace kernel
} // namespace hubbardos

#endif