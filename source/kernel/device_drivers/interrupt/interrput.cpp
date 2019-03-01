#include "array.h"
#include "console.h"
#include "interrupt.h"
#include "io_port.h"

namespace hubbardos {
namespace kernel {

using namespace interface;

#ifdef __cplusplus
extern "C" {
#endif

void SetIdtr(void* idtr);
void Exception0(void);
void Exception1(void);
void Exception2(void);
void Exception3(void);
void Exception4(void);
void Exception5(void);
void Exception6(void);
void Exception7(void);
void Exception8(void);
void Exception9(void);
void Exception10(void);
void Exception11(void);
void Exception12(void);
void Exception13(void);
void Exception14(void);
void Exception15(void);
void Exception16(void);
void Exception17(void);
void Exception18(void);
void Exception19(void);
void Exception20(void);
void Exception21(void);
void Exception22(void);
void Exception23(void);
void Exception24(void);
void Exception25(void);
void Exception26(void);
void Exception27(void);
void Exception28(void);
void Exception29(void);
void Exception30(void);
void Exception31(void);

void Request0(void);
void Request1(void);
void Request2(void);
void Request3(void);
void Request4(void);
void Request5(void);
void Request6(void);
void Request7(void);
void Request8(void);
void Request9(void);
void Request10(void);
void Request11(void);
void Request12(void);
void Request13(void);
void Request14(void);
void Request15(void);

#ifdef __cplusplus
};
#endif

IdtGate Interrupt::gate_[Interrupt::kVectorTotal];
IDTR Interrupt::idtr_;
bool Interrupt::initialized = false;

static Interrupt::handler_t handler_[Interrupt::kVectorTotal];

void
Interrupt::InitializeChip8259A(void) {
    IoPort::Instance().WriteByte(0x20, 0x11);
    IoPort::Instance().WriteByte(0xA0, 0x11);

    // 设置主片 IRQ 从 0x20(32) 号中断开始
    IoPort::Instance().WriteByte(0x21, 0x20);

    // 设置从片 IRQ 从 0x28(40) 号中断开始
    IoPort::Instance().WriteByte(0xA1, 0x28);

    // 设置主片 IR2 引脚连接从片
    IoPort::Instance().WriteByte(0x21, 0x04);

    // 告诉从片输出引脚和主片 IR2 号相连
    IoPort::Instance().WriteByte(0xA1, 0x02);

    // 设置主片和从片按照 8086 的方式工作
    IoPort::Instance().WriteByte(0x21, 0x01);
    IoPort::Instance().WriteByte(0xA1, 0x01);

    // 设置主从片允许中断
    IoPort::Instance().WriteByte(0x21, 0x0);
    IoPort::Instance().WriteByte(0xA1, 0x0);
}

void
Interrupt::SetGate(uint8_t vector_num, uint32_t entry, uint16_t selector, uint8_t other) {
    gate_[vector_num].base_address_low = entry & 0xFFFF;
    gate_[vector_num].base_address_high = (entry >> 16) & 0xFFFF;

    gate_[vector_num].selector = selector;
    gate_[vector_num].always0 = 0;
    gate_[vector_num].flags = other;
}

void
Interrupt::GoToSetIdtr(void) {
    SetIdtr(&idtr_);
}

void
Interrupt::Initialize(void) {
    if (initialized == true) {
        return;
    }

    InitializeChip8259A();

    SetMember(handler_, sizeof(handler_t) * kVectorTotal, 0);
    idtr_.limit = sizeof(IdtGate) * kVectorTotal - 1;
    idtr_.base = reinterpret_cast<uint32_t>(&gate_);

    SetGate(0, reinterpret_cast<uint32_t>(Exception0), 0x08, 0x8E);
    SetGate(1, reinterpret_cast<uint32_t>(Exception1), 0x08, 0x8E);
    SetGate(2, reinterpret_cast<uint32_t>(Exception2), 0x08, 0x8E);
    SetGate(3, reinterpret_cast<uint32_t>(Exception3), 0x08, 0x8E);
    SetGate(4, reinterpret_cast<uint32_t>(Exception4), 0x08, 0x8E);
    SetGate(5, reinterpret_cast<uint32_t>(Exception5), 0x08, 0x8E);
    SetGate(6, reinterpret_cast<uint32_t>(Exception6), 0x08, 0x8E);
    SetGate(7, reinterpret_cast<uint32_t>(Exception7), 0x08, 0x8E);
    SetGate(8, reinterpret_cast<uint32_t>(Exception8), 0x08, 0x8E);
    SetGate(9, reinterpret_cast<uint32_t>(Exception9), 0x08, 0x8E);
    SetGate(10, reinterpret_cast<uint32_t>(Exception10), 0x08, 0x8E);
    SetGate(11, reinterpret_cast<uint32_t>(Exception11), 0x08, 0x8E);
    SetGate(12, reinterpret_cast<uint32_t>(Exception12), 0x08, 0x8E);
    SetGate(13, reinterpret_cast<uint32_t>(Exception13), 0x08, 0x8E);
    SetGate(14, reinterpret_cast<uint32_t>(Exception14), 0x08, 0x8E);
    SetGate(15, reinterpret_cast<uint32_t>(Exception15), 0x08, 0x8E);
    SetGate(16, reinterpret_cast<uint32_t>(Exception16), 0x08, 0x8E);
    SetGate(17, reinterpret_cast<uint32_t>(Exception17), 0x08, 0x8E);
    SetGate(18, reinterpret_cast<uint32_t>(Exception18), 0x08, 0x8E);
    SetGate(19, reinterpret_cast<uint32_t>(Exception19), 0x08, 0x8E);
    SetGate(20, reinterpret_cast<uint32_t>(Exception20), 0x08, 0x8E);
    SetGate(21, reinterpret_cast<uint32_t>(Exception21), 0x08, 0x8E);
    SetGate(22, reinterpret_cast<uint32_t>(Exception22), 0x08, 0x8E);
    SetGate(23, reinterpret_cast<uint32_t>(Exception23), 0x08, 0x8E);
    SetGate(24, reinterpret_cast<uint32_t>(Exception24), 0x08, 0x8E);
    SetGate(25, reinterpret_cast<uint32_t>(Exception25), 0x08, 0x8E);
    SetGate(26, reinterpret_cast<uint32_t>(Exception26), 0x08, 0x8E);
    SetGate(27, reinterpret_cast<uint32_t>(Exception27), 0x08, 0x8E);
    SetGate(28, reinterpret_cast<uint32_t>(Exception28), 0x08, 0x8E);
    SetGate(29, reinterpret_cast<uint32_t>(Exception29), 0x08, 0x8E);
    SetGate(30, reinterpret_cast<uint32_t>(Exception30), 0x08, 0x8E);
    SetGate(31, reinterpret_cast<uint32_t>(Exception31), 0x08, 0x8E);

    SetGate(32, reinterpret_cast<uint32_t>(Request0), 0x08, 0x8E);
    SetGate(33, reinterpret_cast<uint32_t>(Request1), 0x08, 0x8E);
    SetGate(34, reinterpret_cast<uint32_t>(Request2), 0x08, 0x8E);
    SetGate(35, reinterpret_cast<uint32_t>(Request3), 0x08, 0x8E);
    SetGate(36, reinterpret_cast<uint32_t>(Request4), 0x08, 0x8E);
    SetGate(37, reinterpret_cast<uint32_t>(Request5), 0x08, 0x8E);
    SetGate(38, reinterpret_cast<uint32_t>(Request6), 0x08, 0x8E);
    SetGate(39, reinterpret_cast<uint32_t>(Request7), 0x08, 0x8E);
    SetGate(40, reinterpret_cast<uint32_t>(Request8), 0x08, 0x8E);
    SetGate(41, reinterpret_cast<uint32_t>(Request9), 0x08, 0x8E);
    SetGate(42, reinterpret_cast<uint32_t>(Request10), 0x08, 0x8E);
    SetGate(43, reinterpret_cast<uint32_t>(Request11), 0x08, 0x8E);
    SetGate(44, reinterpret_cast<uint32_t>(Request12), 0x08, 0x8E);
    SetGate(45, reinterpret_cast<uint32_t>(Request13), 0x08, 0x8E);
    SetGate(46, reinterpret_cast<uint32_t>(Request14), 0x08, 0x8E);
    SetGate(47, reinterpret_cast<uint32_t>(Request15), 0x08, 0x8E);

    GoToSetIdtr();

    initialized = true;
}

void
Interrupt::RegisterHandler(uint8_t vector_num, handler_t new_handler) {
    handler_[vector_num] = new_handler;
}

extern "C" void
HandleException(SavedMessage* message) {
    int vector_num = message->vector_num;
    // if (vector_num == 14) {
    //     ;
    //     // return;
    // }
    if (handler_[vector_num]) {
        handler_[vector_num](message);
    } else {
        Console::SubInstance().PrintFormatted("No~vector number: %d.\n", vector_num);
    }
}

extern "C" void
HandleRequest(SavedMessage* message) {
    int vector_num = message->vector_num;
    if (vector_num >= 40) {
        IoPort::Instance().WriteByte(0xA0, 0x20);
    }
    IoPort::Instance().WriteByte(0x20, 0x20);

    if (handler_[vector_num]) {
        handler_[vector_num](message);
    } else {
        Console::SubInstance().PrintFormatted("No~vector number: %d.\n", vector_num);
    }
}

void
Interrupt::Open(void) {
    asm volatile("sti");
}

void
Interrupt::Close(void) {
    asm volatile("cli");
}
} // namespace kernel
} // namespace hubbardos