/**
 * @file
 *
 * @brief       Type definitions
 *
 * @date        2019-09-10
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */


#ifndef TYPES_H_
#define TYPES_H_

#define OS_CONFIG_GEN_ENUM
#include "AppCfg.h"
#undef OS_CONFIG_GEN_ENUM

#include "Types/TaskTypes.h"
#include "Types/ResourceTypes.h"
#include "Types/EventTypes.h"
#include "Types/CounterTypes.h"
#include "Types/AlarmTypes.h"
#include "Types/ErrorTypes.h"
#include "Types/ScheduleTableTypes.h"

#undef ISR
/**
 * @brief   Define implementation for interrupt service routine
 */
#define ISR(Name) extern void Func ## Name(void)

/**
 * @brief   Type for status
 */
typedef enum StatusType_e {
    E_OK = 0,
    E_OS_ACCESS,
    E_OS_CALLLEVEL,
    E_OS_ID,
    E_OS_LIMIT,
    E_OS_NOFUNC,
    E_OS_RESOURCE,
    E_OS_STATE,
    E_OS_VALUE,
    E_OS_STACKFAULT,
    E_OS_PARAM_POINTER,
    E_OS_PROTECTION_ARRIVAL
} StatusType;

/**
 * @brief   Return type for protection hook
 */
typedef enum ProtectionReturnType_e {
    PRO_IGNORE,                 /**< Ignore protection error */
    PRO_TERMINATETASKISR,       /**< Terminate faulty task or ISR */
    PRO_TERMINATEAPPL,          /**< Terminate faulty OS application */
    PRO_TERMINATEAPPL_RESTART,  /**< Terminate and restart faulty OS application */
    PRO_SHUTDOWN                /**< Shutdown system */
} ProtectionReturnType;

/**
 * @brief   Type for application mode
 */
typedef enum applicationMode_e {
    OSDEFAULTAPPMODE
} AppModeType;

#endif /* TYPES_H_ */