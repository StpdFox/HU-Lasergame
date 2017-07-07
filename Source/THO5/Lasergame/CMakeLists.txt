cmake_minimum_required(VERSION 3.2)
project(laser)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs/hwlib")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs/bmptk")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs/rtos")

#
# To enable code auto-completion, please specify path
# to main source file (*.c, *.cpp) and uncomment line below
#
add_executable(lightboard
        src/wrap-hwlib.cc
        src/main.cpp)