#include "array.h"
#include "console.h"
#include "gdt.h"
#include "interrupt.h"
#include "multiboot.h"
#include "physical_memory.h"
#include "timer.h"
#include "type.h"
#include "virtual_memory.h"

using namespace hubbardos::interface;

namespace hubbardos {
namespace kernel {

char kernel_stack[PhysialMemory::kThreadStackSize_];
int HubbardOsKernel(void);

extern "C" {
__attribute__((section(".init.text"))) void
PrepareForHubbardOsKernel(void) {
    VirtualMemory::InitializeFirstStep();
    uint32_t kernel_stack_top =
      (reinterpret_cast<uint32_t>(kernel_stack) + PhysialMemory::kThreadStackSize_) &
      0xFFFFFFF0;
    asm volatile("mov %0, %%esp\n\t"
                 "xor %%ebp, %%ebp"
                 :
                 : "r"(kernel_stack_top));
    HubbardOsKernel();
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

    Console::Instance().DrawTitle("Welcome to HubbardOS Kernel!");

    Console::SubInstance().PrintFormatted("page number: %u.\n",
                                          PhysialMemory::GetPageTotal());
    uint32_t page = PhysialMemory::AllocatePage();
    Console::SubInstance().PrintFormatted("page = 0x%x\n", page);
    page = PhysialMemory::AllocatePage();
    Console::SubInstance().PrintFormatted("page = 0x%x\n", page);
    page = PhysialMemory::AllocatePage();
    Console::SubInstance().PrintFormatted("page = 0x%x\n", page);
    Timer::Instance().StartWorking();
    Interrupt::Close();
    uint32_t p = reinterpret_cast<uint32_t>(VirtualMemory::GetMapping(&p));
    Console::SubInstance().PrintFormatted("&p = 0x%x.\n", p);

    while (1) {
        asm volatile("hlt");
    }
    return 0;
}

} // namespace kernel
} // namespace hubbardos