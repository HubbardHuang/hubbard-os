#include "array.h"
#include "console.h"
#include "gdt.h"
#include "heap_region.h"
#include "interrupt.h"
#include "multiboot.h"
#include "physical_memory.h"
#include "thread.h"
#include "timer.h"
#include "type.h"
#include "virtual_memory.h"

using namespace hubbardos::interface;

namespace hubbardos {
namespace kernel {

char kernel_stack[PhysialMemory::kThreadStackSize_] __attribute__((aligned(16)));
int HubbardOsKernel(void);

extern "C" {
__attribute__((section(".init.text"))) void
PrepareForHubbardOsKernel(void) {
    VirtualMemory::InitializeFirstStep();
    uint32_t kernel_stack_top =
      (reinterpret_cast<uint32_t>(kernel_stack) + PhysialMemory::kThreadStackSize_);
    asm volatile("mov %0, %%esp\n\t"
                 "xor %%ebp, %%ebp"
                 :
                 : "r"(kernel_stack_top));
    HubbardOsKernel();
}
}

void
TestHeap() {
    void* a1 = HeapRegion::Allocate(50);
    Console::SubInstance().PrintFormatted("malloc 50: 0x%x\n", a1);
    void* a2 = HeapRegion::Allocate(500);
    Console::SubInstance().PrintFormatted("malloc 500: 0x%x\n", a2);
    void* a3 = HeapRegion::Allocate(5000);
    Console::SubInstance().PrintFormatted("malloc 5000: 0x%x\n", a3);

    Console::SubInstance().PrintFormatted("free 50: 0x%x\n", a1);
    HeapRegion::Free(a1);
    Console::SubInstance().PrintFormatted("free 500: 0x%x\n", a2);
    HeapRegion::Free(a2);
    Console::SubInstance().PrintFormatted("free 5000: 0x%x\n", a3);
    HeapRegion::Free(a3);
}

int flag = 1;
void
PrintA(void* arg) {
    uint32_t start = Timer::Instance().GetClick();

    while (1) {
        uint32_t end = Timer::Instance().GetClick();
        if (1)
            Console::SubInstance().PrintFormatted("A"), flag ^= 1;
        if (end - start >= Thread::kCommonTimeSlice) {
            Thread::Schedule();
        }
    }
}
void
PrintB(void* arg) {
    uint32_t start = Timer::Instance().GetClick();

    while (1) {
        uint32_t end = Timer::Instance().GetClick();
        if (1)
            Console::SubInstance().PrintFormatted("B"), flag ^= 1;
        if (end - start >= Thread::kCommonTimeSlice) {
            Thread::Schedule();
        }
    }
}

int
HubbardOsKernel(void) {
    ProtectedMode::Initialize();
    Console::Instance().Clear();
    Interrupt::Initialize();
    PhysialMemory::Initialize();
    PhysialMemory::ShowMap();
    VirtualMemory::InitializeSecondStep(); // 顺序不能更改
    HeapRegion::Initialize();
    Thread::InitializeScheduling();

    Console::Instance().DrawTitle("Welcome to HubbardOS Kernel!");
    Console::SubInstance().PrintFormatted("page number: %u.\n",
                                          PhysialMemory::GetPageTotal());

    Timer::Instance().StartWorking();
    uint32_t page = PhysialMemory::AllocatePage();
    Console::SubInstance().PrintFormatted("page = 0x%x\n", page);
    page = PhysialMemory::AllocatePage();
    Console::SubInstance().PrintFormatted("page = 0x%x\n", page);
    page = PhysialMemory::AllocatePage();
    Console::SubInstance().PrintFormatted("page = 0x%x\n", page);

    TestHeap();
    Thread::id_type id1 = Thread::Create(PrintA, nullptr);
    Console::SubInstance().PrintFormatted("thread %d has been created.\n", id1);
    Thread::id_type id2 = Thread::Create(PrintB, nullptr);
    Console::SubInstance().PrintFormatted("thread %d has been created.\n", id2);

    uint32_t start = Timer::Instance().GetClick();
    while (1) {
        if (1)
            Console::SubInstance().PrintFormatted("C"), flag ^= 1;
        uint32_t end = Timer::Instance().GetClick();
        if (end - start >= Thread::kCommonTimeSlice) {
            Thread::Schedule();
        }
    }

    while (1) {
        asm volatile("hlt");
    }
    return 0;
}

} // namespace kernel
} // namespace hubbardos