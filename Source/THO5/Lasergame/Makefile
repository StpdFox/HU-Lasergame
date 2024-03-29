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
TARGET          	?= arduino_due
SERIAL_PORT     	?= COM5
CONSOLE_BAUDRATE	?= 38400
STACK_SIZE      	?= 81920
HEAP            	?= BMPTK

PROJECT 	  		?= src/main

# use hwlib
SEARCH          	+= ./libs/hwlib
SOURCES         	+= hwlib.cpp

# use the rtos
SEARCH          	+= ./libs/rtos
HEADERS         	+= coroutine.hpp switch_to.hpp
SOURCES         	+= coroutine.cpp switch_to.asm
HEADERS         	+= rtos.hpp
SOURCES         	+= rtos.cpp
HEADERS				+= src/RunGameController.hpp
SOURCES				+= src/RunGameController.cpp


HEADERS				+= src/KeypadController.hpp
SOURCES				+= src/KeypadController.cpp

HEADERS				+= src/InitGameController.hpp
SOURCES				+= src/InitGameController.cpp

HEADERS				+= src/KeypadListener.hpp

HEADERS				+= src/SpeakerController.hpp
SOURCES				+= src/SpeakerController.cpp

HEADERS				+= src/GameParamsController.hpp
SOURCES				+=src/GameParamsController.cpp

# the source and header files

SEARCH				+= ./src/
HEADERS				+= defines.h
HEADERS				+= OLEDBoundary.hpp
SOURCES				+= OLEDBoundary.cpp

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
