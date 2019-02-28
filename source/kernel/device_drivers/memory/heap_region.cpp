#include "heap_region.h"
#include "console.h"
#include "physical_memory.h"
#include "virtual_memory.h"

namespace hubbardos {
namespace kernel {

struct HeapList {
    HeapList* prev;
    HeapList* next;
    bool allocated;
    uint32_t length; // 包含该结构体在内的内存块总长度
};

HeapList* HeapRegion::head_;
uint32_t HeapRegion::top_ = HeapRegion::kStartAddress_;

void
HeapRegion::Initialize(void) {
    head_ = nullptr;
}

void*
HeapRegion::Allocate(size_t size) {
    size += sizeof(HeapList);
    HeapList* curr_node = head_;
    HeapList* prev_node = nullptr;
    while (curr_node) {
        if (curr_node->length >= size && !curr_node->allocated) {
            SplitChunk(curr_node, size);
            curr_node->allocated = true;
            return reinterpret_cast<void*>(reinterpret_cast<uint32_t>(curr_node) +
                                           sizeof(HeapList));
        }
        prev_node = curr_node;
        curr_node = curr_node->next;
    }

    uint32_t new_chunk = 0;

    if (prev_node) {
        new_chunk = prev_node->length + reinterpret_cast<uint32_t>(prev_node);
    } else {
        new_chunk = kStartAddress_;
        head_ = reinterpret_cast<HeapList*>(new_chunk);
    }
    AllocateChunk(new_chunk, size);

    curr_node = reinterpret_cast<HeapList*>(new_chunk);
    curr_node->prev = prev_node;
    curr_node->next = nullptr;
    curr_node->allocated = true;
    curr_node->length = size;
    if (prev_node) {
        prev_node->next = curr_node;
    }
    return reinterpret_cast<void*>(new_chunk + sizeof(HeapList));
}

void
HeapRegion::Free(void* p) {
    HeapList* curr_node =
      reinterpret_cast<HeapList*>(reinterpret_cast<uint32_t>(p) - sizeof(HeapList));
    curr_node->allocated = false;
    GlueChunk(curr_node);
}

void
HeapRegion::AllocateChunk(uint32_t start_address, size_t size) {
    while (start_address + size > top_) {
        uint32_t new_page = PhysialMemory::AllocatePage();
        VirtualMemory::MapTo(top_, new_page,
                             VirtualMemory::kPagePresent_ | VirtualMemory::kPageWrite_);
        top_ += VirtualMemory::kPageSize_;
    }
}

void
HeapRegion::FreeChunk(HeapList* node) {
    if (!node || !node->prev) {
        head_ = nullptr;
    } else {
        node->prev->next = nullptr;
    }

    while (top_ - VirtualMemory::kPageSize_ >= reinterpret_cast<uint32_t>(node)) {
        top_ -= VirtualMemory::kPageSize_;
        uint32_t page = VirtualMemory::GetMapping(top_);
        VirtualMemory::Unmap(top_);
        PhysialMemory::FreePage(page);
    }
}

void
HeapRegion::SplitChunk(HeapList* node, size_t size) {
    if (!node || node->length - size <= sizeof(HeapList)) {
        return;
    }
    HeapList* new_chunk =
      reinterpret_cast<HeapList*>(reinterpret_cast<size_t>(node) + size);
    new_chunk->allocated = false;
    new_chunk->next = node->next;
    new_chunk->prev = node;
    new_chunk->length = node->length - size;
    node->next = new_chunk;
    node->length = size;
}

void
HeapRegion::GlueChunk(HeapList* node) {
    if (!node) {
        return;
    }
    if (node->next && !node->next->allocated) {
        node->length = node->length + node->next->length;
        if (node->next->next) {
            node->next->next->prev = node;
        }
        node->next = node->next->next;
    }
    if (node->prev && !node->prev->allocated) {
        node->prev->length = node->prev->length + node->length;
        node->prev->next = node->next;
        if (node->next) {
            node->next->prev = node->prev;
        }
        node = node->prev;
    }
    if (!node->next) {
        FreeChunk(node);
    }
}

} // namespace kernel
} // namespace hubbardos