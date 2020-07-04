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
#ifdef OS_STACK_MONITORING
#undef OS_STACK_MONITORING
#endif
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
#ifdef OS_CONFIG_SCHEDULE_TABLE_BEGIN
#undef OS_CONFIG_SCHEDULE_TABLE_BEGIN
#endif
#ifdef OS_CONFIG_SCHEDULE_TABLE_DEF_BEGIN
#undef OS_CONFIG_SCHEDULE_TABLE_DEF_BEGIN
#endif
#ifdef OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_BEGIN
#undef OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_BEGIN
#endif
#ifdef OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_BEGIN
#undef OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_BEGIN
#endif
#ifdef OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_BEGIN
#undef OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_BEGIN
#endif
#ifdef OS_CONFIG_SCHEDULE_TABLE_TASK_ACTION_DEF
#undef OS_CONFIG_SCHEDULE_TABLE_TASK_ACTION_DEF
#endif
#ifdef OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_END
#undef OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_END
#endif
#ifdef OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_BEGIN
#undef OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_BEGIN
#endif
#ifdef OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTION_DEF
#undef OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTION_DEF
#endif
#ifdef OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_END
#undef OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_END
#endif
#ifdef OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_END
#undef OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_END
#endif
#ifdef OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_END
#undef OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_END
#endif
#ifdef OS_CONFIG_SCHEDULE_TABLE_DEF_END
#undef OS_CONFIG_SCHEDULE_TABLE_DEF_END
#endif
#ifdef OS_CONFIG_SCHEDULE_TABLE_END
#undef OS_CONFIG_SCHEDULE_TABLE_END
#endif
#ifdef OS_CONFIG_SYSTEM_STACK
#undef OS_CONFIG_SYSTEM_STACK
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
 * @brief   Enable ErrorHook
 *
 * If defined and set to true the ErrorHook() will be called when a system service
 * returns StatusType not equal E_OK.
 * It will also be called when an alarm expires  and an error is detected during task
 * activation or event setting.
 * It will not be called if a system service called from the ErrorHook does not
 * return E_OK.
 */
#define OS_CONFIG_HOOK_ERROR

/**
 * @brief   Enable ProtectionHook
 * 
 * If defined and set to true the ProtectionHook() will be called when any of the 
 * protection facilities (e.g. stack monitoring) detects an error.
 */
#define OS_CONFIG_HOOK_PROTECTION

/**
 * @brief   Enable extended mode
 *
 * If set to true the system will be compiled in extended mode with extended error checks
 * enabled.
 */
#define OS_CONFIG_EXTENDED

/**
 * @brief   Configure number of parameters saved for error information
 *
 * Configure the number of parameters saved for error information. Additional parameters
 * will not be saved.
 *
 * If set to zero only the service ID will be saved.
 *
 * If undefined or set to values below zero no information will be saved.
 *
 * @note    While possible to only save the service ID or even nothing at all the system
 *          is not conforming to the OSEK standard in that configuration.
 */
#define OS_CONFIG_MAX_ERROR_PARAM

/**
 * @brief   Configure stack monitoring
 *
 * Stack monitoring will check for task stack overrun on context switch. If a overrun is
 * detected ShutdownOS() will be called with #E_OS_STACKFAULT.
 *
 * The following values are possible:
 * * If not configured or set to 0 stack monitoring is disabled and related variables
 *   and code are removed to conserve memory.
 * * If set to >= 1 stack monitoring is performed by calculating the difference between
 *   the current stack pointer and the stack base. Note that this will only detect
 *   overruns if the stack pointer is still outside the stack region during the context
 *   switch.
 * * If set to >= 2 an additional marker will be placed above the stack top. This allows
 *   to detect overruns even if the stack pointer was moved back into the stack
 *   region before the context switch but requires additional memory.
 * * If set to >= 3 the stack will be initialized with 0xBE. This allows to analyze the
 *   memory sections during debugging.
 *
 * For values above zero the current and maximum stack use will be saved in the task
 * structure #task_s.
 *
 * Note that the above settings are only applied to task stacks. Stack monitoring for the
 * system stack is always enabled and will always use the marker method.
 */
