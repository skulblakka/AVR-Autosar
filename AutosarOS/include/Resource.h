/**
 * @file
 *
 * @brief       Resource management
 *
 * @date        2020-04-27
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */


#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <stdint.h>
#include <stdbool.h>

#include "Types.h"
#include "OCB.h"

/**
 * @brief   Request a resource
 *
 * This call serves to enter critical sections in the code that are assigned to the resource referenced by ResID.
 * A critical section shall always be left using #OS_ReleaseResource.
 *
 * @param   ResID   Reference to resource
 *
 * @return  E_OK            No error \n
 *          E_OS_ID         Invalid resource ID \n
 *          E_OS_ACCESS     Attempt to get a resource which is already occupied by any task or ISR,
 *                          or the statically assigned priority of the calling task or interrupt
 *                          routine is higher than the calculated ceiling priority
 */
extern StatusType OS_GetResource(enum resources_e ResID);

/**
 * @brief   Release a resource
 *
 * #OS_ReleaseResource is the counterpart of #OS_GetResource and serves to leave critical sections in the code
 * that are assigned to the resource referenced by ResID.
 *
 * @param   ResID   Reference to resource
 *
 * @return  E_OK            No error \n
 *          E_OS_ID         Invalid resource ID \n
 *          E_OS_NOFUNC     Attempt to release a resource which is not occupied or another resource shall
 *                          be released before
 *          E_OS_ACCESS     Attempt to release a resource which has a lower ceiling priority
 *                          than the statically assigned priority of the calling task or interrupt routine
 *
 */
extern StatusType OS_ReleaseResource(enum resources_e ResID);

/**
 * @brief   Get internal resource of the current task if one is assigned
 *
 * @warning This function should only be called with interrupts disabled!
 */
extern void OS_GetInternalResource();

/**
 * @brief   Release internal resource of the current task if one is assigned
 *
 * @warning This function should only be called with interrupts disabled!
 */
extern void OS_ReleaseInternalResource();


#endif /* RESOURCE_H_ */