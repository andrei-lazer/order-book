project(Tests LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/eb2d85edd0bff7a712b6aff147cd9f789f0d7d0b.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(TEST_FILES
	test_main.cpp
	test_lockfree_fifo.cpp)

add_executable(Tests ${TEST_FILES})

target_link_libraries(
	Tests
	PRIVATE
		OrderBookLib
		GTest::gtest_main
)

target_compile_options(Tests PRIVATE)
target_link_options(Tests PRIVATE)

include(GoogleTest)
gtest_discover_tests(Tests)
