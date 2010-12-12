cmake_minimum_required(VERSION 2.6)
project(TicTocTimer)
#set(CMAKE_VERBOSE_MAKEFILE 1) ## Useful for debuging the build

set(TIMER_SRCS
  src/tictoc.c
  src/tictoc.h
  )
include_directories(src)
add_library(TicTocTimer SHARED STATIC
  ${TIMER_SRCS}
  )

add_subdirectory(test)
