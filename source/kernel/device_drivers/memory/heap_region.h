#ifndef HEAP_REGION_H
#define HEAP_REGION_H

#include "type.h"

namespace hubbardos {
namespace kernel {

typedef struct HeapList HeapList;

class HeapRegion {
public:
    static void Initialize(void);
    static void* Allocate(size_t size);
    static void Free(void* p);
    static const uint32_t kStartAddress_ = 0xE0000000;
    static const uint32_t kEndAddress_ = 0xE0010000;

private:
    static HeapList* head_;
    static uint32_t top_;
    static void AllocateChunk(uint32_t start_address, size_t length);
    static void FreeChunk(HeapList* node);
    static void SplitChunk(HeapList* node, size_t length);
    static void GlueChunk(HeapList* node);
};

} // namespace kernel
} // namespace hubbardos

#endif
