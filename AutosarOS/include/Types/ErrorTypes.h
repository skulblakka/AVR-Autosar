/**
 * @file
 *
 * @brief       Types and macros for error handling
 *
 * @date        2020-06-18
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */


#ifndef ERRORTYPES_H_
#define ERRORTYPES_H_

#include <string.h>

/**
 * @brief   Type for Service IDs
 */
typedef enum osServiceIdType {
    OSServiceId_GetAlarmBase = 0,
    OSServiceId_GetAlarm,
    OSServiceId_SetRelAlarm,
    OSServiceId_SetAbsAlarm,
    OSServiceId_CancelAlarm,
    OSServiceId_IncrementCounter,
    OSServiceId_GetCounterValue,
    OSServiceId_GetElapsedValue,
    OSServiceId_SetEvent,
    OSServiceId_ClearEvent,
    OSServiceId_GetEvent,
    OSServiceId_WaitEvent,
    OSServiceId_StartOS,
    OSServiceId_ShutdownOS,
    OSServiceId_GetActiveApplicationMode,
    OSServiceId_EnableAllInterrupts,
    OSServiceId_DisableAllInterrupts,
    OSServiceId_ResumeAllInterrupts,
    OSServiceId_SuspendAllInterrupts,
    OSServiceId_ResumeOSInterrupts,
    OSServiceId_SuspendOSInterrupts,
    OSServiceId_GetResource,
    OSServiceId_ReleaseResource,
    OSServiceId_ActivateTask,
    OSServiceId_ChainTask,
    OSServiceId_TerminateTask,
    OSServiceId_Schedule,
    OSServiceId_GetTaskID,
    OSServiceId_GetTaskState
} OSServiceIdType;

/**
 * @brief   Structure used for saving error information
 */
struct errorInfo_s {
    OSServiceIdType id;     /**< ID of service */
#if OS_CONFIG_MAX_ERROR_PARAM >= 1
    uint64_t param1;        /**< First service parameter */
#endif /* OS_CONFIG_MAX_ERROR_PARAM >= 1 */
#if OS_CONFIG_MAX_ERROR_PARAM >= 2
    uint64_t param2;        /**< Second service parameter */
#endif /* OS_CONFIG_MAX_ERROR_PARAM >= 2 */
#if OS_CONFIG_MAX_ERROR_PARAM == 3
    uint64_t param3;        /**< Third service parameter */
#endif /* OS_CONFIG_MAX_ERROR_PARAM >= 3 */
};

#if defined(OS_CONFIG_MAX_ERROR_PARAM) && OS_CONFIG_MAX_ERROR_PARAM >= 0 && !defined(__DOXYGEN__)
#if OS_CONFIG_MAX_ERROR_PARAM == 0
#define OS_SET_ERROR_INFO0(serviceId)                       ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
                                                                errorInfo.id = serviceId; \
                                                            }

#define OS_SET_ERROR_INFO1(serviceId, paramPtr1, size1)     ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
                                                                errorInfo.id = serviceId; \
                                                            }

#define OS_SET_ERROR_INFO2(serviceId, paramPtr1, size1, \
                            paramPtr2, size2)               ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
                                                                errorInfo.id = serviceId; \
                                                            }

#define OS_SET_ERROR_INFO3(serviceId, paramPtr1, size1, \
                            paramPtr2, size2, \
                            paramPtr3, size3)               ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
                                                                errorInfo.id = serviceId; \
                                                            }
#endif /* OS_CONFIG_MAX_ERROR_PARAM == 0 */

#if OS_CONFIG_MAX_ERROR_PARAM == 1
#define OS_SET_ERROR_INFO0(serviceId)                       ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
                                                                errorInfo.id = serviceId; \
                                                            }

#define OS_SET_ERROR_INFO1(serviceId, paramPtr1, size1)     ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
                                                                errorInfo.id = serviceId; \
                                                                memcpy((uint8_t*) &errorInfo.param1, paramPtr1, size1); \
                                                            }

#define OS_SET_ERROR_INFO2(serviceId, paramPtr1, size1, \
                            paramPtr2, size2)               ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
                                                                errorInfo.id = serviceId; \
                                                                memcpy((uint8_t*) &errorInfo.param1, paramPtr1, size1); \
                                                            }

#define OS_SET_ERROR_INFO3(serviceId, paramPtr1, size1, \
                            paramPtr2, size2, \
                            paramPtr3, size3)               ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
                                                                errorInfo.id = serviceId; \
                                                                memcpy((uint8_t*) &errorInfo.param1, paramPtr1, size1); \
                                                            }
#endif /* OS_CONFIG_MAX_ERROR_PARAM == 1 */

#if OS_CONFIG_MAX_ERROR_PARAM == 2
#define OS_SET_ERROR_INFO0(serviceId)                       ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
                                                                errorInfo.id = serviceId; \
                                                            }

#define OS_SET_ERROR_INFO1(serviceId, paramPtr1, size1)     ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
                                                                errorInfo.id = serviceId; \
                                                                memcpy((uint8_t*) &errorInfo.param1, paramPtr1, size1); \
                                                            }

