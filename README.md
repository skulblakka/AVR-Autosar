# AVR-AUTOSAR

This project contains an real-time operating system for AVR microcontrollers
based on the OSEK/VDX OS and AUTOSAR OS 4.2 standards.

The project was created with the Atmel Studio 7 and the current AVR8-GCC-Toolchain
on Windows.

## Setting up an application

Configuration of the system is done in the `AppCfg.h`. The macros necessary are defined and documented in `include/CfgGenMacros.h`.

The application code is placed in `App.c`.

_The repository contains an test application for ATmega1284 controllers. The test
application was used during development to test all the features of the operating
system._

## Supported Microprocessors

The operating system was tested on ATmega32, ATmega128 and ATmega1284. Other microprocessor of the ATmega-family should work with minimal work required.