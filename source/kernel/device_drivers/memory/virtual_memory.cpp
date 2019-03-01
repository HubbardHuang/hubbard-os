#include "virtual_memory.h"
#include "array.h"
#include "console.h"
#include "interrupt.h"
#include "multiboot.h"
#include "physical_memory.h"

namespace hubbardos {
namespace kernel {

#define GetPageDirectoryIndex(v) (((v) >> 22) & 0x3FF)
#define GetPageTableIndex(v) (((v) >> 12) & 0x3FF)

multiboot_t* multiboot;

VirtualMemory::pgd_t* const VirtualMemory::page_directory_temp_ =
  (VirtualMemory::pgd_t*)0x1000;
VirtualMemory::pte_t* const VirtualMemory::page_table_temp_low_ =
  (VirtualMemory::pte_t*)0x2000;
VirtualMemory::pte_t* const VirtualMemory::page_table_temp_high_ =
  (VirtualMemory::pte_t*)0x3000;

__attribute__((aligned(4096))) VirtualMemory::pgd_t
  VirtualMemory::kernel_page_directory_[VirtualMemory::kPageDirectorySize_];
__attribute__((aligned(4096)))
VirtualMemory::pte_t VirtualMemory::kernel_page_table_[VirtualMemory::kPageTableNumber_]
                                                      [VirtualMemory::kPageTableSize_];

static inline void
SwitchPageDirectory(uint32_t page_directory_address) {
    asm volatile("mov %0, %%cr3" : : "r"(page_directory_address));
}

static inline void
UpdateTlb(uint32_t virtual_address) {
    asm volatile("invlpg (%0)" : : "a"(virtual_address));
}

void
PageFault(SavedMessage* message) {
    Console::SubInstance().PrintFormatted("page fault!\n");
    while (1) {
        ;
    }
}

void
VirtualMemory::InitializeFirstStep(void) {
    page_directory_temp_[GetPageDirectoryIndex(0)] =
      reinterpret_cast<pgd_t>(page_table_temp_low_) | kPagePresent_ | kPageWrite_;
    page_directory_temp_[GetPageDirectoryIndex(kOffset_)] =
      reinterpret_cast<pgd_t>(page_table_temp_high_) | kPagePresent_ | kPageWrite_;

    // 页表存储的是物理页的首地址，所有物理页的首地址都按 4K 对齐
    for (size_t i = 0; i < kPageDirectorySize_; i++) {
        page_table_temp_low_[i] =
          static_cast<pte_t>(i << 12 | kPagePresent_ | kPageWrite_);
    }
    for (size_t i = 0; i < kPageDirectorySize_; i++) {
        page_table_temp_high_[i] =
          static_cast<pte_t>(i << 12 | kPagePresent_ | kPageWrite_);
    }
    // 加载临时页目录
    asm volatile("mov %0, %%cr3" : : "r"(page_directory_temp_));

    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 1 << 31;
    asm volatile("mov %0, %%cr0" : : "r"(cr0));

    multiboot = multiboot_temp + kOffset_;
}

void
VirtualMemory::InitializeSecondStep(void) {
    size_t pgd_1st_index = GetPageDirectoryIndex(kOffset_);
    for (size_t i = pgd_1st_index; i < kPageTableNumber_ + pgd_1st_index; i++) {
        kernel_page_directory_[i] = static_cast<pgd_t>(
          (reinterpret_cast<uint32_t>(kernel_page_table_[i - pgd_1st_index]) - kOffset_) |
          kPagePresent_ | kPageWrite_);
    }
    uint32_t* pte = reinterpret_cast<uint32_t*>(kernel_page_table_);
    for (size_t i = 1; i < kPageTableNumber_ * kPageTableSize_; i++) {
        pte[i] = (i << 12) | kPagePresent_ | kPageWrite_;
    }

    Interrupt::RegisterHandler(Interrupt::Vector::kPageFault, &PageFault);
    uint32_t kpd_physical_address =
      reinterpret_cast<uint32_t>(kernel_page_directory_) - kOffset_;
    SwitchPageDirectory(kpd_physical_address);
}

void
VirtualMemory::MapTo(uint32_t virtual_address, uint32_t physical_address,
                     uint32_t page_table_item_flag) {
    uint32_t page_directory_index = GetPageDirectoryIndex(virtual_address);
    uint32_t page_table_index = GetPageTableIndex(virtual_address);

    pte_t* current_page_table =
      reinterpret_cast<pte_t*>(kernel_page_directory_[page_directory_index] & kPageMask_);
    // 如果页表为空，需要分配一个物理页给页表作存储用
    if (!current_page_table) {
        current_page_table = reinterpret_cast<pte_t*>(PhysialMemory::AllocatePage());
        kernel_page_directory_[page_directory_index] =
          reinterpret_cast<pgd_t>(current_page_table) | kPagePresent_ | kPageWrite_;
        current_page_table = reinterpret_cast<pte_t*>(
          reinterpret_cast<uint32_t>(current_page_table) + kOffset_);

        for (uint32_t i = 0; i < kPageTableSize_; i++) {
            current_page_table[i] = static_cast<pte_t>(0);
        }
    } else {
        current_page_table = reinterpret_cast<pte_t*>(
          reinterpret_cast<uint32_t>(current_page_table) + kOffset_);
    }
    current_page_table[page_table_index] =
      (reinterpret_cast<uint32_t>(physical_address) & kPageMask_) | page_table_item_flag;
    UpdateTlb(virtual_address);
}

void
VirtualMemory::Unmap(uint32_t virtual_address) {
    uint32_t page_directory_index =
      GetPageDirectoryIndex(reinterpret_cast<uint32_t>(virtual_address));
    uint32_t page_table_index =
      GetPageTableIndex(reinterpret_cast<uint32_t>(virtual_address));

    pte_t* current_page_table =
      reinterpret_cast<pte_t*>(kernel_page_directory_[page_directory_index] & kPageMask_);
    if (!current_page_table) {
        return;
    }
    current_page_table =
      reinterpret_cast<pte_t*>(reinterpret_cast<uint32_t>(current_page_table) + kOffset_);
    current_page_table[page_table_index] = 0;
    UpdateTlb(virtual_address);
}

uint32_t
VirtualMemory::GetMapping(uint32_t virtual_address) {
    uint32_t page_directory_index =
      GetPageDirectoryIndex(reinterpret_cast<uint32_t>(virtual_address));
    uint32_t page_table_index =
      GetPageTableIndex(reinterpret_cast<uint32_t>(virtual_address));

    pte_t* current_page_table =
      reinterpret_cast<pte_t*>(kernel_page_directory_[page_directory_index] & kPageMask_);
    current_page_table =
      reinterpret_cast<pte_t*>(reinterpret_cast<uint32_t>(current_page_table) + kOffset_);
    // 返回 0 表示不存在
    if (!current_page_table[page_table_index]) {
        return 0;
    }
    return reinterpret_cast<uint32_t>(current_page_table[page_table_index]);
}

} // namespace kernel
} // namespace hubbardos
