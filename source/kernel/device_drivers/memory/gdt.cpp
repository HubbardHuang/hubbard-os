#include "gdt.h"

extern "C" {
extern void LoadGdtr(void* GDTR);
}

SegmentDescriptor ProtectedMode::gdt_[kSegmentDescriptorTotal];
GDTR ProtectedMode::gdtr_;
bool ProtectedMode::initialized = false;

void
ProtectedMode::Initialize(void) {
    if (initialized == true) {
        return;
    }

    gdtr_.limit = sizeof(SegmentDescriptor) * kSegmentDescriptorTotal - 1;
    gdtr_.base_address = (uint32_t)&gdt_;

    SetSegmentDescriptor(0, 0, 0, 0, 0);

    // kernel code segment
    SetSegmentDescriptor(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    // kernel date segment
    SetSegmentDescriptor(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    // uer code segment
    SetSegmentDescriptor(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    // user data segment
    SetSegmentDescriptor(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    SetGdtr(&gdtr_);

    initialized = true;
}

void
ProtectedMode::SetSegmentDescriptor(int index, uint32_t base_address,
                                    uint32_t segment_limit, uint8_t access,
                                    uint8_t g_db_l_avl) {
    gdt_[index].base_address_low = (base_address & 0xFFFF);
    gdt_[index].base_address_middle = (base_address >> 16) & 0xFF;
    gdt_[index].base_address_high = (base_address >> 24) & 0xFF;
    gdt_[index].segment_limit_low = (segment_limit & 0xFFFF);
    gdt_[index].segment_limit_mid_etc = (segment_limit >> 16) & 0x0F;

    gdt_[index].segment_limit_mid_etc |= g_db_l_avl & 0xF0;
    gdt_[index].access = access;
}

void
ProtectedMode::SetGdtr(GDTR* gdtr) {
    LoadGdtr(gdtr);
}