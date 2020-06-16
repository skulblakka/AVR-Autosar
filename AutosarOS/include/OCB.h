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

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "Types.h"

#include <avr/io.h>
#include <util/delay.h>



#define OS_CONFIG_GEN_FUNC_DECL
#include "AppCfg.h"
#undef OS_CONFIG_GEN_FUNC_DECL

/**
 * @brief   Is currently ISR context?
 */
extern volatile bool isISR;

/**
 * @brief   Priority of current Cat 2 ISR (zero if not in Cat 2 ISR)
 */
extern volatile uint8_t isCat2ISR;

/**
 * @brief   Current task control blocks
 */
extern volatile struct task_s* TCB_Cfg[];

/**
 * @brief   Current system tick
 */
extern volatile uint32_t sysTick;

/**
 * @brief   Scheduling needed during timer interrupt
 *
 * If set to 0 the system timer will not trigger rescheduling.
 */
extern volatile uint8_t needScheduling;

/**
 * @brief   Current resource control blocks
 */
extern volatile struct resource_s* Res_Cfg[];

/**
 * @brief   Resource queue for resources taken by Cat2 ISRs
 */
extern struct resource_s* volatile isrResourceQueue;

/**
 * @brief   Internal resource for tasks with no internal resource defined
 */
extern volatile struct internalResource_s IntResourceNULL_s;

/**
 * @brief   Current counter control blocks
 */
extern volatile struct counter_s* Counter_Cfg[];

/**
 * @brief   Current alarm control blocks
 */
extern volatile struct alarm_s* Alarm_Cfg[];

#endif /* OCB_H_ */