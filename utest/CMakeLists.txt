add_executable(utest
    grid_test.cpp
    gridSoA_test.cpp
    progargs_test.cpp
    gridTest.cpp
    gridTestSoA.cpp
	utests.cpp
)

# Library dependencies
target_link_libraries (utest
PRIVATE
sim
soa
GTest::gtest_main
Microsoft.GSL::GSL)

add_test(NAME utest COMMAND utest)
# Discover all tests and add them to the test driver
include(GoogleTest)
gtest_discover_tests(utest)