#define OS_CONFIG_STACK_MONITORING

/**
 * @brief   Specify size of system stack
 * 
 * Specify the size of the system stack in bytes. The stack will be used during ISRs.
 * 
 * @param   Size                    Size of the stack created
 */
#define OS_CONFIG_SYSTEM_STACK(Size)

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
 * @param   Name                    Name of the interrupt
 * @param   Prio                    Priority of the interrupt (0 for Cat. 1 ISR; >0 for Cat. 2 ISR)
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
 * @param   Name                    Name of resource
 * @param   Prio                    Ceiling priority of resource
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
 * @param   Name                    Name of the resource
 * @param   Prio                    Ceiling priority of the resource
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
 * @param   Name                    Name of the counter
 * @param   MaxAllowedValue         Maximum allowed counter value
 * @param   MinCycle                Smallest allowed value for the cycle-parameter of cyclic alarms
 * @param   TicksPerBase            Number of ticks required to reach a counter-specific unit
 * @param   Type                    Type of the counter in #counterType_e
 * @param   SecondsPerTick          Time of one counter tick in seconds
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
 * @param   Name                    Name of the alarm
 * @param   Base                    Counter used as alarm base
 * @param   AutoStart               Set alarm to autostart
 * @param   Event                   Event to set (only applicable when Type is #ALARM_ACTION_EVENT)
 * @param   Type                    Type of alarm (see #alarmActionType_e)
 * @param   Action                  Action to be performed
 * @param   Expiration              Expiration value of alarm (must be between zero and the maximum allowed
 *                                  value of the base)
 * @param   Cycle                   Cycle of the alarm (must be zero or between minCycle and maximum allowed
 *                                  value of the base)
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
 * @param   Name                    Name of the callback function
 */
#define OS_CONFIG_ALARM_CALLBACK(Name)

/**
 * @brief   Beginning of schedule tables definitions
 */
#define OS_CONFIG_SCHEDULE_TABLE_BEGIN

/**
 * @brief   Beginning schedule table definition
 *
 * @param   Name                    Name of the schedule table
 * @param   Counter                 Counter driving the schedule table
 * @param   AutoStart               Set schedule table to autostart
 * @param   Cyclic                  Set schedule table to cyclic
 * @param   FinalDelay              Delay applied after the final expiry point
 */
#define OS_CONFIG_SCHEDULE_TABLE_DEF_BEGIN(Name, Counter, AutoStart, Cyclic, FinalDelay)

/**
 * @brief   Beginning of expiry points definition
 *
 * This starts the definition of expiry point of a schedule table and must therefore be placed between
 * #OS_CONFIG_SCHEDULE_TABLE_DEF_BEGIN and #OS_CONFIG_SCHEDULE_TABLE_DEF_END.
 *
 * The individual schedule points must defined in ascending order. Only one expiry point per offset
 * is allowed per schedule table.
 *
 * Each schedule table needs at least one expiry point.
 *
 * @param   NumExpiryPoints         Number of expiry points to create for schedule table
 */
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_BEGIN(NumExpiryPoints)

/**
 * @brief   Beginning of expiry point definition
 *
 * This starts the definition of a single expiry point and must therefore be placed between
 * #OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_BEGIN and #OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_END.
 *
 * @note    While this implementation allows both the task actions and the event actions to be zero
 *          at the same time the system does not conform to the AUTOSAR standard it that configuration.
 *
 * @param   Offset                  Offset of the expiry point
 * @param   NumTaskActions          Number of task actions to create for expiry point
 * @param   NumEventActions         Number of event actions to create for expiry point
 */
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_BEGIN(Offset, NumTaskActions, NumEventActions)

/**
 * @brief   Beginning of task actions definitions
 *
 * This starts the definition of an expiry points task actions and must therefore be place between
 * #OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_BEGIN and #OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_END.
 *
 * This and #OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_END should be omitted if no task actions are defined.
 */
#define OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_BEGIN

/**
 * @brief   Definition of task action
 *
 * Defines a task activation action for the expiry point causing the task to be activated once the offset of
 * the expiry point is reached by the schedule table.
 *
 * @param   Task                    Task to activate
 */
#define OS_CONFIG_SCHEDULE_TABLE_TASK_ACTION_DEF(Task)

/**
 * @brief   Ending of task actions definitions
 *
 * This ends the definition of an expiry points task actions and is the counterpart to
 * #OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_BEGIN.
 */
#define OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_END

/**
 * @brief   Beginning of event actions definitions
 *
 * This starts the definition of an expiry points event actions and must therefore be place between
 * #OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_BEGIN and #OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_END.
 *
 * This and #OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_END should be omitted if no event actions are defined.
 */
#define OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_BEGIN

/**
 * @brief   Definition of event action
 *
 * Defines an event action for the expiry point causing the specified event being set for the task once
 * the offset of the expiry point is reached by the schedule table.
 */
#define OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTION_DEF(Task, Event)

/**
 * @brief   Ending of event actions definitions
 *
 * This ends the definition of an expiry points event actions and is the counterpart to
 * #OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_BEGIN.
 */
#define OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_END

/**
 * @brief   Ending of expiry point definition
 *
 * This ends the definition of an expiry point. This is the counterpart to
 * #OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_BEGIN
 */
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_END

/**
 * @brief   Ending of expiry points definitions
 *
 * This ends the list of expiry points of the schedule table. This is the counterpart to
 * #OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_BEGIN.
 */
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_END

/**
 * @brief   Ending of schedule table definition
 *
 * This is the end of the schedule table definition. This is the counterpart to
 * #OS_CONFIG_SCHEDULE_TABLE_DEF_BEGIN.
 */
#define OS_CONFIG_SCHEDULE_TABLE_DEF_END

/**
 * @brief   End of schedule table definitions
 */
#define OS_CONFIG_SCHEDULE_TABLE_END

#endif /* __DOXYGEN__ */


/* Generate enumerations based on config */
#ifdef OS_CONFIG_GEN_ENUM

#define OS_CONFIG_SYSTEM_STACK(Size)

#define OS_CONFIG_TASK_BEGIN                                                    enum tasks_e {
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, \
                            Autostart, TaskType, TaskSchedule, Res, Events)     Name,
#define OS_CONFIG_TASK_END                                                      INVALID_TASK};

#define TASK_COUNT     INVALID_TASK

#define OS_CONFIG_INT_BEGIN
#define OS_CONFIG_INT_DEF(Name, Prio)
#define OS_CONFIG_INT_END

#define OS_CONFIG_RESOURCE_BEGIN                                                enum resources_e {
#define OS_CONFIG_RESOURCE_DEF(Name, Prio)                                      Name,
#define OS_CONFIG_RESOURCE_END                                                  INVALID_RESOURCE};

#define RESOURCE_COUNT  INVALID_RESOURCE

#define OS_CONFIG_INTERNAL_RESOURCE_BEGIN
#define OS_CONFIG_INTERNAL_RESOURCE_DEF(Name, Prio)
#define OS_CONFIG_INTERNAL_RESOURCE_END

#define OS_CONFIG_COUNTER_BEGIN                                                 enum counters_e { \
                                                                                SYSTEM_COUNTER,
#define OS_CONFIG_COUNTER_DEF(Name, MaxAllowedValue, MinCycle, \
                                TicksPerBase, Type, SecondsPerTick)             Name,
