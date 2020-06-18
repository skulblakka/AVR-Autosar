/**
 * @file
 *
 * @brief       API
 *
 * @date        2020-06-018
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */


#ifndef OS_API_H_
#define OS_API_H_

#include "Types.h"

/************************************************************************/
/* ALARM MANAGEMENT                                                     */
/************************************************************************/
#include "Alarm.h"

#define GetAlarmBase                Alarm_GetAlarmBase
#define GetAlarm                    Alarm_GetAlarm
#define SetRelAlarm                 Alarm_SetRelAlarm
#define SetAbsAlarm                 Alarm_SetAbsAlarm
#define CancelAlarm                 Alarm_CancelAlarm

/************************************************************************/
/* COUNTER MANAGEMENT                                                   */
/************************************************************************/
#include "Counter.h"

#define IncrementCounter            Counter_IncrementCounter
#define GetCounterValue             Counter_GetCounterValue
#define GetElapsedValue             Counter_GetElapsedValue

/************************************************************************/
/* EVENT MANAGEMENT                                                     */
/************************************************************************/
#include "Events.h"

#define SetEvent                    Events_SetEvent
#define ClearEvent                  Events_ClearEvent
#define GetEvent                    Events_GetEvent
#define WaitEvent                   Events_WaitEvent

/************************************************************************/
/* OPERATING SYSTEM CONTROL                                             */
/************************************************************************/
#include "OS.h"

#define StartOS                     OS_StartOS
#define ShutdownOS                  OS_ShutdownOS
#define GetActiveApplicationMode    OS_GetActiveApplicationMode
#define EnableAllInterrupts         OS_EnableAllInterrupts
#define DisableAllInterrupts        OS_DisableAllInterrupts
#define ResumeAllInterrupts         OS_ResumeAllInterrupts
#define SuspendAllInterrupts        OS_SuspendAllInterrupts
#define ResumeOSInterrupts          OS_ResumeOSInterrupts
#define SuspendOSInterrupts         OS_SuspendOSInterrupts

/************************************************************************/
/* RESOURCE MANAGEMENT                                                  */
/************************************************************************/
#include "Resource.h"

#define GetResource                 Resource_GetResource
#define ReleaseResource             Resource_ReleaseResource

/************************************************************************/
/* TASK MANAGEMENT                                                      */
/************************************************************************/
#include "Task.h"

#define ActivateTask                Task_ActivateTask
#define ChainTask                   Task_ChainTask
#define TerminateTask               Task_TerminateTask
#define Schedule                    Task_Schedule
#define GetTaskID                   Task_GetTaskID
#define GetTaskState                Task_GetTaskState

#endif /* OS_API_H_ */