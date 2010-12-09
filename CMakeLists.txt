cmake_minimum_required(VERSION 2.6)
project(TicTocTimer)

include_directories(src)
add_library(TicTocTimer SHARED STATIC
  src/tictoc.c
  )
