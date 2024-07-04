cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
project(prog)
include_directories(${CMAKE_SOURCE_DIR}/inc)
file(GLOB SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} tts)