#define OS_CONFIG_COUNTER_END                                                   INVALID_COUNTER};

#define COUNTER_COUNT   INVALID_COUNTER

#define OS_CONFIG_ALARM_BEGIN                                                   enum alarm_e {
#define OS_CONFIG_ALARM_DEF(Name, Base, AutoStart, Event, Type, \
                                Action, Expiration, Cycle)                      Name,
#define OS_CONFIG_ALARM_END                                                     INVALID_ALARM};

#define ALARM_COUNT     INVALID_ALARM

#define OS_CONFIG_ALARM_CALLBACK(Name)

#define OS_CONFIG_SCHEDULE_TABLE_BEGIN                                          enum scheduleTables_e {
#define OS_CONFIG_SCHEDULE_TABLE_DEF_BEGIN(Name, Counter, AutoStart, \
                                            Cyclic, FinalDelay)                 Name,
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_BEGIN(NumExpiryPoints)
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_BEGIN(Offset, NumTaskActions, \
                                                            NumEventActions)
#define OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_BEGIN
#define OS_CONFIG_SCHEDULE_TABLE_TASK_ACTION_DEF(Task)
#define OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_END
#define OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_BEGIN
#define OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTION_DEF(Task, Event)
#define OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_END
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_END
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_END
#define OS_CONFIG_SCHEDULE_TABLE_DEF_END
#define OS_CONFIG_SCHEDULE_TABLE_END                                            INVALID_SCHEDULETABLE};

