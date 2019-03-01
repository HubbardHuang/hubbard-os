#include "thread.h"
#include "console.h"
#include "heap_region.h"
#include "physical_memory.h"
#include "virtual_memory.h"

namespace hubbardos {
namespace kernel {

extern "C" void SwitchContext(Context* current, Context* next);
extern char kernel_stack[PhysialMemory::kThreadStackSize_];

struct Context {
    uint32_t esp;
    uint32_t ebp;
    uint32_t ebx;
    uint32_t esi;
    uint32_t edi;
    uint32_t eflags;
};

struct TaskStruct {
    volatile Thread::State state;
    Thread::id_type id;
    void* stack;
    VirtualMemory::pgd_t* mm;
    Context context;
    TaskStruct* next;
};

TaskStruct* Thread::running_head_ = nullptr;
TaskStruct* Thread::waiting_head_ = nullptr;
TaskStruct* Thread::current_task_ = nullptr;
Thread::id_type Thread::current_max_id_ = 0;

void
Thread::InitializeScheduling(void) {
    current_task_ =
      reinterpret_cast<TaskStruct*>(reinterpret_cast<uint32_t>(kernel_stack));
    current_task_->state = State::kRunning;
    current_task_->id = GetId();
    current_task_->mm = nullptr;
    current_task_->stack = current_task_;

    current_task_->next = current_task_;
    running_head_ = current_task_;
}

void
Thread::Schedule(void) {
    if (current_task_) {
        SwitchTask(current_task_->next);
    }
}

void
Thread::SwitchTask(TaskStruct* next_task) {
    if (current_task_ == next_task) {
        return;
    }
    TaskStruct* prev_task = current_task_;
    current_task_ = next_task;
    SwitchContext(&(prev_task->context), &(current_task_->context));
}

Thread::id_type
Thread::GetId(void) {
    ++current_max_id_;
    return current_max_id_;
}

Thread::id_type
Thread::Create(Thread::function_t task, void* arg) {
    if (!task) {
        return -1;
    }
    void* new_stack = HeapRegion::Allocate(PhysialMemory::kThreadStackSize_);
    TaskStruct* new_task_struct = reinterpret_cast<TaskStruct*>(new_stack);

    new_task_struct->state = State::kRunning;
    new_task_struct->id = GetId();
    new_task_struct->mm = nullptr;

    uint32_t* new_stack_top = reinterpret_cast<uint32_t*>(
      reinterpret_cast<uint32_t>(new_stack) + PhysialMemory::kThreadStackSize_);
    new_task_struct->stack = reinterpret_cast<void*>(new_stack_top);
    *(--new_stack_top) = reinterpret_cast<uint32_t>(arg);
    *(--new_stack_top) = reinterpret_cast<uint32_t>(Exit);
    *(--new_stack_top) = reinterpret_cast<uint32_t>(task);
    new_task_struct->context.esp = reinterpret_cast<uint32_t>(new_stack_top);
    new_task_struct->context.eflags = 0x200;

    // take the new thread to the tail of the running list
    new_task_struct->next = running_head_;
    TaskStruct* tail_node = running_head_;

    while (tail_node->next != running_head_) {
        Console::SubInstance().PrintFormatted("v");
        tail_node = tail_node->next;
    }
    tail_node->next = new_task_struct;

    return new_task_struct->id;
}

void
Thread::Exit(void) {
    uint32_t val = 0;
    Console::SubInstance().PrintFormatted("Thread exited with value %d\n", val);
}

} // namespace kernel
} // namespace hubbardos