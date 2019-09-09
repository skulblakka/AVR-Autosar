/**
 * @file
 *
 * @brief   Macros used for generation of system
 *
 * @date    2019-09-02
 * @author  Pascal Romahn
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

/* Generate enumerations based on config */
#ifdef OS_CONFIG_GEN_ENUM

#define OS_CONFIG_TASK_BEGIN                                                                                enum tasks_e {
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, Autostart, TaskType, TaskSchedule)   Name,
#define OS_CONFIG_TASK_END                                                                                  INVALID_TASK};

#define TASK_COUNT     INVALID_TASK

#endif /* OS_CONFIG_GEN_ENUM */

/* Generate function declarations based on config */
#ifdef OS_CONFIG_GEN_FUNC_DECL

#define OS_CONFIG_TASK_BEGIN
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, Autostart, TaskType, TaskSchedule)   TASK(Name);
#define OS_CONFIG_TASK_END

#endif /* OS_CONFIG_GEN_FUNC_DECL */

/* Generate data structures based on config */
#ifdef OS_CONFIG_GEN_DATA_STRUCT

#define OS_CONFIG_TASK_BEGIN
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, Autostart, TaskType, TaskSchedule)   uint8_t Task##Name##_stack[StackSize]; \
                                                                                                            struct task_s Task##Name##_s = { \
                                                                                                                .stack = (uint8_t* const) &Task##Name##_stack, \
                                                                                                                .stackSize = StackSize, \
                                                                                                                .prio = Prio, \
                                                                                                                .numberOfActivations = NumberOfActivations, \
                                                                                                                .autostart = Autostart, \
                                                                                                                .taskType = TaskType, \
                                                                                                                .taskSchedule = TaskSchedule, \
                                                                                                                .taskFxn = PTASK(Name), \
                                                                                                                .context = Task##Name##_stack + StackSize, \
                                                                                                                .curPrio = Prio, \
                                                                                                                .curState = SUSPENDED \
                                                                                                            };
#define OS_CONFIG_TASK_END

#endif /* OS_CONFIG_GEN_DATA_STRUCT */

/* Generate OS Task Control Block */ // TODO: Rename
#ifdef OS_CONFIG_GEN_TCB

#define OS_CONFIG_TASK_BEGIN                                                                                volatile struct task_s* TCB_Cfg[TASK_COUNT + 1] = {
#define OS_CONFIG_TASK_DEF(Name, Prio, StackSize, NumberOfActivations, Autostart, TaskType, TaskSchedule)   &Task##Name##_s,
#define OS_CONFIG_TASK_END                                                                                  0};

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