#define SCHEDULETABLE_COUNT     INVALID_SCHEDULETABLE

#endif /* OS_CONFIG_GEN_ENUM */

/* Generate function declarations based on config */
#ifdef OS_CONFIG_GEN_FUNC_DECL

#define OS_CONFIG_SYSTEM_STACK(Size)

#define OS_CONFIG_TASK_BEGIN
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, \
                            Autostart, TaskType, TaskSchedule, Res, Events)     TASK(Name);
#define OS_CONFIG_TASK_END

#define OS_CONFIG_INT_BEGIN
#define OS_CONFIG_INT_DEF(Name, Prio)                                           extern void Func ## Name(void);
#define OS_CONFIG_INT_END

#define OS_CONFIG_RESOURCE_BEGIN
#define OS_CONFIG_RESOURCE_DEF(Name, Prio)
#define OS_CONFIG_RESOURCE_END

#define OS_CONFIG_INTERNAL_RESOURCE_BEGIN
#define OS_CONFIG_INTERNAL_RESOURCE_DEF(Name, Prio)
#define OS_CONFIG_INTERNAL_RESOURCE_END

#define OS_CONFIG_COUNTER_BEGIN
#define OS_CONFIG_COUNTER_DEF(Name, MaxAllowedValue, MinCycle, \
                                TicksPerBase, Type, SecondsPerTick)
#define OS_CONFIG_COUNTER_END

#define OS_CONFIG_ALARM_BEGIN
#define OS_CONFIG_ALARM_DEF(Name, Base, AutoStart, Event, Type, \
                                Action, Expiration, Cycle)
#define OS_CONFIG_ALARM_END

#define OS_CONFIG_ALARM_CALLBACK(Name)                                          extern void Name(void);

#define OS_CONFIG_SCHEDULE_TABLE_BEGIN
#define OS_CONFIG_SCHEDULE_TABLE_DEF_BEGIN(Name, Counter, AutoStart, \
                                            Cyclic, FinalDelay)
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_BEGIN(NumExpiryPoints)
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_BEGIN(Offset, NumTaskActions, \
                                                            NumEventActions)
#define OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_BEGIN
#define OS_CONFIG_SCHEDULE_TABLE_TASK_ACTION_DEF(Task)
#define OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_END
#define OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_BEGIN
#define OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTION_DEF(Task, Event)
#define OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_END
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_END
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_END
#define OS_CONFIG_SCHEDULE_TABLE_DEF_END
#define OS_CONFIG_SCHEDULE_TABLE_END

#endif /* OS_CONFIG_GEN_FUNC_DECL */

/* Generate functions based on config */
#ifdef OS_CONFIG_GEN_FUNC

#define OS_CONFIG_SYSTEM_STACK(Size)

#define OS_CONFIG_TASK_BEGIN
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, \
                            Autostart, TaskType, TaskSchedule, Res, Events)
#define OS_CONFIG_TASK_END

