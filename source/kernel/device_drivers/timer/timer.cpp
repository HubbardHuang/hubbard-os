#include "timer.h"
#include "console.h"
#include "interrupt.h"
#include "io_port.h"

namespace hubbardos {
namespace kernel {

static void
TimerInterrupt(SavedMessage* message) {
    Timer::Instance().ClickAddOne();
    uint32_t click = Timer::Instance().GetClick();

    Console::SubInstance().PrintFormatted("Timer: %d.\n", click);
    if (click == 50) {
        Timer::Instance().SetClick(0);
    }
}

Timer&
Timer::Instance(void) {
    static Timer singleton(1);
    return singleton;
}

Timer::Timer(uint32_t requests_per_second)
  : requests_per_second_(requests_per_second)
  , is_working(false)
  , click_(0) {

    if (requests_per_second_ == 0) {
        return;
    }

    IoPort::Instance().WriteByte(0x43, 0x36);
    uint32_t divisor = 1193180 / requests_per_second_;
    uint8_t low_byte = (uint8_t)(divisor & 0xFF);
    uint8_t high_byte = (uint8_t)((divisor >> 8) & 0xFF);
    IoPort::Instance().WriteByte(0x40, low_byte);
    IoPort::Instance().WriteByte(0x40, high_byte);
}

void
Timer::StartWorking(void) {
    if (is_working == true) {
        return;
    }
    Interrupt::RegisterHandler(InterruptVectorNumber::kTimer, TimerInterrupt);
    Interrupt::Open();
    is_working = true;
}

uint32_t
Timer::GetRequestsPerSecond(void) {
    return requests_per_second_;
}

uint32_t
Timer::GetClick(void) {
    return click_;
}

void
Timer::SetClick(uint32_t value) {
    click_ = value;
}

void
Timer::ClickAddOne(void) {
    ++click_;
}

} // namespace kernel
} // namespace hubbardos
