# AVR-AUTOSAR

[AVR-AUTOSAR](https://github.com/skulblakka/AVR-Autosar) a real-time operating
system for AVR microcontrollers based on the
[OSEK/VDX OS](https://www.irisa.fr/alf/downloads/puaut/TPNXT/images/os223.pdf)
and [AUTOSAR OS 4.4](https://www.autosar.org/standards/classic-platform/classic-platform-440/) standards.

AVR-AUTOSAR implements the scalability class 1 of AUTOSAR OS.

## Motivation

The motivation for AVR-AUTOSAR was to create an easy-to-understand open-source
implementation of AUTOSAR OS that can be run on widely available microprocessors.

The project was develop as part of a final thesis at the University of Applied
Sciences Dortmund.

## Quickstart

The project was created with the Atmel Studio 7 and the current AVR8-GCC-Toolchain
on Windows. The solution and project files are included in the repository.

Configuration of the system is done in the `AppCfg.h`. The macros necessary are
defined and documented in `include/CfgGenMacros.h`. The documentation from the
current release can be found [here](https://skulblakka.github.io/AVR-Autosar/index.html).
To generate the current documentation the provided doxyfile can be used.

The application code is placed in `App.c`.

The repository contains an test application for ATmega1284 devices which was
used during development to test all the features of the operating system. It can
be used as an example. Additionally multiple applications are provided for testing
under `AutosarOS/test`.

## Supported Microprocessors

The operating system was tested on ATmega32, ATmega128 and ATmega1284. Other microprocessor of the ATmega-family should work with minimal work required.

## Known limitations

* The system does not use ready queues for tasks. If multiple tasks with the
  same priority get activated they will be selected based on their order in the
  configuration not on the time of activation.
* The system does not support hardware-based counters (instead a timer that
  increments a software-based counter must be used).
* The constants for the alarm base are not automatically created and must either
  be created by the user or `GetAlarmBase()` must be used.