#define OS_CONFIG_INT_BEGIN
#define OS_CONFIG_INT_DEF(Name, Prio)                                           ISR(Name, ISR_NAKED) { \
                                                                                    save_context(); \
                                                                                    OS_SystemStack[0] = 0xBE; \
                                                                                    SP = (uint16_t) OS_SystemStackPtr; \
                                                                                    isISR = true; \
                                                                                    isCat2ISR = Prio; \
                                                                                    if (currentTask == INVALID_TASK || Prio == 0 \
                                                                                            || Prio > TCB_Cfg[currentTask]->curPrio) \
                                                                                        Func ## Name(); \
                                                                                    isISR = false; \
                                                                                    isCat2ISR = 0; \
                                                                                    if (OS_SystemStack[0] != 0xBE) \
                                                                                        OS_ProtectionHookInternal(E_OS_STACKFAULT); \
                                                                                    restore_context(); \
                                                                                    asm volatile("reti"); \
                                                                                }
#define OS_CONFIG_INT_END

#define OS_CONFIG_RESOURCE_BEGIN
#define OS_CONFIG_RESOURCE_DEF(Name, Prio)
#define OS_CONFIG_RESOURCE_END

#define OS_CONFIG_INTERNAL_RESOURCE_BEGIN
#define OS_CONFIG_INTERNAL_RESOURCE_DEF(Name, Prio)
#define OS_CONFIG_INTERNAL_RESOURCE_END

#define OS_CONFIG_COUNTER_BEGIN
#define OS_CONFIG_COUNTER_DEF(Name, MaxAllowedValue, MinCycle, \
                                TicksPerBase, Type, SecondsPerTick)
#define OS_CONFIG_COUNTER_END

#define OS_CONFIG_ALARM_BEGIN
#define OS_CONFIG_ALARM_DEF(Name, Base, AutoStart, Event, Type, \
                                Action, Expiration, Cycle)
#define OS_CONFIG_ALARM_END

#define OS_CONFIG_ALARM_CALLBACK(Name)

#define OS_CONFIG_SCHEDULE_TABLE_BEGIN
#define OS_CONFIG_SCHEDULE_TABLE_DEF_BEGIN(Name, Counter, AutoStart, \
                                            Cyclic, FinalDelay)
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_BEGIN(NumExpiryPoints)
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_BEGIN(Offset, NumTaskActions, \
                                                            NumEventActions)
#define OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_BEGIN
#define OS_CONFIG_SCHEDULE_TABLE_TASK_ACTION_DEF(Task)
#define OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_END
#define OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_BEGIN
#define OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTION_DEF(Task, Event)
#define OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_END
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_END
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_END
#define OS_CONFIG_SCHEDULE_TABLE_DEF_END
#define OS_CONFIG_SCHEDULE_TABLE_END

#endif /* OS_CONFIG_GEN_FUNC */

/* Generate data structures based on config */
#ifdef OS_CONFIG_GEN_DATA_STRUCT

#if defined (OS_CONFIG_STACK_MONITORING) && OS_CONFIG_STACK_MONITORING >= 2
#define OS_STACK_MONITORING_MARKER_SIZE                                         1
#else
#define OS_STACK_MONITORING_MARKER_SIZE                                         0
#endif

#define OS_CONFIG_SYSTEM_STACK(Size)                                            uint8_t OS_SystemStack[Size + 1]; \
                                                                                uint8_t* const OS_SystemStackPtr = OS_SystemStack + sizeof(OS_SystemStack) - 1;

#define OS_CONFIG_TASK_BEGIN
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, \
                            Autostart, TaskType, TaskSchedule, Res, Events)     uint8_t Task##Name##_stack[StackSize + OS_STACK_MONITORING_MARKER_SIZE]; \
                                                                                volatile struct task_s Task##Name##_s = { \
                                                                                    .stack = ((uint8_t* const) &Task##Name##_stack) + OS_STACK_MONITORING_MARKER_SIZE, \
                                                                                    .stackSize = StackSize, \
                                                                                    .prio = Prio, \
                                                                                    .numberOfActivations = NumberOfActivations, \
                                                                                    .autostart = Autostart, \
                                                                                    .taskType = TaskType, \
                                                                                    .taskSchedule = TaskSchedule, \
                                                                                    .taskFxn = PTASK(Name), \
                                                                                    .context = Task##Name##_stack + StackSize - 1 + OS_STACK_MONITORING_MARKER_SIZE, \
                                                                                    .curPrio = Prio, \
                                                                                    .curState = SUSPENDED, \
                                                                                    .resourceQueue = NULL, \
                                                                                    .internalResource = &IntResource##Res##_s, \
                                                                                    .events = Events, \
                                                                                };
