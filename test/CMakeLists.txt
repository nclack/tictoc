#
# INPUT VARIABLES
# ---------------
#
# ROOT_3RDPARTY_PATH
#

#Some stuff to let this CMakeLists.txt stand alone, if needed
cmake_minimum_required(VERSION 2.8)
project(tests)
enable_testing(true)

#set(CMAKE_VERBOSE_MAKEFILE 1) ## Useful for debuging the build

#find gtest
#message(${ROOT_3RDPARTY_PATH})
set(GTEST_ROOT "${ROOT_3RDPARTY_PATH}/gtest")
set(GTEST_MSVC_SEARCH MD)
find_package(GTest)
if(GTEST_FOUND)
  include_directories(${GTEST_INCLUDE_DIRS})

##
# Test Sources
#
  include_directories(../src)
  file(GLOB TEST_SOURCES
    ../src/*.c
    ../src/*.cc
    ../src/*.cpp
    ../src/*.h
    ../src/*.hh
    ../src/*.hpp
    *.cc
    *.cpp
    *.h
    *.hh
    *.hpp
  )

##
# Config
#

find_package(Threads)
find_library(LIBRT NAMES rt)

##
# Add tests
#
  add_executable(alltests alltests.cc 
    ${TEST_SOURCES} 
  )
  target_link_libraries(alltests
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
  )
  if(LIBRT)
    target_link_libraries(alltests ${LIBRT})
  endif()
  set_target_properties(alltests
      PROPERTIES
      COMPILE_DEFINITIONS
        HAVE_GTEST     
  )
  if(TEST_SOURCES)
    gtest_add_tests(alltests " " ${TEST_SOURCES})
  endif()
endif(GTEST_FOUND)
