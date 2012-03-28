cmake_minimum_required(VERSION 2.6)
project(TicTocTimer)

set(ROOT_3RDPARTY_PATH ${PROJECT_SOURCE_DIR}/3rdParty)

find_library(LIBRT NAMES rt)

set(TIMER_SRCS
  src/tictoc.c
  src/tictoc.h
  )
include_directories(src)
add_library(TicTocTimer SHARED STATIC
  ${TIMER_SRCS}
  )
if(LIBRT)
  target_link_libraries(TicTocTimer ${LIBRT})
endif()

add_subdirectory(test)
