cmake_minimum_required(VERSION 3.10)

if (CMAKE_SYSTEM_PROCESSOR MATCHES "^[Ww][Ii][Nn]32$")
	set(TRIPLET_ARCH x86)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^x.*64$")
	set(TRIPLET_ARCH x64)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^[Aa][Rr][Mm]$")
	set(TRIPLET_ARCH arm)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^[Aa][Rr][Mm]64$")
	set(TRIPLET_ARCH arm64)
else()
	message(FATAL_ERROR "Cannot detect architecture, abort")
endif()

if (CMAKE_SYSTEM_NAME MATCHES "^[Ll][Ii][Nn][Uu][Xx]$")
	set(TRIPLET_OS linux)
else()
	message(FATAL_ERROR "Operation system not supported, abort")
endif()

if (CMAKE_BUILD_TYPE MATCHES "^[Dd][Ee][Bb][Uu][Gg]$")
	set(TRIPLET_NAME ${TRIPLET_ARCH}-${TRIPLET_OS}-custom)
else()
	set(TRIPLET_NAME ${TRIPLET_ARCH}-${TRIPLET_OS}-release-custom)
endif()

message("Using triplet: '${TRIPLET_NAME}'")
set(VCPKG_TARGET_TRIPLET ${TRIPLET_NAME})
set(VCPKG_HOST_TRIPLET ${VCPKG_TARGET_TRIPLET})
set(VCPKG_OVERLAY_TRIPLETS "../cmake/vcpkg/triplets/")

include($ENV{VCPKG_INSTALLATION_ROOT}/scripts/buildsystems/vcpkg.cmake)

