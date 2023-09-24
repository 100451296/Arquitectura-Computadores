add_executable(utest
    block_test.cpp
    grid_test.cpp
    progargs_test.cpp
)
target_link_libraries(utest sim)
