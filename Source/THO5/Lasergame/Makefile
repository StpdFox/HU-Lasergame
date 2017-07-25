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

SEARCH			+= ./src/
HEADERS			+= RunGameController.hpp
SOURCES			+= RunGameController.cpp

HEADERS			+= KeypadController.hpp
SOURCES			+= KeypadController.cpp

HEADERS			+= InitGameController.hpp
SOURCES			+= InitGameController.cpp

HEADERS			+= KeypadListener.hpp
HEADERS			+= SpeakerController.hpp
SOURCES			+= SpeakerController.cpp

HEADERS			+= GameParamsController.hpp
SOURCES			+= GameParamsController.cpp

HEADERS			+= glcd_oled_part_buffered.hpp

# the source and header files

HEADERS				+= defines.h
HEADERS				+= OLEDBoundary.hpp
SOURCES				+= OLEDBoundary.cpp

SEARCH				+= ./src/ir/
HEADERS				+= receiverController.hpp
SOURCES				+= receiverController.cpp
HEADERS				+= gameParameters.hpp
HEADERS				+= transmitter.hpp
SOURCES 			+= transmitter.cpp
HEADERS				+= transmitterController.hpp
SOURCES				+= transmitterController.cpp
HEADERS				+= messageLogic.hpp
SOURCES 			+= messageLogic.cpp

OS ?= $(shell uname)

# set BMPTK to the bmptk subdirectory
BMPTK := $(BMPTK)./libs/bmptk/

# Defer to the bmptk/Makefile.inc to do the work
include $(BMPTK)Makefile.inc

ifeq ($(OS), Linux)
run:
	sudo stty -F /dev/ttyACM0 speed 1200 cs8 -cstopb -parenb && make build && sudo bossac --info --port "ttyACM0" --erase --write --verify --reset  --boot -U false ./src/main.bin && platformio serialports monitor -b 38400
endif