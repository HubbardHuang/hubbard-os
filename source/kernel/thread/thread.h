#ifndef THREAD_H
#define THREAD_H

#include "type.h"

namespace hubbardos {
namespace kernel {

typedef struct TaskStruct TaskStruct;
typedef struct Context Context;

class Thread {
public:
    using function_t = void (*)(void*);
    using id_type = int32_t;
    enum class State { kRunning, kInterruptible, kUninterruptible, kZombie, kStopped };
    static const uint32_t kCommonTimeSlice = 1;
    static void InitializeScheduling(void);
    static void Schedule(void);
    static Thread::id_type Create(function_t task, void* arg);
    static void Exit(void);

private:
    static TaskStruct* running_head_;
    static TaskStruct* waiting_head_;
    static TaskStruct* current_task_;
    static id_type current_max_id_;

    static id_type GetId(void);
    static void SwitchTask(TaskStruct* next_task);
};

} // namespace kernel
} // namespace hubbardos

#endif