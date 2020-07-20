/**
 * @file
 *
 * @brief       Schedule tables management
 *
 * @date        2020-06-25
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */


#ifndef SCHEDULETABLES_H_
#define SCHEDULETABLES_H_

#include "Types.h"

/**
 * @brief   Start schedule table from relative offset
 *
 * Start schedule table from relative offset. The first expiry point will be executed in
 * offset + offset of first expiry point ticks of the underlying counter.
 *
 * Offset must be greater than zero and smaller or equal MaxAllowedValue of the underlying counter.
 *
 * @param   scheduleTableID         ID of the schedule table to start
 * @param   offset                  Offset in Ticks before the schedule table is executed
 *
 * @return  E_OK                    No error \n
 *          E_OS_ID                 Invalid schedule table ID \n
 *          E_OS_VALUE              Value of offset is invalid \n
 *          E_OS_STATE              Schedule table was already started
 */
extern StatusType ScheduleTable_StartScheduleTableRel(ScheduleTableType scheduleTableID, TickType offset);

/**
 * @brief   Start schedule table from absolute start
 *
 * Start the schedule table from an absolute starting point. The first expiry point will be
 * executed at start + offset of first expiry point tick value of the underlaying counter.
 *
 * Start must be below or equal MaxAllowedValue of the underlying counter.
 *
 * @param   scheduleTableID         ID of the schedule table to start
 * @param   start                   Start value in ticks
 *
 * @return  E_OK                    No error \n
 *          E_OS_ID                 Invalid schedule table ID \n
 *          E_OS_VALUE              Value of start is invalid \n
 *          E_OS_STATE              Schedule table was already started
 */
extern StatusType ScheduleTable_StartScheduleTableAbs(ScheduleTableType scheduleTableID, TickType start);

/**
 * @brief   Stop schedule table
 *
 * Stop execution of the schedule table.
 *
 * @param   scheduleTableID         ID of the schedule table to stop
 *
 * @return  E_OK                    No error \n
 *          E_OS_ID                 Invalid schedule table ID \n
 *          E_OS_NOFUNC             Schedule table was already stopped
 */
extern StatusType ScheduleTable_StopScheduleTable(ScheduleTableType scheduleTableID);

/**
 * @brief   Queue schedule table
 *
 * Queue schedule table to be executed after the current schedule table finishes
 * execution. If a there is already a next schedule table configured it will be replaced.
 *
 * Both schedule tables must be driven by the same counter.
 *
 * @param   scheduleTableID_from    ID of current schedule table
 * @param   scheduleTableID_to      ID of schedule table to be queued
 *
 * @return  E_OK                    No error \n
 *          E_OS_ID                 Invalid schedule table IDs or counters not matching \n
 *          E_OS_NOFUNC             Current schedule table is not started \n
 *          E_OS_STATE              Schedule table to be queues is not stopped
 */
extern StatusType ScheduleTable_NextScheduleTable(ScheduleTableType scheduleTableID_from,
        ScheduleTableType scheduleTableID_to);

/**
 * @brief   Get schedule table status
 *
 * Get current status of a schedule table.
 *
 * @param   scheduleTableID         ID of schedule table
 * @param   scheduleStatus          Reference to status
 *
 * @return  E_OK                    No error \n
 *          E_OS_ID                 Invalid schedule table ID \n
 *          E_OS_PARAM_POINTER      Pointer parameter is invalid
 */
extern StatusType ScheduleTable_GetScheduleTableStatus(ScheduleTableType scheduleTableID,
        ScheduleTableStatusRefType scheduleStatus);

/**
 * @brief   Handle tick
 *
 * Handles a tick of the specified counter. This will increment all associated schedule
 * tables and handle expiry points if necessary.
 *
 * @param   counter                 ID of the counter that triggered the tick
 */
extern void ScheduleTable_handleTick(CounterType counter);

/**
 * @brief   Handle system tick
 *
 * Calls ScheduleTable_handleTick() where counter euqals SYSTEM_COUNTER. This only exists
 * to simplify the function call within the SysTick interrupt.
 */
extern void ScheduleTable_handleSysTick(void);

/**
 * @brief   Startup function for schedule table management
 *
 * This function starts all schedule tables configured as autostart and handles expiry
 * points if necessary.
 */
extern void ScheduleTable_startup(void);

#endif /* SCHEDULETABLES_H_ */