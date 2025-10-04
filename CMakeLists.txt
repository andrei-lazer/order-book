cmake_minimum_required(VERSION 3.28)

project(OrderBook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# default build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

option(BUILD_TESTS "Build tests using GoogleTest" OFF)

# Core library
add_library(
	${PROJECT_NAME}Lib
	STATIC
		src/Order.cpp
		src/OrderBook.cpp
)

target_include_directories(${PROJECT_NAME}Lib PUBLIC src)

add_executable(Benchmark src/main.cpp)
target_link_libraries(Benchmark PRIVATE ${PROJECT_NAME}Lib)

# Tests only built if requested
if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
