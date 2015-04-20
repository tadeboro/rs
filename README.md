# Quick and easy cross-compilation setup

This repository contains files, needed in lab sessions in order to make
ARM development on STM32F4xx discovery board a bit nicer.

## Prerequisites

Obviously, one needs to have proper toolchain installed. On most
distributions, this toolchain can be obtained by installing couple of
`arm-none-eabi-*` packages from distribution repository. Note that
packages on fedora 21 are broken, so this will not work for stable
fedora at the moment.

Other option is to create cross compiler manually, in which case we only
need cross compiled `binutils`, stage1 cross compiled `gcc` and cross
compiled `newlib`. For debugging, we can use multi target `gdb`. This
setup was tested on Gentoo system and works like a charm.

Third option is to use precompiled arm toolchain that can be obtained
from it's [official site][gnu-arm]. This package contains all the
binaries for our development. There is just one small snag: in order to
be able to use `gdb`, 32-bit `ncurses` library should be installed on
host machine.

Additionally, STM development libraries should also be installed
somewhere on the system (it really doesn't matter where, since location
is configurable).

Last thing needed is `stlink` package (more specifically, `st-flash` and
`st-util` executables should be present on the system). Again, it really
doesn't matter where, since this is configurable.

## Preparation before first use

This harnes needs almost no preparation, since most of the stuff is
written in generic way that works for any project. Only configuration
that should be done resides in `common/Makefile` file under
**CONFIGURATION** section. Variables that should be set are:

  * `STM_COMMON` that should point to toplevel of STM library
  * `CC` that should contain path to cross compiler for arm
  * `AS` that should contain path to cross assembler for arm
  * `OBJCOPY` should point to cross compiled `objcopy` program
  * `GDB` should point to cross compiled or multitarget debugger
  * `STFLASH` should point to `st-flash` binary
  * `STUTIL` should point to `st-util` binary

All other sections of that makefile should be left as they are, since
they should work with no editing.

## Creating new project

Creating new project is as easy as adding new folder in which to place
sources and project makefile. Makefile should contain only two things:
`NAME` variable definition, which controls the name of the final elf
file (for example, if we place `NAME := test` in `Makefile`, end result
will be `test.elf` binary) and include statement for common `Makefile`.
For quick sample, have a look at `v01` folder.

## Compiling project

Compiling project is as easy as calling `make` in project folder. There
is nothing more to it.

## Flashing board

Again, flashing board is dead easy. Just call `make burn` and your
program will be transferred to development board. Make sure your board
is connected to computer before executing this step.

## Debugging program

Again, debugging is easy. Connect your board and call `make gdb`. This
command will make sure your binary is up-to-date, start remote `gdb`
server, connect to it, upload fresh image to board and halt the
execution of your program. Now you can debug your program just like any
other C program.


[gnu-arm]: https://launchpad.net/gcc-arm-embedded "GNU ARM toolchain"
