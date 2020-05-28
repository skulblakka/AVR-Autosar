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

#include "Types/TaskTypes.h"
#include "Types/ResourceTypes.h"
#include "Types/EventTypes.h"

#define OS_CONFIG_GEN_ENUM
#include "AppCfg.h"
#undef OS_CONFIG_GEN_ENUM

// TODO: Location for these two might cause problems...
#undef ISR
#define ISR(Name) extern void Func ## Name(void)

// TODO: Docu
typedef enum StatusType_e {
    E_OK = 0,
    E_OS_ACCESS,
    E_OS_CALLLEVEL,
    E_OS_ID,
    E_OS_LIMIT,
    E_OS_NOFUNC,
    E_OS_RESOURCE,
    E_OS_STATE,
    E_OS_VALUE
} StatusType;



#endif /* TYPES_H_ */