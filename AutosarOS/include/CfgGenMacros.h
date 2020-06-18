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
#ifdef OS_CONFIG_COUNTER_BEGIN
#undef OS_CONFIG_COUNTER_BEGIN
#endif
#ifdef OS_CONFIG_COUNTER_DEF
#undef OS_CONFIG_COUNTER_DEF
#endif
#ifdef OS_CONFIG_COUNTER_END
#undef OS_CONFIG_COUNTER_END
#endif
#ifdef OS_CONFIG_ALARM_BEGIN
#undef OS_CONFIG_ALARM_BEGIN
#endif
#ifdef OS_CONFIG_ALARM_DEF
#undef OS_CONFIG_ALARM_DEF
#endif
#ifdef OS_CONFIG_ALARM_END
#undef OS_CONFIG_ALARM_END
#endif
#ifdef OS_CONFIG_ALARM_CALLBACK
#undef OS_CONFIG_ALARM_CALLBACK
#endif

/* Generate documentation */
#ifdef __DOXYGEN__

/**
 * @brief   Enable StartupHook
 *
 * If defined and set to true the StartupHook() will be called on system startup.
 */
#define OS_CONFIG_HOOK_STARTUP

/**
 * @brief   Enable ShutdownHook
 *
 * If defined and set to true the ShutdownHook() will be called on system shutdown.
 */
#define OS_CONFIG_HOOK_SHUTDOWN

/**
 * @brief   Enable PreTaskHook
 *
 * If defined and set to true the PreTaskHook() will be called after a new task enters
 * the running state.
 */
#define OS_CONFIG_HOOK_PRE_TASK

/**
 * @brief   Enable PostTaskHook
 *
 * If defined and set to true the PostTaskHook() will be called before a task leaves
 * the running state.
 */
#define OS_CONFIG_HOOK_POST_TASK

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
 * @param   Events                  Events for this task (only applicable if TaskType equals #EXTENDED)
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
 * Note that in this case interrupts will be skipped not deferred!
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

/**
 * @brief   Begin of counter definitions
 */
#define OS_CONFIG_COUNTER_BEGIN

/**
 * @brief   Counter definition
 *
 * @param   Name            Name of the counter
 * @param   MaxAllowedValue Maximum allowed counter value
 * @param   MinCycle        Smallest allowed value for the cycle-parameter of cyclic alarms
 * @param   TicksPerBase    Number of ticks required to reach a counter-specific unit
 * @param   Type            Type of the counter in #counterType_e
 * @param   SecondsPerTick  Time of one counter tick in seconds
 */
#define OS_CONFIG_COUNTER_DEF(Name, MaxAllowedValue, MinCycle, TicksPerBase, Type, SecondsPerTick)

/**
 * @brief   End of counter definitions
 */
#define OS_CONFIG_COUNTER_END

/**
 * @brief   Count of counters defined
 */
#define COUNTER_COUNT

/**
 * @brief   Begin of alarm definitions
 */
#define OS_CONFIG_ALARM_BEGIN

/**
 * @brief   Alarm definition
 *
 * Define an alarm.
 *
 * The value of Action must be according to the type of the alarm. \n
 * If Type is #ALARM_ACTION_TASK, Action must be the task to be activated. \n
 * If Type is #ALARM_ACTION_EVENT, Action must be the task of which the Event should be set. \n
 * If Type is #ALARM_ACTION_CALLBACK, Action must be a pointer to the callback function. \n
 * If Type is #ALARM_ACTION_COUNTER, Action must be the counter to be incremented.
 *
 * @param   Name            Name of the alarm
 * @param   Base            Counter used as alarm base
 * @param   AutoStart       Set alarm to autostart
 * @param   Event           Event to set (only applicable when Type is #ALARM_ACTION_EVENT)
 * @param   Type            Type of alarm (see #alarmActionType_e)
 * @param   Action          Action to be performed
 * @param   Expiration      Expiration value of alarm (must be between zero and the maximum allowed value of the base)
 * @param   Cycle           Cycle of the alarm (must be zero or between minCycle and maximum allowed value of the base)
 */
#define OS_CONFIG_ALARM_DEF(Name, Base, AutoStart, Event, Type, Action, Expiration, Cycle)

/**
 * @brief   End of alarm definitions
 */
#define OS_CONFIG_ALARM_END

/**
 * @brief   Count of alarm defined
 */
#define ALARM_COUNT

/**
 * @brief   Definition of alarm callback
 *
 * Define a callback function to be used with an alarm of #ALARM_ACTION_CALLBACK type. The definition must
 * must be made before the alarm is defined.
 *
 * @param   Name            Name of the callback function
 */
#define OS_CONFIG_ALARM_CALLBACK(Name)

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

#define OS_CONFIG_COUNTER_BEGIN                                                                                 enum counters_e { \
                                                                                                                SYSTEM_COUNTER,
#define OS_CONFIG_COUNTER_DEF(Name, MaxAllowedValue, MinCycle, TicksPerBase, Type, SecondsPerTick)              Name,
#define OS_CONFIG_COUNTER_END                                                                                   INVALID_COUNTER};

#define COUNTER_COUNT   INVALID_COUNTER

#define OS_CONFIG_ALARM_BEGIN                                                                                   enum alarm_e {
#define OS_CONFIG_ALARM_DEF(Name, Base, AutoStart, Event, Type, Action, Expiration, Cycle)                      Name,
#define OS_CONFIG_ALARM_END                                                                                     INVALID_ALARM};

#define ALARM_COUNT     INVALID_ALARM

#define OS_CONFIG_ALARM_CALLBACK(Name)

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

#define OS_CONFIG_COUNTER_BEGIN
#define OS_CONFIG_COUNTER_DEF(Name, MaxAllowedValue, MinCycle, TicksPerBase, Type, SecondsPerTick)
#define OS_CONFIG_COUNTER_END

#define OS_CONFIG_ALARM_BEGIN
#define OS_CONFIG_ALARM_DEF(Name, Base, AutoStart, Event, Type, Action, Expiration, Cycle)
#define OS_CONFIG_ALARM_END

#define OS_CONFIG_ALARM_CALLBACK(Name)                                                                          extern void Name(void);

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

#define OS_CONFIG_COUNTER_BEGIN
#define OS_CONFIG_COUNTER_DEF(Name, MaxAllowedValue, MinCycle, TicksPerBase, Type, SecondsPerTick)
#define OS_CONFIG_COUNTER_END

#define OS_CONFIG_ALARM_BEGIN
#define OS_CONFIG_ALARM_DEF(Name, Base, AutoStart, Event, Type, Action, Expiration, Cycle)
#define OS_CONFIG_ALARM_END

#define OS_CONFIG_ALARM_CALLBACK(Name)

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

#define OS_CONFIG_COUNTER_BEGIN                                                                                 volatile struct counter_s CounterSYSTEM_COUNTER_s = { \
                                                                                                                    .maxallowedvalue = UINT32_MAX, \
                                                                                                                    .mincycle = 1, \
                                                                                                                    .ticksperbase = 57, \
                                                                                                                    .type = HARDWARE, \
                                                                                                                    .secondspertick = 0.017778, \
                                                                                                                    .value = 0 \
                                                                                                                };
