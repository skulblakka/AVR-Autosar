/**
 * @file
 *
 * @date    2019-09-02
 * @author  Pascal Romahn
 */ 

#include "TCB.h"

#define OS_CONFIG_GEN_DATA_STRUCT
#include "AppCfg.h"
#undef OS_CONFIG_GEN_DATA_STRUCT

#define OS_CONFIG_GEN_TCB
#include "AppCfg.h"
#undef OS_CONFIG_GEN_TCB
