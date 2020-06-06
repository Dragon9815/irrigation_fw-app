/*****************************************************************************
 * @file        tasking.h
 * @brief       Multitasking implementation
 * @author      gruenzinger.stefan@gmail.com
 * @copyright   Copyright (c) 2020 Stefan Grünzinger
 *****************************************************************************
 */
#include "system/tasking.h"

#include "system/syscall.h"
#include "system/timing.h"

#include <stdlib.h>
#include <stm32f4xx.h>
#include <string.h>

/**
 * \brief queue node.
 *
 * Any data structure can extend a queue by adding this header to the beginning
 * of the structure.  The macros below automatically cast arguments to qnode_t,
 * which makes it easy to enqueue and dequeue items.
 */
typedef struct qnode
{
    struct qnode * prev;
    struct qnode * next;
} qnode_t;

/**
 * \brief queue.
 *
 * The queue has a head and a tail.  Items are normally added to the tail and
 * removed from the head.  There are two edge cases we must handle: if the queue
 * is empty, then both head and tail will be null.  In this case, the head needs
 * to be set to the tail.  If the last element has been removed from the queue,
 * then the head will be null.  In this case, the tail needs to be set to the
 * head.
 */
typedef struct qqueue
{
    qnode_t * head;
    qnode_t * tail;
} qqueue_t;

/**
 * Initialize a queue.
 */
#define QINIT(queue) (queue)->head = (queue)->tail = NULL

/**
 * Enqueue an element; setting the head to the tail if the queue is empty.
 */
#define QENQUEUE(queue, elem)         \
    if((queue)->tail)                 \
        (queue)->tail->next = (elem); \
    else                              \
        (queue)->head = (elem);       \
    (elem)->prev  = (queue)->tail;    \
    (elem)->next  = NULL;             \
    (queue)->tail = (elem)

/**
 * Dequeue an element; setting the tail to NULL if this is the last element.
 */
#define QDEQUEUE(queue, elem)                \
    if((queue)->head) {                      \
        (elem)        = (queue)->head;       \
        (queue)->head = (queue)->head->next; \
        (elem)->next = (elem)->prev = NULL;  \
        if(!(queue)->head) {                 \
            (queue)->tail = NULL;            \
        }                                    \
        else {                               \
            (queue)->head->prev = NULL;      \
        }                                    \
    }                                        \
    else                                     \
        (elem) = NULL

typedef void (*task_entry)(void *);

typedef struct task
{
    taskid_t pid;
    qnode_t  hdr;
    bool     running;
    //    uint32_t   flags;
    uint32_t   psp;
    uint32_t   stack_start;
    uint32_t   stack_size;
    task_entry start;
    void *     context;
    qqueue_t   blockers;
} task_t;

static qqueue_t task_queue;
static task_t * current_task = NULL;
static taskid_t next_pid     = 0;

static task_t gTasks[OS_MAX_TASKS + 1];

void tasking_svc_yield(u32 * svc_data);
void tasking_svc_exit(u32 * svc_data);

void tasking_init(void)
{
    memset(gTasks, 0, sizeof(gTasks));

    QINIT(&task_queue);

    syscall_register(SYSCALL_YIELD, tasking_svc_yield);
    syscall_register(SYSCALL_EXIT, tasking_svc_exit);
}

static size_t findTaskSlot(void)
{
    for(int i = 1; i < OS_MAX_TASKS; i++) {
        if(!gTasks[i].running)
            return i;
    }

    return 0;
}

void task_end()
{
    SYSCALL(SYSCALL_EXIT); // will not return
    while(1)
        ;
}

