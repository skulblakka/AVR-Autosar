/**
 * @file
 *
 * @brief       Event management
 *
 * @date        2020-05-28
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */


#ifndef EVENTS_H_
#define EVENTS_H_

#include "Types.h"

/**
 * @brief   Set events
 *
 * Set one or several events according to the specified events mask. Other events
 * remain unchanged.
 *
 * The service may be called from an ISR or task level, but not from hook routines.
 *
 * If the specified was waiting for at least one of the events specified it will
 * be transferred to the ready state.
 *
 * @param   TaskID          ID of the task to set the events for
 * @param   events          Mask of the events to set
 *
 * @return  E_OK            No error \n
 *          E_OS_ID         TaskID is invalid \n
 *          E_OS_ACCESS     Referenced task is no extended task \n
 *          E_OS_STATE      Referenced task is in suspended state
 */
extern StatusType Events_SetEvent(enum tasks_e TaskID, EventMaskType events);

/**
 * @brief   Clear events
 *
 * Clear one or several events according to the specified events mask.
 *
 * Events can only be cleared by extended tasks. Tasks can only clear their
 * own events.
 *
 * @param   events          Mask of events to clear
 *
 * @return  E_OK            No error \n
 *          E_OS_ACCESS     Call not from extended task \n
 *          E_OS_CALLLEVEL  Call at interrupt level
 */
extern StatusType Events_ClearEvent(EventMaskType events);

/**
 * @brief   Get events
 *
 * Get current event state of task.
 *
 * @param   TaskID          ID of the task to request event state from
 * @param   events          Pointer to write event state to
 *
 * @return  E_OK            No error \n
 *          E_OS_ID         TaskID is invalid \n
 *          E_OS_ACCESS     Referenced task is not an extended task \n
 *          E_OS_STATE      Referenced task is in the suspended state
 */
extern StatusType Events_GetEvent(enum tasks_e TaskID, EventMaskRefType events);

/**
 * @brief   Wait for events
 *
 * Wait for events according to the specified events mask. The calling task
 * is transferred to the waiting state unless at least one of the specified
 * events is already pending/set.
 *
 * If rescheduling takes place the internal resource of the task is released
 * while the task is in the waiting state.
 *
 * This service shall only be called from the extended task owning the events.
 *
 * @param   events          Mask of events to wait for
 *
 * @return  E_OK            No error \n
 *          E_OS_ACCESS     Calling task is not an extended task \n
 *          E_OS_RESOURCE   Calling task occupies resources
 *          E_OS_CALLLEVEL  Call at interrupt level
 */
extern StatusType Events_WaitEvent(EventMaskType events);



#endif /* EVENTS_H_ */