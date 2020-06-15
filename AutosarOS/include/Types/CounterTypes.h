/**
 * @file
 *
 * @brief       Types used for counter management
 *
 * @date        2020-06-15
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */


#ifndef COUNTERTYPES_H_
#define COUNTERTYPES_H_

typedef uint64_t TickType;
typedef TickType* TickRefType;

enum counterType_e {
    HARDWARE,
    SOFTWARE
};

struct counter_s {
    TickType maxallowedvalue;
    TickType mincycle;
    TickType ticksperbase;
    enum counterType_e type;
    float secondspertick;
};



#endif /* COUNTERTYPES_H_ */