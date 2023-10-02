add_executable(utest
    block_test.cpp
    grid_test.cpp
    progargs_test.cpp
	utests.cpp
)

target_link_libraries(utest
    gtest_main  # Enlazar con Google Test
    sim         # Enlazar con tu biblioteca "sim" u otras dependencias
)

add_test(NAME utest COMMAND utest)