#define OS_SET_ERROR_INFO2(serviceId, paramPtr1, size1, \
                            paramPtr2, size2)               ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
                                                                errorInfo.id = serviceId; \
                                                                memcpy((uint8_t*) &errorInfo.param1, paramPtr1, size1); \
                                                                memcpy((uint8_t*) &errorInfo.param2, paramPtr2, size2); \
                                                            }

#define OS_SET_ERROR_INFO3(serviceId, paramPtr1, size1, \
                            paramPtr2, size2, \
                            paramPtr3, size3)               ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
                                                                errorInfo.id = serviceId; \
                                                                memcpy((uint8_t*) &errorInfo.param1, paramPtr1, size1); \
                                                                memcpy((uint8_t*) &errorInfo.param2, paramPtr2, size2); \
                                                            }
#endif /* OS_CONFIG_MAX_ERROR_PARAM == 2 */

#if OS_CONFIG_MAX_ERROR_PARAM == 3
#define OS_SET_ERROR_INFO0(serviceId)                       ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
                                                                errorInfo.id = serviceId; \
                                                            }

#define OS_SET_ERROR_INFO1(serviceId, paramPtr1, size1)     ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
                                                                errorInfo.id = serviceId; \
                                                                memcpy((uint8_t*) &errorInfo.param1, paramPtr1, size1); \
                                                            }

#define OS_SET_ERROR_INFO2(serviceId, paramPtr1, size1, \
                            paramPtr2, size2)               ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
                                                                errorInfo.id = serviceId; \
                                                                memcpy((uint8_t*) &errorInfo.param1, paramPtr1, size1); \
                                                                memcpy((uint8_t*) &errorInfo.param2, paramPtr2, size2); \
                                                            }

#define OS_SET_ERROR_INFO3(serviceId, paramPtr1, size1, \
                            paramPtr2, size2, \
                            paramPtr3, size3)               ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { \
                                                                errorInfo.id = serviceId; \
                                                                memcpy((uint8_t*) &errorInfo.param1, paramPtr1, size1); \
                                                                memcpy((uint8_t*) &errorInfo.param2, paramPtr2, size2); \
                                                                memcpy((uint8_t*) &errorInfo.param3, paramPtr3, size3); \
                                                            }
#endif /* OS_CONFIG_MAX_ERROR_PARAM == 3 */

#if OS_CONFIG_MAX_ERROR_PARAM > 3
#error Values of OS_CONFIG_MAX_ERROR_PARAM larger 3 are not supported!
#endif /* OS_CONFIG_MAX_ERROR_PARAM > 3 */

#else /* defined(OS_CONFIG_MAX_ERROR_PARAM) && OS_CONFIG_MAX_ERROR_PARAM != 0 */

/**
 * @brief   Set error info with zero parameters
 *
 * This will set error info with service ID if #OS_CONFIG_MAX_ERROR_PARAM is set to at least zero.
 *
 * @param   serviceId       ID of the service
 */
#define OS_SET_ERROR_INFO0(serviceId)

/**
 * @brief   Set error info with up to one parameter
 *
 * This will set error info with service ID and up to one parameter if #OS_CONFIG_MAX_ERROR_PARAM is set to at
 * least zero.
 *
 * @param   serviceId       ID of the service
 * @param   paramPtr1       Pointer to first parameter (used if #OS_CONFIG_MAX_ERROR_PARAM >= 1)
 * @param   size1           Size of first parameter in bytes (used if #OS_CONFIG_MAX_ERROR_PARAM >= 1)
 */
#define OS_SET_ERROR_INFO1(serviceId, paramPtr1, size1)

/**
 * @brief   Set error info with up to two parameters
 *
 * This will set error info with service ID and up to two parameters if #OS_CONFIG_MAX_ERROR_PARAM is set to at
 * least zero.
 *
 * @param   serviceId       ID of the service
 * @param   paramPtr1       Pointer to first parameter (used if #OS_CONFIG_MAX_ERROR_PARAM >= 1)
 * @param   size1           Size of first parameter in bytes (used if #OS_CONFIG_MAX_ERROR_PARAM >= 1)
 * @param   paramPtr2       Pointer to first parameter (used if #OS_CONFIG_MAX_ERROR_PARAM >= 2)
 * @param   size2           Size of first parameter in bytes (used if #OS_CONFIG_MAX_ERROR_PARAM >= 2)
 */
#define OS_SET_ERROR_INFO2(serviceId, paramPtr1, size1, paramPtr2, size2)

