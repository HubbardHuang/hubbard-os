#include "virtual_memory.h"
#include "console.h"
#include "interrupt.h"
#include "multiboot.h"

namespace hubbardos {
namespace kernel {

#define GetPgdIndex(v) (((v) >> 22) & 0x3FF)

multiboot_t* multiboot;

VirtualMemory::pgd_t* const VirtualMemory::page_directory_temp =
  (VirtualMemory::pgd_t*)0x1000;
VirtualMemory::pte_t* const VirtualMemory::page_table_temp_low =
  (VirtualMemory::pte_t*)0x2000;
VirtualMemory::pte_t* const VirtualMemory::page_table_temp_high =
  (VirtualMemory::pte_t*)0x3000;

__attribute__((aligned(4096))) VirtualMemory::pgd_t
  VirtualMemory::kernel_page_directory[VirtualMemory::kPageDirectorySize_];
__attribute__((aligned(4096)))
VirtualMemory::pte_t VirtualMemory::kernel_page_table[VirtualMemory::kPageTableSize_]
                                                     [VirtualMemory::kPageTableItemSize_];

static inline void
SwitchPageDirectory(uint32_t page_directory_address) {
    asm volatile("mov %0, %%cr3" : : "r"(page_directory_address));
}

void
PageFault(SavedMessage* message) {
    Console::SubInstance().PrintFormatted("page fault!\n");
}

void
VirtualMemory::InitializeFirstStep(void) {
    page_directory_temp[GetPgdIndex(0)] =
      (pgd_t)page_table_temp_low | kPagePresent_ | kPageWrite_;
    page_directory_temp[GetPgdIndex(kOffset_)] =
      (pgd_t)page_table_temp_high | kPagePresent_ | kPageWrite_;

    for (size_t i = 0; i < kPageTableItemSize_; i++) {
        page_table_temp_low[i] = (pte_t)i << 12 | kPagePresent_ | kPageWrite_;
    }
    for (size_t i = 0; i < kPageTableItemSize_; i++) {
        page_table_temp_high[i] = (pte_t)i << 12 | kPagePresent_ | kPageWrite_;
    }

    asm volatile("mov %0, %%cr3" : : "r"(page_directory_temp));

    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 1 << 31;
    asm volatile("mov %0, %%cr0" : : "r"(cr0));

    multiboot = multiboot_temp + kOffset_;
}

void
VirtualMemory::InitializeSecondStep(void) {
    size_t pgd_1st_index = GetPgdIndex(kOffset_);
    for (size_t i = pgd_1st_index; i < kPageTableSize_ + pgd_1st_index; i++) {
        kernel_page_directory[i] =
          (((pgd_t)kernel_page_table[i - pgd_1st_index] - kOffset_) | kPagePresent_ |
           kPageWrite_);
    }
    uint32_t* pte = (uint32_t*)kernel_page_table;
    for (size_t i = 1; i < kPageTableSize_ * kPageTableItemSize_; i++) {
        pte[i] = (i << 12) | kPagePresent_ | kPageWrite_;
    }

    Interrupt::RegisterHandler(14, &PageFault);

    SwitchPageDirectory((uint32_t)kernel_page_directory - kOffset_);
}

} // namespace kernel
} // namespace hubbardos
