#ifndef TIMER_H
#define TIMER_H

#include "type.h"

namespace hubbardos {
namespace kernel {

class Timer {
public:
    static Timer& Instance(void);
    void StartWorking(void);
    uint32_t GetRequestsPerSecond(void);
    uint32_t GetClick(void);
    void SetClick(uint32_t value);
    void ClickAddOne(void);

private:
    Timer();
    Timer(uint32_t requests_per_second);
    Timer(const Timer&);
    Timer& operator=(const Timer&);
    uint32_t requests_per_second_;
    bool is_working;
    uint32_t click_;
};

} // namespace kernel
} // namespace hubbardos

#endif