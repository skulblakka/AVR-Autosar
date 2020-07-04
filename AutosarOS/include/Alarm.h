/**
 * @file
 *
 * @brief       Alarm management
 *
 * @date        2020-06-15
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */


#ifndef ALARM_H_
#define ALARM_H_

#include "Types.h"

/**
 * @brief   Get alarm base
 *
 * This service will get a copy of the counter configuration used as the alarm base.
 *
 * @param   alarmID                 Alarm which base should be read
 * @param   info                    Pointer to copy base info into
 *
 * @return  E_OK                    No error \n
 *          E_OS_ID                 alarmID is invalid \n
 *          E_OS_PARAM_POINTER      Pointer parameter is invalid
 */
extern StatusType Alarm_GetAlarmBase(AlarmType alarmID, AlarmBaseRefType info);

/**
 * @brief   Get alarm
 *
 * Get relative value in ticks before alarm expires.
 *
 * @note    The value of tick is not defined if the alarm is not in use.
 *
 * @param   alarmID                 Alarm which remaining ticks should be read
 * @param   tick                    Pointer to write remaining ticks into
 *
 * @return  E_OK                    No error \n
 *          E_OS_ID                 alarmID is invalid \n
 *          E_OS_NOFUNC             Alarm is not used
 */
extern StatusType Alarm_GetAlarm(AlarmType alarmID, TickRefType tick);

/**
 * @brief   Set relative alarm
 *
 * Set a relative alarm. The alarm will expire in increment ticks. If cycle does not equal zero
 * the alarm will be restarted to expire in cycle ticks after it expires.
 *
 * @param   alarmID                 Alarm to set
 * @param   increment               Relative value in ticks (must be between one and maxallowedvalue of the base)
 * @param   cycle                   Cycle value for cyclic alarm (must be between mincycle and maxallowedvalue of
 *                                  the base)
 *
 * @return  E_OK                    No error \n
 *          E_OS_ID                 alarmID is invalid \n
 *          E_OS_STATE              Alarm is already in use \n
 *          E_OS_VALUE              Values of increment or cycle are outside of the admissible range
 */
extern StatusType Alarm_SetRelAlarm(AlarmType alarmID, TickType increment, TickType cycle);

/**
 * @brief   Set absolute alarm
 *
 * Set an absolute alarm. The alarm will expire once the base counter reaches the start value. If cycle
 * does not equal zero the alarm will be restarted to expire in cycle ticks after it expires.
 *
 * @param   alarmID                 Alarm to set
 * @param   start                   Absolute value in ticks (must be between zero and maxallowedvalue of the base)
 * @param   cycle                   Cycle value for cyclic alarm (must be between mincycle and maxallowedvalue of
 *                                  the base)
 *
 * @return  E_OK                    No error \n
 *          E_OS_ID                 alarmID is invalid \n
 *          E_OS_STATE              Alarm is already in use \n
 *          E_OS_VALUE              Values of increment or cycle are outside of the admissible range
 */
extern StatusType Alarm_SetAbsAlarm(AlarmType alarmID, TickType start, TickType cycle);

/**
 * @brief   Cancel alarm
 *
 * @param   alarmID                 Alarm to cancel
 *
 * @return  E_OK                    No error \n
 *          E_OS_ID                 alarmID is invalid \n
 *          E_OS_NOFUNC             Alarm is not used
 */
extern StatusType Alarm_CancelAlarm(AlarmType alarmID);

/**
 * @brief   Evaluate alarms with user generated counter
 *
 * This will evaluate all alarms with a user generated counter and handle expiration if necessary.
 *
 * counter may not equal SYSTEM_COUNTER. Alarm_evaluateSysTickAlarm() must be used instead.
 *
 * @param   counter                 ID of the counter that triggered the alarm evaluation
 */
extern void Alarm_evaluateAlarm(CounterType counter);

/**
 * @brief   Evaluate alarm with SYSTEM_COUNTER
 *
 * This will evaluate all alarms with the SYSTEM_COUNTER as their base and handle expiration
 * if necessary.
 */
extern void Alarm_evaluateSysTickAlarm(void);

/**
 * @brief   Startup function for alarm management
 */
extern void Alarm_startup(void);

#endif /* ALARM_H_ */