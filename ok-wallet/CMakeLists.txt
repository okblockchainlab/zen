cmake_minimum_required(VERSION 3.4)
project(zen-wallet)

if(${PROJECT_BINARY_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
	message(FATAL_ERROR "DO NOT run cmake in source directory")
endif()

if (NOT DEPENDS_LIB)
	message(FATAL_ERROR "undefined DEPENDS_LIB")
endif()

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if (NOT OKLIBRARY_NAME)
	set(OKLIBRARY_NAME zen)
endif()

if (ENABLE_WALLET)
	add_definitions(-DENABLE_WALLET)
endif()

include_directories(${DEPENDS_LIB}/include)

set(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_C_FLAGS_RELEASE "$ENV{CFLAGS} -O2 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CFLAGS} -O0 -Wall -g -ggdb -fopenmp")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CFLAGS} -O2 -Wall -fopenmp")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

enable_testing()

link_libraries(pthread)
link_libraries(dl)

include(ExternalProject)
add_subdirectory(src)
