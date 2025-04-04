cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
project(stt)

include(cmake/vcpkg/setup.cmake)
include(cmake/flags.cmake)
include(cmake/dependencies.cmake)

option(RUN_TESTS "Creates and runs unit tests for the project" OFF)

if(RUN_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

add_subdirectory(examples)

include_directories(inc)
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} ${SOURCES})
add_dependencies(${PROJECT_NAME} liblogger)
add_dependencies(${PROJECT_NAME} libshellcmd)
add_dependencies(${PROJECT_NAME} libnlohmann)
target_link_libraries(${PROJECT_NAME} logger shell CURL::libcurl google-cloud-cpp::speech)

