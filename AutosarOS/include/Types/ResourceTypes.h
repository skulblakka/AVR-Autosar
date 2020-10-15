/**
 * @file
 *
 * @brief       Types used for resources
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
 * @brief   Type for resource reference
 */
typedef enum resources_e ResourceType;

/**
 * @brief   Data structure for OS resource
 */
struct resource_s {
    const uint8_t prio;         /**< Ceiling priority of resource */
    bool assigned;              /**< If true the resource is currently assigned to a task or ISR */
    struct resource_s* next;    /**< Pointer to next resource in resource queue */
};

/**
 * @brief   Data structure for internal resource
 */
struct internalResource_s {
    const uint8_t prio;         /**< Ceiling priority of resource */
    bool assigned;              /**< If true the resource is currently assigned to a task */
};

#endif /* RESOURCETYPES_H_ */