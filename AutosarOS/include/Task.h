/*
 * Task.h
 *
 * Created: 05.08.2019 14:50:29
 *  Author: Pascal Romahn
 */ 


#ifndef TASK_H_
#define TASK_H_

#include <stdint.h>

#define TASK(TaskName) void Func ## TaskName(void)
#define PTASK(TaskName) &Func ## TaskName

typedef void (*pTaskFxn)(); // TODO: Location and name

typedef enum OsTaskType_e {
    BASIC = 0,
    EXTENDED
} OsTaskType;

typedef enum OsTaskSchedule_e {
    PREEMPTIVE = 0,
    NON_PREEMPTIVE
} OsTaskSchedule;

typedef enum OsTaskState_e {
    SUSPENDED = 0,
    READY,
    RUNNING,
    WAITING    
} OsTaskState;

typedef enum OsTaskAutostart_e {
    NO_AUTOSTART = 0,
    AUTOSTART
} OsTaskAutostart;

struct task_s {
    uint8_t* const stack;
    const uint16_t stackSize;
    const uint8_t prio;
    const uint8_t numberOfActivations;
    const uint8_t autostart;
    const OsTaskType taskType;
    const OsTaskSchedule taskSchedule;
    const pTaskFxn taskFxn;
    uint8_t* context;
    uint8_t curPrio;
    OsTaskState curState;
};

#endif /* TASK_H_ */