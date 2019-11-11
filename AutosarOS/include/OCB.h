/**
 * @file
 *
 * @brief       Operating System Control Block
 *
 * This contains the operating system control blocks generated during compilation.
 *
 * @date        2019-09-02
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */


#ifndef OCB_H_
#define OCB_H_

#include <stdbool.h>
#include "Types.h"
#include "TaskTypes.h"

#include <avr/io.h>
#include <util/delay.h>

#define OS_CONFIG_GEN_ENUM
#include "AppCfg.h"
#undef OS_CONFIG_GEN_ENUM

#define OS_CONFIG_GEN_FUNC_DECL
#include "AppCfg.h"
#undef OS_CONFIG_GEN_FUNC_DECL

/**
 * @brief   Is currently ISR context?
 */
extern volatile bool isISR;

/**
 * @brief   Is currently Cat 2 ISR context?
 */
extern volatile bool isCat2ISR;

/**
 * @brief   Current task control blocks
 */
extern volatile struct task_s* TCB_Cfg[];

/**
 * @brief   Current system tick
 */
extern volatile uint32_t sysTick;

#endif /* OCB_H_ */