add_executable(utest
    block_test.cpp
    grid_test.cpp
    progargs_test.cpp
	utests.cpp
)

# Library dependencies
target_link_libraries (utest
PRIVATE
sim
GTest::gtest_main
Microsoft.GSL::GSL)

add_test(NAME utest COMMAND utest)
# Discover all tests and add them to the test driver
include(GoogleTest)
gtest_discover_tests(utest)