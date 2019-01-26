#include "array.h"
#include "console.h"
#include "console_test.h"
#include "gdt.h"
#include "interrupt.h"
#include "multiboot.h"
#include "physical_memory.h"
#include "timer.h"
#include "type.h"

using namespace hubbardos::interface;

namespace hubbardos {
namespace kernel {

extern "C" int
HubbardOsKernel(void) {
    ProtectedMode::Initialize();
    Interrupt::Initialize();
    Console::Instance().Clear();
    Console::Instance().DrawTitle("Welcome to HubbardOS Kernel!");
    // Timer::Instance().StartWorking();
    Interrupt::Close();

    asm volatile("int $3");
    PhysialMemory::Initialize();
    PhysialMemory::ShowMap();

    Console::SubInstance().PrintFormatted("page number: %u.\n",
                                          PhysialMemory::GetPageTotal());
    uint32_t page = PhysialMemory::AllocatePage();
    Console::SubInstance().PrintFormatted("page = 0x%x\n", page);
    page = PhysialMemory::AllocatePage();
    Console::SubInstance().PrintFormatted("page = 0x%x\n", page);
    page = PhysialMemory::AllocatePage();
    Console::SubInstance().PrintFormatted("page = 0x%x\n", page);

    return 0;
}

} // namespace kernel
} // namespace hubbardos