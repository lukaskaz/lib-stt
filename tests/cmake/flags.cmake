cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
  -pedantic\
  -Wall \
  -Wextra \
  -Wnon-virtual-dtor \
  -Wpointer-arith \
  -Wcast-qual \
  -Wconversion \
  -Wno-sign-conversion \
  -Woverloaded-virtual \
  -Wpointer-arith \
  -Wcast-qual \
  -Wno-strict-aliasing"
  # -Wshadow \
)
