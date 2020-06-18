/**
 * @file
 *
 * @brief       Main loop (Program entry point)
 *
 * @date        2019-09-02
 * @author      Pascal Romahn
 * @copyright   This program is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              (at your option) any later version.
 */

#include "OS.h"


int main(void)
{
    OS_StartOS(OSDEFAULTAPPMODE);

    while (1);
}