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
    if (ResID >= RESOURCE_COUNT) {
        return E_OS_ID;
    }
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (Res_Cfg[ResID]->assigned) {
            return E_OS_ACCESS;
        }

        if ((!isCat2ISR && TCB_Cfg[currentTask]->prio > Res_Cfg[ResID]->prio)
                || (isCat2ISR && curIsrPrio > Res_Cfg[ResID]->prio)) { // TODO Extended error check
            // Prio of requested resource is lower than static prio of calling task or ISR
            return E_OS_ACCESS;
        }

        Res_Cfg[ResID]->assigned = true;
        
        struct resource_s* volatile* resPtr;
        
        if (isCat2ISR) {
            // Set pointer to start of ISR resourceQueue
            resPtr = &isrResourceQueue;
        } else {
            // Set pointer to start of resourceQueue
            resPtr = &(TCB_Cfg[currentTask]->resourceQueue);
        }            

        /* Find next empty spot */
        while (*resPtr != NULL) {
            resPtr = &(*resPtr)->next;
        }
        
        // Set requested resource in empty spot found above 
        *resPtr = (struct resource_s*) Res_Cfg[ResID];

        if (!isCat2ISR) {
            /* Increase current task priority if ceiling priority is higher */
            if (TCB_Cfg[currentTask]->curPrio < Res_Cfg[ResID]->prio) {
                TCB_Cfg[currentTask]->curPrio = Res_Cfg[ResID]->prio;
            }
        }            
    }

    return E_OK;
}

extern StatusType OS_ReleaseResource(enum resources_e ResID)
{
    if (ResID >= RESOURCE_COUNT) {
        return E_OS_ID;
    }
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (!Res_Cfg[ResID]->assigned) {
            return E_OS_NOFUNC;
        }

        if ((!isCat2ISR && TCB_Cfg[currentTask]->prio > Res_Cfg[ResID]->prio) 
                || (isCat2ISR && curIsrPrio > Res_Cfg[ResID]->prio)) { // TODO Extended error check
            // Prio of requested resource is lower than static prio of calling task or ISR
            return E_OS_ACCESS;
        }
        
        uint8_t ceilingPrio;
        struct resource_s* volatile* resPtr;
        
        if (isCat2ISR) {
            // Set pointer to start of ISR resourceQueue and initial ceiling prio to the maximum value
            resPtr = &isrResourceQueue;
            ceilingPrio = UINT8_MAX;
        } else {
            // Set pointer to start of task resourceQueue and initial ceiling prio to static task prio
            resPtr = &(TCB_Cfg[currentTask]->resourceQueue);
            ceilingPrio = TCB_Cfg[currentTask]->prio;
        }

        /* Find last element in queue and new ceiling priority */
        while ((*resPtr)->next != NULL) {
            if (ceilingPrio < (*resPtr)->prio) {
                ceilingPrio = (*resPtr)->prio;
            }
            
            resPtr = &(*resPtr)->next;
        }
        
        if (*resPtr != Res_Cfg[ResID]) { // TODO Extended error check
            return E_OS_NOFUNC;
        }
     
        // Remove element from queue
        *resPtr = NULL;
        
        Res_Cfg[ResID]->assigned = false;

        if (!isCat2ISR) {
            // Set new task priority
            TCB_Cfg[currentTask]->curPrio = ceilingPrio;
        }        
    }
    
    // Rescheduling might be required because of possible change in priority
    OS_Schedule();

    return E_OK;
}

extern void OS_GetInternalResource()
{
    if (TCB_Cfg[currentTask]->internalResource != INVALID_INTERNAL_RESOURCE) {
        TCB_Cfg[currentTask]->internalResource->assigned = true;

        if (TCB_Cfg[currentTask]->curPrio < TCB_Cfg[currentTask]->internalResource->prio) {
            TCB_Cfg[currentTask]->curPrio = TCB_Cfg[currentTask]->internalResource->prio;
        }
    }
}

extern void OS_ReleaseInternalResource()
{
    if (TCB_Cfg[currentTask]->internalResource != INVALID_INTERNAL_RESOURCE) {
        TCB_Cfg[currentTask]->internalResource->assigned = false;

        // Reset task priority
        TCB_Cfg[currentTask]->curPrio = TCB_Cfg[currentTask]->prio;

        uint8_t ceilingPrio = TCB_Cfg[currentTask]->prio;
        struct resource_s* volatile* resPtr = &(TCB_Cfg[currentTask]->resourceQueue);

        /* Find ceiling priority */
        while ((*resPtr)->next != NULL) {
            if (ceilingPrio < (*resPtr)->prio) {
                ceilingPrio = (*resPtr)->prio;
            }

            resPtr = &(*resPtr)->next;
        }

        // Set new task priority
        TCB_Cfg[currentTask]->curPrio = ceilingPrio;

    }
}