#define OS_CONFIG_TASK_END

#define OS_CONFIG_INT_BEGIN
#define OS_CONFIG_INT_DEF(Name, Prio)
#define OS_CONFIG_INT_END

#define OS_CONFIG_RESOURCE_BEGIN
#define OS_CONFIG_RESOURCE_DEF(Name, Prio)                                      volatile struct resource_s Resource##Name##_s = { \
                                                                                    .prio = Prio, \
                                                                                    .assigned = false, \
                                                                                    .next = NULL \
                                                                                };
#define OS_CONFIG_RESOURCE_END

#define OS_CONFIG_INTERNAL_RESOURCE_BEGIN                                       volatile struct internalResource_s IntResourceNULL_s;
#define OS_CONFIG_INTERNAL_RESOURCE_DEF(Name, Prio)                             volatile struct internalResource_s IntResource##Name##_s = { \
                                                                                    .prio = Prio, \
                                                                                    .assigned = false \
                                                                                };
#define OS_CONFIG_INTERNAL_RESOURCE_END

#define OS_CONFIG_COUNTER_BEGIN                                                 volatile struct counter_s CounterSYSTEM_COUNTER_s = { \
                                                                                    .maxallowedvalue = UINT32_MAX, \
                                                                                    .mincycle = 1, \
                                                                                    .ticksperbase = 57, \
                                                                                    .type = HARDWARE, \
                                                                                    .secondspertick = 0.017778, \
                                                                                    .value = 0 \
                                                                                };
#define OS_CONFIG_COUNTER_DEF(Name, MaxAllowedValue, MinCycle, \
                                TicksPerBase, Type, SecondsPerTick)             volatile struct counter_s Counter##Name##_s = { \
                                                                                    .maxallowedvalue = MaxAllowedValue, \
                                                                                    .mincycle = MinCycle, \
                                                                                    .ticksperbase = TicksPerBase, \
                                                                                    .type = Type, \
                                                                                    .secondspertick = SecondsPerTick, \
                                                                                    .value = 0 \
                                                                                };
#define OS_CONFIG_COUNTER_END

#define OS_CONFIG_ALARM_BEGIN
#define OS_CONFIG_ALARM_DEF(Name, Base, AutoStart, Event, Type, \
                             Action, Expiration, Cycle)                         volatile struct alarm_s Alarm##Name##_s = { \
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

#define OS_CONFIG_SCHEDULE_TABLE_BEGIN
#define OS_CONFIG_SCHEDULE_TABLE_DEF_BEGIN(Name, Counter, AutoStart, \
                                            Cyclic, FinalDelay)                 struct scheduleTable_s ScheduleTable##Name##_s = { \
                                                                                    .counter = Counter, \
                                                                                    .autoStart = AutoStart, \
                                                                                    .cyclic = Cyclic, \
                                                                                    .finalDelay = FinalDelay, \
                                                                                    .currentState = SCHEDULETABLE_STOPPED, \
                                                                                    .next = INVALID_SCHEDULETABLE,
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_BEGIN(NumExpiryPoints)               .numExpiryPoints = NumExpiryPoints, \
                                                                                    .expiryPointList = (struct scheduleTableExpiryPoint_s[]) {
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_BEGIN(Offset, NumTaskActions, \
                                                            NumEventActions)            { .offset = Offset, \
                                                                                          .numTaskActions = NumTaskActions, \
                                                                                          .numEventActions = NumEventActions,
#define OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_BEGIN                                       .taskActionList = (struct scheduleTableExpiryActionTask_s[]) {
#define OS_CONFIG_SCHEDULE_TABLE_TASK_ACTION_DEF(Task)                                        { .task = Task },
#define OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_END                                         },
#define OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_BEGIN                                      .eventActionList = (struct scheduleTableExpiryActionEvent_s[]) {
#define OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTION_DEF(Task, Event)                                { .task = Task, .event = Event },
#define OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_END                                        },
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_END                                   },
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_END                                  },
#define OS_CONFIG_SCHEDULE_TABLE_DEF_END                                        };
#define OS_CONFIG_SCHEDULE_TABLE_END

