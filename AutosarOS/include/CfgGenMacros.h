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
#ifdef OS_CONFIG_INTERNAL_RESOURCE_BEGIN
#undef OS_CONFIG_INTERNAL_RESOURCE_BEGIN
#endif
#ifdef OS_CONFIG_INTERNAL_RESOURCE_DEF
#undef OS_CONFIG_INTERNAL_RESOURCE_DEF
#endif
#ifdef OS_CONFIG_INTERNAL_RESOURCE_END
#undef OS_CONFIG_INTERNAL_RESOURCE_END
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
 * @param   Res                     Internal resource assigned to task
 * @param   Events                  Events for this task (only applicable if TaskType equals #EXTENDED_TASK)
 *
 * This will create a new task and all required data structures. Each task will need a function TASK(Name).
 */
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, Autostart, TaskType, TaskSchedule, Res, Events)

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
 * Define an interrupt service routine. The name must correspond to an interrupt vector of the processor.
 * The priority is only used for resource management and does not correspond to the priorities used
 * in handling the interrupt vectors. Category 1 ISRs must have have a priority of zero. Priority of category 2 ISRs
 * must be at least one.
 *
 * If the interrupt is triggered and the currently executed task has a higher priority the ISR will not be executed.
 *
 * @param   Name    Name of the interrupt
 * @param   Prio    Priority of the interrupt (0 for Cat. 1 ISR; >0 for Cat. 2 ISR)
 *
 * This will create a new interrupt and declare its ISR. Each interrupt will need a function ISR(Name).
 */
#define OS_CONFIG_INT_DEF(Name, Prio)

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
 *
 * @param   Name            Name of resource
 * @param   Prio            Ceiling priority of resource
 */
#define OS_CONFIG_RESOURCE_DEF(Name, Prio)

/**
 * @brief   End of resource definitions
 */
#define OS_CONFIG_RESOURCE_END

/**
 * @brief   Count of resources defined
 */
#define RESOURCE_COUNT

/**
 * @brief   Beginning of internal resource definitions
 */
#define OS_CONFIG_INTERNAL_RESOURCE_BEGIN

/**
 * @brief   Internal resource definition
 *
 * @param   Name            Name of the resource
 * @param   Prio            Ceiling priority of the resource
 */
#define OS_CONFIG_INTERNAL_RESOURCE_DEF(Name, Prio)

/**
 * @brief   End of internal resource definitions
 */
#define OS_CONFIG_INTERNAL_RESOURCE_END

#endif /* __DOXYGEN__ */


/* Generate enumerations based on config */
#ifdef OS_CONFIG_GEN_ENUM

#define OS_CONFIG_TASK_BEGIN                                                                                    enum tasks_e {
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, Autostart, TaskType, TaskSchedule, Res, Events)  Name,
#define OS_CONFIG_TASK_END                                                                                      INVALID_TASK};

#define TASK_COUNT     INVALID_TASK

#define OS_CONFIG_INT_BEGIN
#define OS_CONFIG_INT_DEF(Name, Prio)
#define OS_CONFIG_INT_END

#define OS_CONFIG_RESOURCE_BEGIN                                                                                enum resources_e {
#define OS_CONFIG_RESOURCE_DEF(Name, Prio)                                                                      Name,
#define OS_CONFIG_RESOURCE_END                                                                                  INVALID_RESOURCE};

#define RESOURCE_COUNT  INVALID_RESOURCE

#define OS_CONFIG_INTERNAL_RESOURCE_BEGIN
#define OS_CONFIG_INTERNAL_RESOURCE_DEF(Name, Prio)
#define OS_CONFIG_INTERNAL_RESOURCE_END

#endif /* OS_CONFIG_GEN_ENUM */

/* Generate function declarations based on config */
#ifdef OS_CONFIG_GEN_FUNC_DECL

#define OS_CONFIG_TASK_BEGIN
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, Autostart, TaskType, TaskSchedule, Res, Events)  TASK(Name);
#define OS_CONFIG_TASK_END

#define OS_CONFIG_INT_BEGIN
#define OS_CONFIG_INT_DEF(Name, Prio)                                                                           extern void Func ## Name(void);
#define OS_CONFIG_INT_END

#define OS_CONFIG_RESOURCE_BEGIN
#define OS_CONFIG_RESOURCE_DEF(Name, Prio)
#define OS_CONFIG_RESOURCE_END

