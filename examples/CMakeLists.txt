include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_BINARY_DIR}/lib)

add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark kispmv)
