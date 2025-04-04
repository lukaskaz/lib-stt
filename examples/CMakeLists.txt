cmake_minimum_required(VERSION 3.10)

option(ADD_EXAMPLES "Creates examples" OFF)

if(ADD_EXAMPLES)
    add_subdirectory(v1/googlecloud)
    add_subdirectory(v2/googleapi)
    add_subdirectory(v2/googlecloud)
endif()
