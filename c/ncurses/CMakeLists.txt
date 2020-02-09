cmake_minimum_required(VERSION 3.12)
project(app)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${app_SOURCE_DIR}/cmake")
set(BIN_DIR ${app_SOURCE_DIR}/bin)

# Bump up warning levels appropriately for clang, gcc & msvc and build in debug mode
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c11")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
endif()

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

include_directories("./src")
include_directories("./src/include")
file (GLOB app_SOURCES src/*.c)
file (GLOB app_HEADERS src/include/*.h)

add_executable(app ${app_HEADERS} ${app_SOURCES})
target_link_libraries(app ${CURSES_LIBRARY})
install(TARGETS app RUNTIME DESTINATION ${BIN_DIR})
