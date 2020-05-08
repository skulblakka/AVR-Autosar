/**
 * @file
 *
 * @date        2020-04-27
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */

#include "Resource.h"
#include "Types.h"
#include "OS.h"
#include "assert.h"

#include <util/atomic.h>

extern StatusType OS_GetResource(enum resources_e ResID)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (ResID >= RESOURCE_COUNT) {
            return E_OS_ID;
        }

        if (Res_Cfg[ResID]->assigned) {
            return E_OS_ACCESS;
        }

        if (!isCat2ISR && TCB_Cfg[currentTask]->prio > Res_Cfg[ResID]->prio) { // TODO Extended error check
            // We are not in an ISR and the prio of the resource is higher than the static prio of the requesting task
            return E_OS_ACCESS;
        }

        Res_Cfg[ResID]->assigned = true;
        
        // Set pointer to start of resourceQueue
        struct resource_s* volatile* resPtr = &(TCB_Cfg[currentTask]->resourceQueue);

        /* Find next empty spot */
        while (*resPtr != NULL) {
            resPtr = &(*resPtr)->next;
        }
        
        // Set requested resource in empty spot found above 
        *resPtr = (struct resource_s*) Res_Cfg[ResID];

        /* Increase current task priority if ceiling priority is higher */
        if (TCB_Cfg[currentTask]->curPrio < Res_Cfg[ResID]->prio) {
            TCB_Cfg[currentTask]->curPrio = Res_Cfg[ResID]->prio;
        }
    }

    return E_OK;
}

extern StatusType OS_ReleaseResource(enum resources_e ResID)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (ResID >= RESOURCE_COUNT) {
            return E_OS_ID;
        }

        if (!Res_Cfg[ResID]->assigned) {
            return E_OS_NOFUNC;
        }

        if (!isCat2ISR && TCB_Cfg[currentTask]->prio > Res_Cfg[ResID]->prio) { // TODO Extended error check
            // We are not in an ISR and the prio of the resource is lower than the static prio of the requesting task
            return E_OS_ACCESS;
        }
        
        // Set pointer to start of resourceQueue
        struct resource_s* volatile* resPtr = &(TCB_Cfg[currentTask]->resourceQueue);
        
        assert(resPtr != NULL);

        /* Find last element in queue */
        while ((*resPtr)->next != NULL) {
            resPtr = &(*resPtr)->next;
        }
        
        if (*resPtr != Res_Cfg[ResID]) { // TODO Extended error check
            return E_OS_NOFUNC;
        }
     
        // Remove element from queue
        *resPtr = NULL;
        
        Res_Cfg[ResID]->assigned = false;

        if (TCB_Cfg[currentTask]->curPrio < Res_Cfg[ResID]->prio) {
            TCB_Cfg[currentTask]->curPrio = Res_Cfg[ResID]->prio;
        }
    }
    
    // Rescheduling might be required because of possible change in priority
    OS_Schedule();

    return E_OK;
}