taskid_t task_add(osTaskFunc_t entryPoint, void * context, u32 stack_size)
{
    size_t slot = findTaskSlot();
    if(slot <= 0)
        return 0;

    task_t * task = &gTasks[slot];

    task->pid         = next_pid++;
    task->running     = true;
    task->psp         = (uint32_t)malloc(stack_size) + stack_size;
    task->stack_start = task->psp;
    task->stack_size  = stack_size; //&gTasksStack[ pid * OS_TASK_STACK_SIZE ] +
                                    // OS_TASK_STACK_SIZE;//(uint32_t)malloc(stack_size)
                                    //+ stack_size;
    task->start   = entryPoint;
    task->context = context;
    QINIT(&task->blockers);

    /* set up task entry. */
    uint32_t * stack = (uint32_t *)task->psp;
    --stack;
    *stack = (uint32_t)0x21000000; /* xPSR */
    --stack;
    *stack = (uint32_t)task->start & 0xFFFFFFFE; /* PC */
    --stack;
    *stack = (uint32_t)&task_end; /* LR */
    --stack;
    *stack = 0; /* R12 */
    --stack;
    *stack = 0; /* R3 */
    --stack;
    *stack = 0; /* R2 */
    --stack;
    *stack = 0; /* R1 */
    --stack;
    *stack = (uint32_t)task->context; /* R0 */
    --stack;
    *stack = (uint32_t)0x00000000; /* R11 */
    --stack;
    *stack = (uint32_t)0x00000000; /* R10 */
    --stack;
    *stack = (uint32_t)0x00000000; /* R9 */
    --stack;
    *stack = (uint32_t)0x00000000; /* R8 */
    --stack;
    *stack = (uint32_t)0x00000000; /* R7 */
    --stack;
    *stack = (uint32_t)0x00000000; /* R6 */
    --stack;
    *stack = (uint32_t)0x00000000; /* R5 */
    --stack;
    *stack    = (uint32_t)0x00000000; /* R4 */
    task->psp = (uint32_t)stack;

    QENQUEUE(&task_queue, (qnode_t *)task);
    return task->pid;
}

bool task_is_running(taskid_t pid)
{
    if(pid == 0)
        return false;

    for(int i = 1; i < OS_MAX_TASKS; i++) {
        if(gTasks[i].running && gTasks[i].pid == pid) {
            return true;
        }
    }
    return false;
}

ALWAYS_INLINE
inline void task_yield()
{
    SYSCALL(SYSCALL_YIELD);
}

void tasking_switch_context_next_task(bool readd_task_to_queue)
{
    qnode_t * new_task;

    if(current_task != NULL) {
        current_task->psp = __get_PSP();
        if(readd_task_to_queue) {
            QENQUEUE(&task_queue, (qnode_t *)current_task);
        }
    }
    QDEQUEUE(&task_queue, new_task);
    current_task = (task_t *)new_task;

    if(current_task == NULL) {
        // no tasks to run
        __BKPT(0);
        while(true)
            ;
    }

    if(current_task->psp < (current_task->stack_start - current_task->stack_size)) {
        // stack overflow !!!
        __BKPT(0);
        while(true)
            ;
    }

    __set_PSP(current_task->psp);
}

void tasking_svc_yield(u32 * svc_data)
{
    WA_UNUSED(svc_data);
    // switch to next task and queue current task
    tasking_switch_context_next_task(true);
}

void tasking_svc_exit(u32 * svc_data)
{
    WA_UNUSED(svc_data);
    // switch to next task and don't queue current task
    task_t * oldtask = current_task;
    tasking_switch_context_next_task(false);

    // free stack memory
    free((void *)(oldtask->stack_start - oldtask->stack_size));

    // mark task as free by clearing first bit in flags
    oldtask->running = false;
}

#define USE_PSP_IN_THREAD_MODE      (1 << 1)
#define THREAD_MODE_IS_UNPRIVILEGED (1 << 0)

void tasking_start(void)
{
    uint32_t pspStart;

    // we need a small bit of process stack for the first task switch
    pspStart = (uint32_t)malloc(128) + 128;
    __set_PSP(pspStart);
    __ISB();

    // enable use of psp and switch to unprivileged mode
    __set_CONTROL(USE_PSP_IN_THREAD_MODE | THREAD_MODE_IS_UNPRIVILEGED);
    __ISB();

    // start the first task
    task_yield();

    while(true)
        ;
}

void task_delay(uint32_t ms)
{
    uint32_t T0, T1, dT;

    if(ms < UINT32_MAX)
        ms += 1;

    T0 = Timing_GetTicks_ms();
    while(true) {
        T1 = Timing_GetTicks_ms();
        dT = T1 - T0;
        if(dT >= ms)
            break;

        task_yield();
    }
}
