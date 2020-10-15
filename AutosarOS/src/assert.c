/**
 * @file
 *
 * @brief       Assert functions
 *
 * @date        2019-06-04
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */

#include "assert.h"

extern void __assert(const char* file, const int line, const char* msg)
{
#ifdef ASSERT_BUFFER_SIZE
#ifdef STATIC_ASSERT_BUFFER
    static char error[ASSERT_BUFFER_SIZE];
#else /* STATIC_ASSERT_BUFFER */
    char error[ASSERT_BUFFER_SIZE];
#endif /* STATIC_ASSERT_BUFFER */

    char format[] = "Assertion failed in %s : Line %d : %s";

    // Disable warning because the address for error will always evaluate as 'true' if STATIC_ASSERT_BUFFER is defined
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waddress"
    if (strlen(msg) + strlen(format) + strlen(file) + 16 <= ASSERT_BUFFER_SIZE && error) {
        sprintf(error, format, file, line, msg);
    }
#pragma GCC diagnostic pop

#endif /* ASSERT_BUFFER_SIZE */

#ifdef BREAK_ON_ASSERT
    asm("BREAK");
#endif /* BREAK_ON_ASSERT */

    abort();
}