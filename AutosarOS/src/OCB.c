/**
 * @file
 *
 * @brief       Implementation of Operating System Control Block
 *
 * @date        2019-09-02
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */

#include "OCB.h"
#include "OS.h"
#include "assert.h"
#include <avr/interrupt.h>

volatile bool isISR = false;
volatile uint8_t isCat2ISR;

volatile TaskType currentTask = INVALID_TASK;

volatile uint32_t sysTick;
volatile uint8_t needScheduling;
volatile uint8_t blockScheduling;
bool forceScheduling;

struct resource_s* volatile isrResourceQueue = NULL;

#if (defined(OS_CONFIG_HOOK_ERROR) && OS_CONFIG_HOOK_ERROR == true)
volatile bool blockErrorHook;
#endif /* (defined(OS_CONFIG_HOOK_ERROR) && OS_CONFIG_HOOK_ERROR == true) */

#if defined(OS_CONFIG_MAX_ERROR_PARAM) && OS_CONFIG_MAX_ERROR_PARAM >= 0
volatile struct errorInfo_s errorInfo;
#endif /* defined(OS_CONFIG_MAX_ERROR_PARAM) && OS_CONFIG_MAX_ERROR_PARAM >= 0 */

#define OS_CONFIG_GEN_DATA_STRUCT
#include "AppCfg.h"
#undef OS_CONFIG_GEN_DATA_STRUCT

#define OS_CONFIG_GEN_TCB
#include "AppCfg.h"
#undef OS_CONFIG_GEN_TCB

#define OS_CONFIG_GEN_FUNC
#include "AppCfg.h"
#undef OS_CONFIG_GEN_FUNC

// FIXME Implement generation of schedule tables via AppCfg
struct scheduleTable_s st1 = {
    .counter = C5,
    .autoStart = true,
    .cyclic = true,
    .currentState = SCHEDULETABLE_STOPPED,
    .finalDelay = 3,
    .next = INVALID_SCHEDULETABLE,
    .expiryPointList = {
        .offset = 0,
        .taskActionList = &(struct scheduleTableExpiryActionTask_s)
        {
            .task = T8,
            .next = &(struct scheduleTableExpiryActionTask_s) {
                .task = T9
            }
        },
        .eventActionList = &(struct scheduleTableExpiryActionEvent_s)
        {
            .task = T8,
            .event = 0b01,
            .next = &(struct scheduleTableExpiryActionEvent_s) {
                .task = T9,
                .event = 0b01
            }
        },
        .next = &(struct scheduleTableExpiryPoint_s)
        {
            .offset = 5,
            .eventActionList = &(struct scheduleTableExpiryActionEvent_s) {
                .task = T8,
                .event = 0b10
            },
            .next = &(struct scheduleTableExpiryPoint_s) {
                .offset = 10,
                .eventActionList = &(struct scheduleTableExpiryActionEvent_s) {
                    .task = T9,
                    .event = 0b10
                }
            }
        }
    }
};

struct scheduleTable_s st2 = {
    .counter = C5,
    .autoStart = false,
    .cyclic = false,
    .currentState = SCHEDULETABLE_STOPPED,
    .finalDelay = 3,
    .next = INVALID_SCHEDULETABLE,
    .expiryPointList = {
        .offset = 0,
        .eventActionList = &(struct scheduleTableExpiryActionEvent_s)
        {
            .task = T10,
            .event = 0b01
        },
        .next = &(struct scheduleTableExpiryPoint_s)
        {
            .offset = 5,
            .eventActionList = &(struct scheduleTableExpiryActionEvent_s) {
                .task = T10,
                .event = 0b10
            }
        }
    }
};

struct scheduleTable_s st3 = {
    .counter = C5,
    .autoStart = false,
    .cyclic = false,
    .currentState = SCHEDULETABLE_STOPPED,
    .finalDelay = 5,
    .next = INVALID_SCHEDULETABLE,
    .expiryPointList = {
        .offset = 1,
        .eventActionList = &(struct scheduleTableExpiryActionEvent_s)
        {
            .task = T10,
            .event = 0b01
        },
        .next = &(struct scheduleTableExpiryPoint_s)
        {
            .offset = 3,
            .eventActionList = &(struct scheduleTableExpiryActionEvent_s) {
                .task = T10,
                .event = 0b10
            }
        }
    }
};

struct scheduleTable_s st4 = {
    .counter = SYSTEM_COUNTER,
    .autoStart = false,
    .cyclic = false,
    .currentState = SCHEDULETABLE_STOPPED,
    .finalDelay = 5,
    .next = INVALID_SCHEDULETABLE,
    .expiryPointList = {
        .offset = 0,
        .eventActionList = &(struct scheduleTableExpiryActionEvent_s)
        {
            .task = T10,
            .event = 0b01
        },
        .next = &(struct scheduleTableExpiryPoint_s)
        {
            .offset = 50,
            .eventActionList = &(struct scheduleTableExpiryActionEvent_s) {
                .task = T10,
                .event = 0b10
            }
        }
    }
};

struct scheduleTable_s st5 = {
    .counter = C6,
    .autoStart = false,
    .cyclic = false,
    .currentState = SCHEDULETABLE_STOPPED,
    .finalDelay = 5,
    .next = INVALID_SCHEDULETABLE,
    .expiryPointList = {
        .offset = 0,
        .eventActionList = &(struct scheduleTableExpiryActionEvent_s)
        {
            .task = T10,
            .event = 0b01
        },
        .next = &(struct scheduleTableExpiryPoint_s)
        {
            .offset = 5,
            .eventActionList = &(struct scheduleTableExpiryActionEvent_s) {
                .task = T10,
                .event = 0b10
            }
        }
    }
};

struct scheduleTable_s st6 = {
    .counter = C7,
    .autoStart = false,
    .cyclic = false,
    .currentState = SCHEDULETABLE_STOPPED,
    .finalDelay = 5,
    .next = INVALID_SCHEDULETABLE,
    .expiryPointList = {
        .offset = 0,
        .eventActionList = &(struct scheduleTableExpiryActionEvent_s)
        {
            .task = T10,
            .event = 0b01
        },
        .next = &(struct scheduleTableExpiryPoint_s)
        {
            .offset = 5,
            .eventActionList = &(struct scheduleTableExpiryActionEvent_s) {
                .task = T10,
                .event = 0b10
            }
        }
    }
};

volatile struct scheduleTable_s* ScheduleTable_Cfg[] = {
    &st1,
    &st2,
    &st3,
    &st4,
    &st5,
    &st6
};