cmake_minimum_required(VERSION 3.10)

option(ADD_EXAMPLES "Creates examples" OFF)

if(ADD_EXAMPLES)
    add_subdirectory(googleapi)
    add_subdirectory(googlecloud)
endif()
