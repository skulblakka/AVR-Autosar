/**
 * @file
 *
 * @brief       Macros used for generation of system
 *
 * @date        2019-09-02
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */

/* Undefine already existing defines for generation */
#ifdef OS_CONFIG_TASK_BEGIN
#undef OS_CONFIG_TASK_BEGIN
#endif
#ifdef OS_CONFIG_TASK_DEF
#undef OS_CONFIG_TASK_DEF
#endif
#ifdef OS_CONFIG_TASK_END
#undef OS_CONFIG_TASK_END
#endif
#ifdef OS_CONFIG_INT_BEGIN
#undef OS_CONFIG_INT_BEGIN
#endif
#ifdef OS_CONFIG_INT_DEF
#undef OS_CONFIG_INT_DEF
#endif
#ifdef OS_CONFIG_INT_END
#undef OS_CONFIG_INT_END
#endif
#ifdef OS_CONFIG_RESOURCE_BEGIN
#undef OS_CONFIG_RESOURCE_BEGIN
#endif
#ifdef OS_CONFIG_RESOURCE_DEF
#undef OS_CONFIG_RESOURCE_DEF
#endif
#ifdef OS_CONFIG_RESOURCE_END
#undef OS_CONFIG_RESOURCE_END
#endif

/* Generate documentation */
#ifdef __DOXYGEN__
/**
 * @brief   Beginning of task definitions
 */
#define OS_CONFIG_TASK_BEGIN

/**
 * @brief   Task definition
 *
 * @param   Name                    Name of task
 * @param   Prio                    Static priority of task
 * @param   StackSize               Size of task stack in bytes (maximum of UINT16_MAX)
 * @param   NumberOfActivations     Number of times the task can be activated
 * @param   Autostart               Set task to autostart (::OsTaskAutostart)
 * @param   TaskType                Type of task (::OsTaskType)
 * @param   TaskSchedule            Type of task scheduling (::OsTaskSchedule)
 *
 * This will create a new task and all required data structures. Each task will need a function TASK(Name).
 */
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, Autostart, TaskType, TaskSchedule)

/**
 * @brief   Ending of task definitions
 */
#define OS_CONFIG_TASK_END

/**
 * @brief   Count of tasks defined
 */
#define TASK_COUNT

/**
 * @brief   Beginning of interrupt definitions
 */
#define OS_CONFIG_INT_BEGIN

/**
 * @brief   Interrupt definition
 *
 * @param   Name    Name of the interrupt
 *
 * This will create a new interrupt and declare its ISR. Each interrupt will need a function ISR(Name).
 */
#define OS_CONFIG_INT_DEF(Name)

/**
 * @brief   Ending of interrupt definitions
 */
#define OS_CONFIG_INT_END

/**
 * @brief   Beginning of resource definitions
 */
#define OS_CONFIG_RESOURCE_BEGIN

/**
 * @brief   Resource definition
 */
#define OS_CONFIG_RESOURCE_DEF(Name, Prio, IsrAllowed)

/**
 * @brief   End of resource definitions
 */
#define OS_CONFIG_RESOURCE_END

/**
 * @brief   Count of resources defined
 */
#define RESOURCE_COUNT

#endif /* __DOXYGEN__ */


/* Generate enumerations based on config */
#ifdef OS_CONFIG_GEN_ENUM

#define OS_CONFIG_TASK_BEGIN                                                                                enum tasks_e {
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, Autostart, TaskType, TaskSchedule)   Name,
#define OS_CONFIG_TASK_END                                                                                  INVALID_TASK};

#define TASK_COUNT     INVALID_TASK

#define OS_CONFIG_INT_BEGIN
#define OS_CONFIG_INT_DEF(Name)
#define OS_CONFIG_INT_END

#define OS_CONFIG_RESOURCE_BEGIN                                                                            enum resources_e {
#define OS_CONFIG_RESOURCE_DEF(Name, Prio, IsrAllowed)                                                      Name,
#define OS_CONFIG_RESOURCE_END                                                                              INVALID_RESOURCE};

#define RESOURCE_COUNT  INVALID_RESOURCE

#endif /* OS_CONFIG_GEN_ENUM */

/* Generate function declarations based on config */
#ifdef OS_CONFIG_GEN_FUNC_DECL

#define OS_CONFIG_TASK_BEGIN
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, Autostart, TaskType, TaskSchedule)   TASK(Name);
#define OS_CONFIG_TASK_END

#define OS_CONFIG_INT_BEGIN
#define OS_CONFIG_INT_DEF(Name)                                                                             extern void Func ## Name(void);
#define OS_CONFIG_INT_END

