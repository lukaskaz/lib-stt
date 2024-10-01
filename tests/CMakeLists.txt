cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi")

project(stt-ut)
enable_testing()

include(cmake/flags.cmake)
include(cmake/dependencies.cmake)

option(COVERAGE "Create test coverage report" ON)
option(COVEXPORT "Export test coverage report" OFF)

if(COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage -fno-inline")
endif()

include_directories(../inc)
file(GLOB APP_SOURCES "../src/*.cpp")
list(FILTER APP_SOURCES EXCLUDE REGEX "^.*/(main)\\.cpp$")

include_directories(inc)
file(GLOB TEST_SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${APP_SOURCES} ${TEST_SOURCES})

add_dependencies(${PROJECT_NAME} googletest)
add_dependencies(${PROJECT_NAME} libshellcmd)
add_dependencies(${PROJECT_NAME} libnlohmann)
target_link_libraries(${PROJECT_NAME} shellcmd gtest gmock Boost::filesystem CURL::libcurl)

add_test(
    NAME ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
)

if(RUN_TESTS)
    add_custom_command(TARGET ${PROJECT_NAME}
        COMMENT "Running unit tests for the project"
        COMMAND GTEST_COLOR=1 ${CMAKE_CTEST_COMMAND} --output-on-failure || true
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests
        POST_BUILD
    )
endif()

set(OBJECTS_DIR "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir")
get_filename_component(SOURCES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/.. ABSOLUTE)
get_filename_component(HOME_PATH ~ ABSOLUTE)
execute_process(COMMAND date +%Y%m%d_%H%M%S OUTPUT_VARIABLE TIMESTAMP)
set(COVERAGE_COPY_DIR "${HOME_PATH}/samba/coverage/${TIMESTAMP}")

add_custom_target(coverage
    COMMENT "Coverage report"
    COMMAND echo "Creating @ timestamp ${TIMESTAMP}"
    )
    
add_custom_command(TARGET coverage
    COMMENT "Initiating resorces"
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/coverage
    COMMAND lcov --directory ${OBJECTS_DIR} --zerocounters
    COMMAND ${CMAKE_CTEST_COMMAND}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(TARGET coverage
    COMMENT "Collecting data ${SOURCES_PATH}/inc/*"
    COMMAND lcov --capture --rc lcov_branch_coverage=1 --directory ${OBJECTS_DIR}
        --output-file coverage.info
    COMMAND lcov --extract coverage.info '${SOURCES_PATH}/src/*' '${SOURCES_PATH}/inc/*'
        --output-file coverage.info
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(TARGET coverage
    COMMENT "Generating report"
    COMMAND genhtml ${CMAKE_CURRENT_BINARY_DIR}/coverage.info --output-directory .
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/coverage
)

if(COVEXPORT)
    add_custom_command(TARGET coverage
        COMMENT "Exporting to ${COVERAGE_COPY_DIR}"
        COMMAND mkdir -p ${COVERAGE_COPY_DIR}
        COMMAND cp -a . ${COVERAGE_COPY_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/coverage
    )
endif()
