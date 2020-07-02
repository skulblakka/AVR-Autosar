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
#include "context.h"
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