#define OS_CONFIG_RESOURCE_BEGIN
#define OS_CONFIG_RESOURCE_DEF(Name, Prio, IsrAllowed)
#define OS_CONFIG_RESOURCE_END

#endif /* OS_CONFIG_GEN_FUNC_DECL */

/* Generate functions based on config */
#ifdef OS_CONFIG_GEN_FUNC

#define OS_CONFIG_TASK_BEGIN
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, Autostart, TaskType, TaskSchedule)
#define OS_CONFIG_TASK_END

#define OS_CONFIG_INT_BEGIN
#define OS_CONFIG_INT_DEF(Name)                                                                             ISR(Name) { \
                                                                                                                isISR = true; \
                                                                                                                isCat2ISR = true; \
                                                                                                                Func ## Name(); \
                                                                                                                isISR = false; \
                                                                                                                isCat2ISR = false; \
                                                                                                            }
#define OS_CONFIG_INT_END

#define OS_CONFIG_RESOURCE_BEGIN
#define OS_CONFIG_RESOURCE_DEF(Name, Prio, IsrAllowed)
#define OS_CONFIG_RESOURCE_END

#endif /* OS_CONFIG_GEN_FUNC */

/* Generate data structures based on config */
#ifdef OS_CONFIG_GEN_DATA_STRUCT

#define OS_CONFIG_TASK_BEGIN
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, Autostart, TaskType, TaskSchedule)   uint8_t Task##Name##_stack[StackSize]; \
                                                                                                            volatile struct task_s Task##Name##_s = { \
                                                                                                                .stack = (uint8_t* const) &Task##Name##_stack, \
                                                                                                                .stackSize = StackSize, \
                                                                                                                .prio = Prio, \
                                                                                                                .numberOfActivations = NumberOfActivations, \
                                                                                                                .autostart = Autostart, \
                                                                                                                .taskType = TaskType, \
                                                                                                                .taskSchedule = TaskSchedule, \
                                                                                                                .taskFxn = PTASK(Name), \
                                                                                                                .context = Task##Name##_stack + StackSize - 1, \
                                                                                                                .curPrio = Prio, \
                                                                                                                .curState = SUSPENDED, \
                                                                                                                .curStackUse = 0, \
                                                                                                                .maxStackUse = 0, \
                                                                                                                .resourceQueue = NULL \
                                                                                                            };
#define OS_CONFIG_TASK_END

#define OS_CONFIG_INT_BEGIN
#define OS_CONFIG_INT_DEF(Name)
#define OS_CONFIG_INT_END

#define OS_CONFIG_RESOURCE_BEGIN
#define OS_CONFIG_RESOURCE_DEF(Name, Prio, IsrAllowed)                                                      volatile struct resource_s Resource##Name##_s = { \
                                                                                                                .prio = Prio, \
                                                                                                                .assigned = false, \
                                                                                                                .isrAllowed = IsrAllowed, \
                                                                                                                .next = NULL \
                                                                                                            };
#define OS_CONFIG_RESOURCE_END

#endif /* OS_CONFIG_GEN_DATA_STRUCT */

/* Generate OS Task Control Block */
#ifdef OS_CONFIG_GEN_TCB

#define OS_CONFIG_TASK_BEGIN                                                                                volatile struct task_s* TCB_Cfg[TASK_COUNT] = {
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, Autostart, TaskType, TaskSchedule)   &Task##Name##_s,
#define OS_CONFIG_TASK_END                                                                                  };

#define OS_CONFIG_INT_BEGIN
#define OS_CONFIG_INT_DEF(Name)
#define OS_CONFIG_INT_END

#define OS_CONFIG_RESOURCE_BEGIN                                                                            volatile struct resource_s* Res_Cfg[RESOURCE_COUNT] = {
#define OS_CONFIG_RESOURCE_DEF(Name, Prio, IsrAllowed)                                                      &Resource##Name##_s,
#define OS_CONFIG_RESOURCE_END                                                                              };

#endif /* OS_CONFIG_GEN_TCB */

/* Undefine current generation defines */
#ifdef OS_CONFIG_GEN_ENUM
#undef OS_CONFIG_GEN_ENUM
#endif
#ifdef OS_CONFIG_GEN_FUNC_DECL
#undef OS_CONFIG_GEN_FUNC_DECL
#endif
#ifdef OS_CONFIG_GEN_DATA_STRUCT
#undef OS_CONFIG_GEN_DATA_STRUCT
#endif