/*
 * Types.h
 *
 * Created: 10.09.2019 15:14:25
 *  Author: pascal
 */


#ifndef TYPES_H_
#define TYPES_H_

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