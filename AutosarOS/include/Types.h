/**
 * @file
 *
 * @brief   Type definitions
 *
 * @date    2019-09-10
 * @author  Pascal Romahn
 */ 


#ifndef TYPES_H_
#define TYPES_H_

// TODO: Location for these two might cause problems...
#undef ISR
#define ISR(Name) extern void Func ## Name(void)

typedef enum StatusType_e {
    E_OK = 0,
    E_OS_ACCESS,
    E_OS_CALLLEVEL,
    E_OS_ID,
    E_OS_LIMIT,
    E_OS_NOFUNC,
    E_OS_RESOURCE,
    E_OS_STATE,
    E_OS_VALUE
} StatusType;



#endif /* TYPES_H_ */