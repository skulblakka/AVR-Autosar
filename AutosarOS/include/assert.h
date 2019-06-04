/*
* assert.h
*
* Created: 04.06.2019 19:10:48
*  Author: Pascal Romahn
*/


#ifndef ASSERT_H_
#define ASSERT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__DOXYGEN__)
/**
* @def     assert
* @param   expression    Expression to test for
*
* The assert() macro tests the given expression and if it is false,
* the calling process is terminated.  A diagnostic message is written
* to the error buffer and the function abort() is called, effectively
* terminating the program.
*
* If expression is true, the assert() macro does nothing.
*
* The assert() macro may be removed at compile time by defining
* NDEBUG as a macro (e.g., by using the compiler option -DNDEBUG).
*/
#define assert(expression)

/**
* @def     assertMsg
* @param   expression      Expression to test for
* @param   message         Message to write instead of expression
*
* The assert() macro tests the given expression and if it is false,
* the calling process is terminated.  The passed message is written
* to the error buffer and the function abort() is called, effectively
* terminating the program.
*
* If expression is true, the assert() macro does nothing.
*
* The assertMsg() macro may be removed at compile time by defining
* NDEBUG as a macro (e.g., by using the compiler option -DNDEBUG).
*/
#define assertMsg(expression, message)

/**
* @def     ASSERT_BUFFER_SIZE
* @param   size    Size of buffer in bytes
*
* A buffer of the specified size will be created to write the error message
* into. If the message is longer than the buffer no message will be created.
* 
* If not defined no buffer will be created.
*/
#define ASSERT_BUFFER_SIZE size

/**
* @def     STATIC_ASSERT_BUFFER
*
* If defined the error buffer will be declared as static.
*/
#define STATIC_ASSERT_BUFFER

/**
* @def     BREAK_ON_ASSERT
*
* If this is defined a break opcode will be executed before abort()
* is called. This way the debugger will break without explicitly
* setting a breakpoint.
*/
#define BREAK_ON_ASSERT

#else /* !DOXYGEN */

#ifdef NDEBUG
#define assert(exp)         ((void)0)
#else
#define assert(exp)         ((exp) ? (void)0 : __assert(__FILE__, __LINE__, #exp))
#define assertMsg(exp, msg) ((exp) ? (void)0 : __assert(__FILE__, __LINE__, msg))
#endif /* NDEBUG */


#ifdef __cplusplus
extern "C" {
#endif

    extern void __assert(const char* file, const int line, const char* msg);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* DOXYGEN */

#endif /* ASSERT_H_ */