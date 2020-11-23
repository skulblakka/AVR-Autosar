# AVR-AUTOSAR

[AVR-AUTOSAR](https://github.com/skulblakka/AVR-Autosar) is a real-time operating
system for AVR microcontrollers based on the
[OSEK/VDX OS](https://www.irisa.fr/alf/downloads/puaut/TPNXT/images/os223.pdf)
and [AUTOSAR OS 4.4](https://www.autosar.org/standards/classic-platform/classic-platform-440/) standards.

AVR-AUTOSAR implements the scalability class 1 of AUTOSAR OS.

## Motivation

The motivation for AVR-AUTOSAR was to create an easy-to-understand open-source
implementation of AUTOSAR OS that can be run on widely available microprocessors.

The project was developed as part of a final thesis at the University of Applied
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

The operating system was tested with the following devices:
* ATmega32
* ATmega328P
* ATmega128
* ATmega1284
* ATmega2560

Other microprocessors of the ATmega-family should work with minimal work required.

## Tracing

During development a debugger-based system for tracing was used to record program flow.

The tracepoints set in Atmel Studio can be imported from the `tracing.xml` file.
Note that the tracepoints for the ISRs will probably not load correctly as they
are set on the user functions outside the operating system.

Additionally a data tracepoint can be set for `errorInfo.id` (write access) if
error information is used. The following print message can be used to generate
comments in the trace output which show what OS services are being called:

`# {currentTask};{isISR} called $FUNCTION`

The generated trace should look something like this:
```
#version 1.0
# INVALID_TASK;0 called OS_StartOS
898158796,Core_0,0,T,Idle,0,activate
898158921,Core_0,0,T,Task1,0,activate
898159328,Core_0,0,T,Task1,0,start
# Task1;0 called Events_SetEvent
# Task1;0 called Task_TerminateTask
898167984,Core_0,0,T,Task1,0,terminate
898169140,Core_0,0,T,Idle,0,start
```

The traces follow the "Better Trace Format" for OSEK and can be imported into
Eclipse Trace Compass.

_Note that the timestamps are generated on the host not the target! This means
that absolute timings can't be trusted. Therefore this method should only be
used to trace the program flow/order of execution._

## Known limitations

* The system does not use ready queues for tasks. If multiple tasks with the
  same priority get activated they will be selected based on their order in the
  configuration not on the time of activation.
* The system does not support hardware-based counters (instead a timer that
  increments a software-based counter must be used).
* The constants for the alarm base are not automatically created and must either
  be created by the user or `GetAlarmBase()` must be used.