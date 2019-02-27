#include "physical_memory.h"
#include "console.h"

namespace hubbardos {
namespace kernel {

extern "C" {
extern uint8_t start_address[];
extern uint8_t end_address[];
}

uint32_t PhysialMemory::kernel_start_address_;
uint32_t PhysialMemory::kernel_end_address_;
uint32_t PhysialMemory::page_stack_[PhysialMemory::kPageMaxNumber_] = { 2 };
size_t PhysialMemory::stack_top_;
bool PhysialMemory::initialized_ = false;
uint32_t PhysialMemory::page_total_;

void
PhysialMemory::Initialize(void) {
    if (initialized_ == true) {
        return;
    }

    kernel_start_address_ = (uint32_t)start_address;
    kernel_end_address_ = (uint32_t)end_address;

    mmap_entry_t* map_start_address = (mmap_entry_t*)multiboot->mmap_addr;
    mmap_entry_t* map_end_address =
      (mmap_entry_t*)multiboot->mmap_addr + multiboot->mmap_length;

    for (mmap_entry_t* map_entry = map_start_address; map_entry < map_end_address;
         map_entry++) {
        if (map_entry->type == 1 && map_entry->base_addr_low == 0x100000) {
            uint32_t page =
              map_entry->base_addr_low + (kernel_end_address_ - kernel_start_address_);
            uint32_t last_page = map_entry->base_addr_low + map_entry->length_low;
            while (page < last_page && page <= kMaxSize_) {
                FreePage(page);
                page += kPageSize_;
                ++page_total_;
            }
        }
    }

    initialized_ = true;
}

uint32_t
PhysialMemory::AllocatePage(void) {
    uint32_t page = page_stack_[stack_top_--];
    return page;
}

void
PhysialMemory::FreePage(uint32_t page) {
    page_stack_[++stack_top_] = page;
}

void
PhysialMemory::ShowMap(void) {
    uint32_t map_entry = multiboot->mmap_addr;
    uint32_t map_length = multiboot->mmap_length;

    for (mmap_entry_t* mmap = (mmap_entry_t*)map_entry;
         (uint32_t)mmap < map_entry + map_length; mmap++) {
        Console::SubInstance().PrintFormatted(
          "base address: 0x%x%x, length = 0x%x%x, type = %d.\n", mmap->base_addr_high,
          mmap->base_addr_low, mmap->length_high, mmap->length_low, mmap->type);
    }

    Console::SubInstance().PrintFormatted(
      "start: %x, end: %x, page total: %u(remainder = %d)\n", kernel_start_address_,
      kernel_end_address_, page_total_,
      (kernel_end_address_ - kernel_start_address_) % kPageSize_);
}

uint32_t
PhysialMemory::GetPageTotal(void) {
    return page_total_;
}

} // namespace kernel
} // namespace hubbardos
