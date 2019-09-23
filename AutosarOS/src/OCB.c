/**
 * @file
 *
 * @date    2019-09-02
 * @author  Pascal Romahn
 */

#include "OCB.h"
#include <avr/interrupt.h>

volatile bool isISR = false;
volatile bool isCat2ISR = false;

#define OS_CONFIG_GEN_DATA_STRUCT
#include "AppCfg.h"
#undef OS_CONFIG_GEN_DATA_STRUCT

#define OS_CONFIG_GEN_TCB
#include "AppCfg.h"
#undef OS_CONFIG_GEN_TCB

#define OS_CONFIG_GEN_FUNC
#include "AppCfg.h"
#undef OS_CONFIG_GEN_FUNC