add_executable(fluid fluid.cpp)
target_link_libraries(fluid sim)
target_include_directories(fluid PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
