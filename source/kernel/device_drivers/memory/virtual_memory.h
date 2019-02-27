#ifndef VIRTUAL_MEMORY_H
#define VIRTUAL_MEMORY_H

#include "type.h"

namespace hubbardos {
namespace kernel {

class VirtualMemory {
public:
    static void InitializeSecondStep(void);
    static void InitializeFirstStep(void) __attribute__((section(".init.text")));
    static const uint32_t kOffset_ = 0xC0000000;
    static const uint32_t kPageSize_ = 4096;
    using pgd_t = uint32_t;
    using pte_t = uint32_t;

private:
    static const uint32_t kPageMask_ = 0xFFFFF000;

    static const uint32_t kPagePresent_ = 1 << 0;
    static const uint32_t kPageWrite_ = 1 << 1;
    static const uint32_t kPageUser_ = 1 << 2;

    static const uint32_t kPageDirectorySize_ = kPageSize_ / sizeof(pgd_t);
    static const uint32_t kPageTableItemSize_ = kPageSize_ / sizeof(pte_t);
    static const uint32_t kPageTableSize_ = 128;

    static pgd_t* const page_directory_temp __attribute__((section(".init.data")));
    static pte_t* const page_table_temp_low __attribute__((section(".init.data")));
    static pte_t* const page_table_temp_high __attribute__((section(".init.data")));

    static pgd_t kernel_page_directory[kPageDirectorySize_]
      __attribute__((aligned(4096)));
    static pte_t kernel_page_table[kPageTableSize_][kPageTableItemSize_]
      __attribute__((aligned(4096)));
};

} // namespace kernel
} // namespace hubbardos

#endif