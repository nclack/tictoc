cmake_minimum_required(VERSION 2.6)
project(TicTocTimer)

set(ROOT_3RDPARTY_PATH ${PROJECT_SOURCE_DIR}/3rdParty)

set(TIMER_SRCS
  src/tictoc.c
  src/tictoc.h
  )
include_directories(src)
add_library(TicTocTimer SHARED STATIC
  ${TIMER_SRCS}
  )

add_subdirectory(test)
