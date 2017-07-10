#############################################################################
#
# makefile.due common settings for Arduino Due projects
#
# (c) Wouter van Ooijen (www.voti.nl) 2016
#
# This file is in the public domain.
#
#############################################################################

# settings for Arduino Due projects
TARGET            ?= arduino_due
SERIAL_PORT       ?= COM3
CONSOLE_BAUDRATE  ?= 38400
STACK_SIZE        ?= 81920
HEAP              ?= BMPTK

PROJECT 	  	  ?= src/main

# use hwlib
SEARCH            += ./libs/hwlib
SOURCES           += hwlib.cpp

# use the rtos
SEARCH            += ./libs/rtos
HEADERS           += coroutine.hpp switch_to.hpp
SOURCES           += coroutine.cpp switch_to.asm
HEADERS           += rtos.hpp
SOURCES           += rtos.cpp

# the source and header files
<<<<<<< HEAD
SEARCH				+= ./src/
HEADERS				+= defines.hpp
HEADERS				+= OLEDBoundary.hpp
SOURCES				+= OLEDBoundary.cpp
HEADERS				+= RunGameController.hpp
SOURCES				+= RunGameController.cpp
HEADERS				+= TestTask.hpp
SOURCES				+= TestTask.cpp
=======
HEADERS				+= OLEDBoundary.hpp
SOURCES				+= OLEDBoundary.cpp
>>>>>>> 3b5a5c834c4232ecda0f58041035d835c7d5be5b

SEARCH				+= ./src/ir/
HEADERS				+= receiverController.hpp
HEADERS				+= gameParameters.hpp
HEADERS				+= transmitter.hpp
HEADERS				+= transmitterController.hpp
HEADERS				+= messageLogic.hpp

OS ?= $(shell uname)

# set BMPTK to the bmptk subdirectory
BMPTK := $(BMPTK)./libs/bmptk/

# Defer to the bmptk/Makefile.inc to do the work
include $(BMPTK)Makefile.inc

ifeq ($(OS), Linux)
run:
	sudo stty -F /dev/ttyACM0 speed 1200 cs8 -cstopb -parenb && make build && sudo bossac --info --port "ttyACM0" --erase --write --verify --reset --debug --boot -U false ./src/main.bin && sudo platformio serialports monitor -b 38400
endif