#define OS_CONFIG_COUNTER_DEF(Name, MaxAllowedValue, MinCycle, TicksPerBase, Type, SecondsPerTick)              volatile struct counter_s Counter##Name##_s = { \
                                                                                                                    .maxallowedvalue = MaxAllowedValue, \
                                                                                                                    .mincycle = MinCycle, \
                                                                                                                    .ticksperbase = TicksPerBase, \
                                                                                                                    .type = Type, \
                                                                                                                    .secondspertick = SecondsPerTick, \
                                                                                                                    .value = 0 \
                                                                                                                };
#define OS_CONFIG_COUNTER_END

#define OS_CONFIG_ALARM_BEGIN
#define OS_CONFIG_ALARM_DEF(Name, Base, AutoStart, Event, Type, Action, Expiration, Cycle)                      volatile struct alarm_s Alarm##Name##_s = { \
                                                                                                                    .alarmBase = &Counter##Base##_s, \
                                                                                                                    .event = Event, \
                                                                                                                    .actionType = Type, \
                                                                                                                    .action.action = (void*) Action, \
                                                                                                                    .running = AutoStart, \
                                                                                                                    .expiration = Expiration, \
                                                                                                                    .cycle = Cycle \
                                                                                                                };
#define OS_CONFIG_ALARM_END

#define OS_CONFIG_ALARM_CALLBACK(Name)

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

#define OS_CONFIG_COUNTER_BEGIN                                                                                 volatile struct counter_s* Counter_Cfg[COUNTER_COUNT] = { \
                                                                                                                &CounterSYSTEM_COUNTER_s,
#define OS_CONFIG_COUNTER_DEF(Name, MaxAllowedValue, MinCycle, TicksPerBase, Type, SecondsPerTick)              &Counter##Name##_s,
#define OS_CONFIG_COUNTER_END                                                                                   };

#define OS_CONFIG_ALARM_BEGIN                                                                                   volatile struct alarm_s* Alarm_Cfg[ALARM_COUNT] = {
#define OS_CONFIG_ALARM_DEF(Name, Base, AutoStart, Event, Type, Action, Expiration, Cycle)                      &Alarm##Name##_s,
#define OS_CONFIG_ALARM_END                                                                                     };

#define OS_CONFIG_ALARM_CALLBACK(Name)

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