#endif /* OS_CONFIG_GEN_DATA_STRUCT */

/* Generate OS Task Control Block */
#ifdef OS_CONFIG_GEN_TCB

#define OS_CONFIG_SYSTEM_STACK(Size)

#define OS_CONFIG_TASK_BEGIN                                                    volatile struct task_s* TCB_Cfg[TASK_COUNT] = {
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, \
                            Autostart, TaskType, TaskSchedule, Res, Events)     &Task##Name##_s,
#define OS_CONFIG_TASK_END                                                      };

#define OS_CONFIG_INT_BEGIN
#define OS_CONFIG_INT_DEF(Name, Prio)
#define OS_CONFIG_INT_END

#define OS_CONFIG_RESOURCE_BEGIN                                                volatile struct resource_s* Res_Cfg[RESOURCE_COUNT] = {
#define OS_CONFIG_RESOURCE_DEF(Name, Prio)                                      &Resource##Name##_s,
#define OS_CONFIG_RESOURCE_END                                                  };

#define OS_CONFIG_INTERNAL_RESOURCE_BEGIN
#define OS_CONFIG_INTERNAL_RESOURCE_DEF(Name, Prio)
#define OS_CONFIG_INTERNAL_RESOURCE_END

#define OS_CONFIG_COUNTER_BEGIN                                                 volatile struct counter_s* Counter_Cfg[COUNTER_COUNT] = { \
                                                                                &CounterSYSTEM_COUNTER_s,
#define OS_CONFIG_COUNTER_DEF(Name, MaxAllowedValue, MinCycle, \
                                TicksPerBase, Type, SecondsPerTick)             &Counter##Name##_s,
#define OS_CONFIG_COUNTER_END                                                   };

#define OS_CONFIG_ALARM_BEGIN                                                   volatile struct alarm_s* Alarm_Cfg[ALARM_COUNT] = {
#define OS_CONFIG_ALARM_DEF(Name, Base, AutoStart, Event, Type, \
                                Action, Expiration, Cycle)                      &Alarm##Name##_s,
#define OS_CONFIG_ALARM_END                                                     };

#define OS_CONFIG_ALARM_CALLBACK(Name)

#define OS_CONFIG_SCHEDULE_TABLE_BEGIN                                          struct scheduleTable_s* ScheduleTable_Cfg[SCHEDULETABLE_COUNT] = {
#define OS_CONFIG_SCHEDULE_TABLE_DEF_BEGIN(Name, Counter, AutoStart, \
                                            Cyclic, FinalDelay)                 &ScheduleTable##Name##_s,
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_BEGIN(NumExpiryPoints)
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_BEGIN(Offset, NumTaskActions, \
                                                            NumEventActions)
#define OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_BEGIN
#define OS_CONFIG_SCHEDULE_TABLE_TASK_ACTION_DEF(Task)
#define OS_CONFIG_SCHEDULE_TABLE_TASK_ACTIONS_END
#define OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_BEGIN
#define OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTION_DEF(Task, Event)
#define OS_CONFIG_SCHEDULE_TABLE_EVENT_ACTIONS_END
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINT_DEF_END
#define OS_CONFIG_SCHEDULE_TABLE_EXPIRY_POINTS_END
#define OS_CONFIG_SCHEDULE_TABLE_DEF_END
#define OS_CONFIG_SCHEDULE_TABLE_END                                            };

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