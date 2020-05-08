/**
 * @file
 *
 * @date        2020-04-27
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */


#ifndef RESOURCETYPES_H_
#define RESOURCETYPES_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief   Data structure for OS resource
 */
struct resource_s {
    const uint8_t prio;         /**< Ceiling priority of resource */
    struct resource_s* next;    /**< Pointer to next resource in resource queue */
    const bool isrAllowed :1;   /**< If true the resource can be requested by ISRs */
    bool assigned :1;           /**< If true the resource is currently assigned to a task or ISR */
};

#endif /* RESOURCETYPES_H_ */