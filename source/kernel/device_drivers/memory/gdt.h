#ifndef GDT_H
#define GDT_H

#include "type.h"

enum { kSegmentDescriptorTotal = 5 };

typedef struct SegmentDescriptor {
    uint16_t segment_limit_low;
    uint16_t base_address_low;
    uint8_t base_address_middle;
    uint8_t access;
    uint8_t segment_limit_mid_etc;
    uint8_t base_address_high;
} __attribute__((packed)) SegmentDescriptor;

typedef struct GDTR {
    uint16_t limit;
    uint32_t base_address;
} __attribute__((packed)) GDTR;

class ProtectedMode {
public:
    static void Initialize(void);

private:
    static SegmentDescriptor gdt_[kSegmentDescriptorTotal];
    static GDTR gdtr_;
    static void SetSegmentDescriptor(int index, uint32_t base_address,
                                     uint32_t segment_limit, uint8_t access,
                                     uint8_t g_db_l_avl);
    static void SetGdtr(GDTR* gdtr);
    static bool initialized;
};

#endif