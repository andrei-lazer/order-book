cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(OrderBook)
set(SOURCE_FILES
	src/main.cpp
	src/Order.cpp
)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# testing using Google Test

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/eb2d85edd0bff7a712b6aff147cd9f789f0d7d0b.zip

)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
set(TEST_FILES tests/test_main.cpp)


set(TEST_NAME OrderBookTests)
add_executable(${TEST_NAME} ${TEST_FILES})

target_link_libraries(
  OrderBookTests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${TEST_NAME})