#define OS_CONFIG_INTERNAL_RESOURCE_BEGIN
#define OS_CONFIG_INTERNAL_RESOURCE_DEF(Name, Prio)
#define OS_CONFIG_INTERNAL_RESOURCE_END

#endif /* OS_CONFIG_GEN_FUNC_DECL */

/* Generate functions based on config */
#ifdef OS_CONFIG_GEN_FUNC

#define OS_CONFIG_TASK_BEGIN
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, Autostart, TaskType, TaskSchedule, Res, Events)
#define OS_CONFIG_TASK_END

#define OS_CONFIG_INT_BEGIN
#define OS_CONFIG_INT_DEF(Name, Prio)                                                                           ISR(Name) { \
                                                                                                                    isISR = true; \
                                                                                                                    isCat2ISR = Prio; \
                                                                                                                    if (currentTask == INVALID_TASK || Prio == 0 || Prio > TCB_Cfg[currentTask]->curPrio) \
                                                                                                                        Func ## Name(); \
                                                                                                                    isISR = false; \
                                                                                                                    isCat2ISR = 0; \
                                                                                                                }
#define OS_CONFIG_INT_END

#define OS_CONFIG_RESOURCE_BEGIN
#define OS_CONFIG_RESOURCE_DEF(Name, Prio)
#define OS_CONFIG_RESOURCE_END

#define OS_CONFIG_INTERNAL_RESOURCE_BEGIN
#define OS_CONFIG_INTERNAL_RESOURCE_DEF(Name, Prio)
#define OS_CONFIG_INTERNAL_RESOURCE_END

#endif /* OS_CONFIG_GEN_FUNC */

/* Generate data structures based on config */
#ifdef OS_CONFIG_GEN_DATA_STRUCT

#define OS_CONFIG_TASK_BEGIN
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, Autostart, TaskType, TaskSchedule, Res, Events)  uint8_t Task##Name##_stack[StackSize]; \
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
                                                                                                                    .resourceQueue = NULL, \
                                                                                                                    .internalResource = &IntResource##Res##_s, \
                                                                                                                    .events = Events, \
                                                                                                                };
#define OS_CONFIG_TASK_END

#define OS_CONFIG_INT_BEGIN
#define OS_CONFIG_INT_DEF(Name, Prio)
#define OS_CONFIG_INT_END

#define OS_CONFIG_RESOURCE_BEGIN
#define OS_CONFIG_RESOURCE_DEF(Name, Prio)                                                                      volatile struct resource_s Resource##Name##_s = { \
                                                                                                                    .prio = Prio, \
                                                                                                                    .assigned = false, \
                                                                                                                    .next = NULL \
                                                                                                                };
#define OS_CONFIG_RESOURCE_END

#define OS_CONFIG_INTERNAL_RESOURCE_BEGIN                                                                       volatile struct internalResource_s IntResourceNULL_s;
#define OS_CONFIG_INTERNAL_RESOURCE_DEF(Name, Prio)                                                             volatile struct internalResource_s IntResource##Name##_s = { \
                                                                                                                    .prio = Prio, \
                                                                                                                    .assigned = false \
                                                                                                                };
#define OS_CONFIG_INTERNAL_RESOURCE_END

#endif /* OS_CONFIG_GEN_DATA_STRUCT */

/* Generate OS Task Control Block */
#ifdef OS_CONFIG_GEN_TCB

#define OS_CONFIG_TASK_BEGIN                                                                                    volatile struct task_s* TCB_Cfg[TASK_COUNT] = {
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, Autostart, TaskType, TaskSchedule, Res, Events)  &Task##Name##_s,
#define OS_CONFIG_TASK_END                                                                                      };

#define OS_CONFIG_INT_BEGIN
#define OS_CONFIG_INT_DEF(Name, Prio)
#define OS_CONFIG_INT_END

#define OS_CONFIG_RESOURCE_BEGIN                                                                                volatile struct resource_s* Res_Cfg[RESOURCE_COUNT] = {
#define OS_CONFIG_RESOURCE_DEF(Name, Prio)                                                                      &Resource##Name##_s,
#define OS_CONFIG_RESOURCE_END                                                                                  };

#define OS_CONFIG_INTERNAL_RESOURCE_BEGIN
#define OS_CONFIG_INTERNAL_RESOURCE_DEF(Name, Prio)
#define OS_CONFIG_INTERNAL_RESOURCE_END

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
#ifdef OS_CONFIG_GEN_TCB
#undef OS_CONFIG_GEN_TCB
#endif