/**
 * @brief   Set error info with up to three parameters
 *
 * This will set error info with service ID and up to three parameters if #OS_CONFIG_MAX_ERROR_PARAM is set to at
 * least zero.
 *
 * @param   serviceId       ID of the service
 * @param   paramPtr1       Pointer to first parameter (used if #OS_CONFIG_MAX_ERROR_PARAM >= 1)
 * @param   size1           Size of first parameter in bytes (used if #OS_CONFIG_MAX_ERROR_PARAM >= 1)
 * @param   paramPtr2       Pointer to first parameter (used if #OS_CONFIG_MAX_ERROR_PARAM >= 2)
 * @param   size2           Size of first parameter in bytes (used if #OS_CONFIG_MAX_ERROR_PARAM >= 2)
 * @param   paramPtr3       Pointer to first parameter (used if #OS_CONFIG_MAX_ERROR_PARAM >= 3)
 * @param   size3           Size of first parameter in bytes (used if #OS_CONFIG_MAX_ERROR_PARAM >= 3)
 */
#define OS_SET_ERROR_INFO3(serviceId, paramPtr1, size1, paramPtr2, size2, paramPtr3, size3)
#endif /* defined(OS_CONFIG_MAX_ERROR_PARAM) && OS_CONFIG_MAX_ERROR_PARAM != 0 */

/************************************************************************/
/* ERROR INFORMATION ACCESS MACROS                                      */
/************************************************************************/
#define OSErrorGetServiceId()                   (errorInfo.id);

/* OSServiceId_GetAlarmBase */
#define OSError_GetAlarmBase_AlarmID()          ((AlarmType) errorInfo.param1)
#define OSError_GetAlarmBase_Info()             ((AlarmBaseRefType) errorInfo.param2)

/* OSServiceId_GetAlarm */
#define OSError_GetAlarm_AlarmID()              ((AlarmType) errorInfo.param1)
#define OSError_GetAlarm_Tick()                 ((TickRefType) errorInfo.param2)

/* OSServiceId_SetRelAlarm */
#define OSError_SetRelAlarm_AlarmID()           ((AlarmType) errorInfo.param1)
#define OSError_SetRelAlarm_increment()         ((TickType) errorInfo.param2)
#define OSError_SetRelAlarm_cycle()             ((TickType) errorInfo.param3)

/* OSServiceId_SetAbsAlarm */
#define OSError_SetAbsAlarm_AlarmID()           ((AlarmType) errorInfo.param1)
#define OSError_SetAbsAlarm_start()             ((TickType) errorInfo.param2)
#define OSError_SetAbsAlarm_cycle()             ((TickType) errorInfo.param3)

/* OSServiceId_CancelAlarm */
#define OSError_CancelAlarm_AlarmID()           ((AlarmType) errorInfo.param1)

/* OSServiceId_IncrementCounter */
#define OSError_IncrementCounter_CounterID()    ((CounterType) errorInfo.param1)

/* OSServiceId_GetCounterValue */
#define OSError_GetCounterValue_CounterID()     ((CounterType) errorInfo.param1)
#define OSError_GetCounterValue_Value()         ((TickRefType) errorInfo.param2)

/* OSServiceId_GetElapsedValue */
#define OSError_GetElapsedValue_CounterID()     ((CounterType) errorInfo.param1)
#define OSError_GetElapsedValue_Value()         ((TickRefType) errorInfo.param2)
#define OSError_GetElapsedValue_ElapsedValue()  ((TickRefType) errorInfo.param3)

/* OSServiceId_SetEvent */
#define OSError_SetEvent_TaskID()               ((TaskType) errorInfo.param1)
#define OSError_SetEvent_Mask()                 ((EventMaskType) errorInfo.param2)

/* OSServiceId_ClearEvent */
#define OSError_ClearEvent_Mask()               ((EventMaskType) errorInfo.param1)

/* OSServiceId_GetEvent */
#define OSError_GetEvent_TaskID()               ((TaskType) errorInfo.param1)
#define OSError_GetEvent_Type()                 ((EventMaskRefType) errorInfo.param2)

/* OSServiceId_WaitEvent */
#define OSError_WaitEvent_Mask()                ((EventMaskType) errorInfo.param1)

/* OSServiceId_StartOS */
#define OSError_StartOS_Mode()                  ((AppModeType) errorInfo.param1)

/* OSServiceId_ShutdownOS */
#define OSError_ShutdownOS_Error                ((StatusType) errorInfo.param1)

/* OSServiceId_GetResource */
#define OSError_GetResource_ResID()             ((ResourceType) errorInfo.param1)

/* OSServiceId_ReleaseResource */
#define OSError_ReleaseResource_ResID()         ((ResourceType) errorInfo.param1)

/* OSServiceId_ActivateTask */
#define OSError_ActivateTask_TaskID()           ((TaskType) errorInfo.param1)

/* OSServiceId_ChainTask */
#define OSError_ChainTask_TaskID()              ((TaskType) errorInfo.param1)

/* OSServiceId_GetTaskID */
#define OSError_GetTaskID_TaskID()              ((TaskRefType) errorInfo.param1)

/* OSServiceId_GetTaskState */
#define OSError_GetTaskState_TaskID()           ((TaskType) errorInfo.param1)
#define OSError_GetTaskState_State()            ((TaskStateRefType) errorInfo.param2)

#endif /* ERRORTYPES_H_ */