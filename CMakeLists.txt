cmake_minimum_required(VERSION 2.6)
project(tictoc)

find_library(LIBRT NAMES rt)

set(TIMER_SRCS
  src/tictoc.c
  src/tictoc.h
  )
include_directories(src)
add_library(tictoc SHARED STATIC
  ${TIMER_SRCS}
  )
if(LIBRT)
  target_link_libraries(tictoc ${LIBRT})
endif()

###############################################################################
#  Testing
###############################################################################

find_path(GTEST_ROOT
  CMakeLists.txt
  HINTS ${ROOT_3RDPARTY_PATH} ~/src/3rdParty
  PATH_SUFFIXES
    gtest-1.6.0
  DOC "The root directory of the gtest install prefix")
find_package(GTest)
find_package(Threads)
if(GTEST_FOUND)
  enable_testing()
  include_directories(${PROJECT_SOURCE_DIR})
  include_directories(${GTEST_INCLUDE_DIRS})
  file(GLOB TEST_SOURCES test/*.cc)
  add_executable(tictoc-tests
    ${TEST_SOURCES}
    )
  target_link_libraries(tictoc-tests
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    tictoc
    )
  add_test(TestTicToc tictoc-tests)
endif()
